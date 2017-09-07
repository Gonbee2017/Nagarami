#ifndef NAGARAMI_TEST_H
#define NAGARAMI_TEST_H

#include"../nagarami.h"
#include<cstddef>
#include<functional>
#include<iomanip>
#include<iostream>
#include<ostream>
#include<string>
#include<utility>
#include<vector>
#include<CppUTest/TestHarness.h>

#define DEBUG_PRINT(x) cout<<#x<<":'"<<(x)<<"'"<<endl
#define DEBUG_PRINTF(x,...)\
    cout<<#x<<":'"<<describe(__VA_ARGS__,(x))<<"'"<<endl
#define NAMED_ADDRESS(x) (make_pair(string(#x),&(x)))

namespace nm
{

using namespace std;

class call
{
public:
    template<class...ARGUMENTS>
        call(const string&name,ARGUMENTS&&...arguments);

    string arguments() const;
    string name() const;
    bool operator!=(const call&rhs) const;
    bool operator==(const call&rhs) const;
private:
    friend ostream&operator<<(ostream&os,const call&call_);
    string arguments_;
    string name_;
};

class history
{
public:
    template<class...ARGUMENTS> void set
    (const pair<string,function<void(ARGUMENTS...)>*>&namedFunc);
    template<class BODY,class RESULT,class...ARGUMENTS> void setWithBody
    (
        const pair<string,function<RESULT(ARGUMENTS...)>*>&namedFunc,
        const BODY&body
    );
    template<class RESULT,class...ARGUMENTS> void setWithResult
    (
        const pair<string,function<RESULT(ARGUMENTS...)>*>&namedFunc,
        const RESULT&result
    );

    ~history();
    const vector<call>&calls() const;
    size_t count(const string&callName) const;
protected:vector<call> calls_;
};

template<class VALUE> SimpleString StringFrom(const VALUE&value);

template<class...ARGUMENTS>
    call::call(const string&name,ARGUMENTS&&...arguments):
    name_(name),arguments_(describe_with(",",arguments...)) {}

template<class...ARGUMENTS> void history::set
(const pair<string,function<void(ARGUMENTS...)>*>&namedFunc)
{
    *namedFunc.second=[this,namedFunc] (ARGUMENTS&&...arguments)
    {calls_.push_back(call(namedFunc.first,arguments...));};
}

template<class BODY,class RESULT,class...ARGUMENTS>
    void history::setWithBody
(
    const pair<string,function<RESULT(ARGUMENTS...)>*>&namedFunc,
    const BODY&body
)
{
    *namedFunc.second=
    [this,namedFunc,body] (ARGUMENTS&&...arguments)->RESULT
    {
        calls_.push_back(call(namedFunc.first,arguments...));
        return body(arguments...);
    };
}

template<class RESULT,class...ARGUMENTS> void history::setWithResult
(
    const pair<string,function<RESULT(ARGUMENTS...)>*>&namedFunc,
    const RESULT&result
)
{
    *namedFunc.second=
    [this,namedFunc,result] (ARGUMENTS&&...arguments)->RESULT
    {
        calls_.push_back(call(namedFunc.first,arguments...));
        return result;
    };
}

template<class VALUE> SimpleString StringFrom(const VALUE&value)
{return SimpleString(describe(value).c_str());}

}

#endif
