#ifndef TEST_H
#define TEST_H

#include<cstddef>
#include<functional>
#include<iomanip>
#include<iostream>
#include"nagarami.h"
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

class ClearPort:Finalizer {public:ClearPort();};

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
    template<class BODY,class...ARGUMENTS> void set
    (
        const pair<string,function<void(ARGUMENTS...)>*>&namedFunc,
        const BODY&body
    );
    template<class RESULT,class...ARGUMENTS> void set
    (
        const pair<string,function<RESULT(ARGUMENTS...)>*>&namedFunc,
        const RESULT&result
    );
    template<class BODY,class RESULT,class...ARGUMENTS> void set
    (
        const pair<string,function<RESULT(ARGUMENTS...)>*>&namedFunc,
        const RESULT&result,
        const BODY&body
    );

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

template<class BODY,class...ARGUMENTS> void history::set
(
    const pair<string,function<void(ARGUMENTS...)>*>&namedFunc,
    const BODY&body
)
{
    *namedFunc.second=[this,namedFunc,body] (ARGUMENTS&&...arguments)
    {
        calls_.push_back(call(namedFunc.first,arguments...));
        body(arguments...);
    };
}

template<class RESULT,class...ARGUMENTS> void history::set
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

template<class BODY,class RESULT,class...ARGUMENTS> void history::set
(
    const pair<string,function<RESULT(ARGUMENTS...)>*>&namedFunc,
    const RESULT&result,
    const BODY&body
)
{
    *namedFunc.second=
    [this,namedFunc,result,body] (ARGUMENTS&&...arguments)->RESULT
    {
        calls_.push_back(call(namedFunc.first,arguments...));
        body(arguments...);
        return result;
    };
}

template<class VALUE> SimpleString StringFrom(const VALUE&value)
{return SimpleString(describe(value).c_str());}

}

#endif
