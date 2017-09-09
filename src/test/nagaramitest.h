#ifndef NAGARAMITEST_H
#define NAGARAMITEST_H

#include"../nagarami.h"
#include<cstddef>
#include<functional>
#include<iomanip>
#include<iostream>
#include<ostream>
#include<stdexcept>
#include<string>
#include<utility>
#include<vector>
#include<CppUTest/TestHarness.h>

#define DEBUG_PRINT(x) cout<<#x<<":'"<<(x)<<"'"<<endl
#define DEBUG_PRINTF(x,...)\
    cout<<#x<<":'"<<describe(__VA_ARGS__,(x))<<"'"<<endl
#define NAMED_ADDRESS(x) (make_pair(string(#x),&(x)))
#define CHECK_THROWS_RUNTIME_ERROR(expected_what,expression)\
    try\
    {\
        expression;\
        FAIL("Do not pass here.");\
    } catch(const runtime_error&error)\
    {CHECK_EQUAL((expected_what),string(error.what()));}

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

class logger
{
public:
    template<class...ARGUMENTS> void setPut
    (const pair<string,function<void(ARGUMENTS...)>*>&namedFunc);
    template<class BODY,class RESULT,class...ARGUMENTS> void setPutWithBody
    (
        const pair<string,function<RESULT(ARGUMENTS...)>*>&namedFunc,
        const BODY&body
    );
    template<class RESULT,class...ARGUMENTS> void setPutWithResult
    (
        const pair<string,function<RESULT(ARGUMENTS...)>*>&namedFunc,
        const RESULT&result
    );

    ~logger();
    const vector<call>&history() const;
    size_t count(const string&callName) const;
protected:vector<call> history_;
};

template<class VALUE> SimpleString StringFrom(const VALUE&value);
bool operator!=(const POINT&lhs,const POINT&rhs);
bool operator!=(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs);
bool operator!=(const RECT&lhs,const RECT&rhs);
bool operator!=(const SIZE&lhs,const SIZE&rhs);
ostream&operator<<(ostream&os,const MSG&message);
ostream&operator<<(ostream&os,const MSG*const message);
ostream&operator<<(ostream&os,const PAINTSTRUCT&paint);
ostream&operator<<(ostream&os,const PAINTSTRUCT*const paint);
ostream&operator<<(ostream&os,const POINT&point);
ostream&operator<<(ostream&os,const POINT*const point);
ostream&operator<<(ostream&os,const POINT_DOUBLE&point);
ostream&operator<<(ostream&os,const RECT&rect);
ostream&operator<<(ostream&os,const RECT*rect);
ostream&operator<<(ostream&os,const SIZE&size);
ostream&operator<<(ostream&os,const WNDCLASSEX&clazz);
ostream&operator<<(ostream&os,const WNDCLASSEX*clazz);
ostream&operator<<(ostream&os,const WINDOWPLACEMENT&placement);
ostream&operator<<(ostream&os,const WINDOWPLACEMENT*const placement);
bool operator==(const POINT&lhs,const POINT&rhs);
bool operator==(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs);
bool operator==(const RECT&lhs,const RECT&rhs);
bool operator==(const SIZE&lhs,const SIZE&rhs);

template<class...ARGUMENTS>
    call::call(const string&name,ARGUMENTS&&...arguments):
    name_(name),arguments_(describe_with(",",arguments...)) {}

template<class...ARGUMENTS> void logger::setPut
(const pair<string,function<void(ARGUMENTS...)>*>&namedFunc)
{
    string name;
    function<void(ARGUMENTS...)>*func;
    decompose(namedFunc,&name,&func);
    *func=[this,name] (ARGUMENTS&&...arguments)
    {history_.push_back(call(name,arguments...));};
}

template<class BODY,class RESULT,class...ARGUMENTS>
    void logger::setPutWithBody
(
    const pair<string,function<RESULT(ARGUMENTS...)>*>&namedFunc,
    const BODY&body
)
{
    string name;
    function<RESULT(ARGUMENTS...)>*func;
    decompose(namedFunc,&name,&func);
    *func=[this,name,body] (ARGUMENTS&&...arguments)->RESULT
    {
        history_.push_back(call(name,arguments...));
        return body(arguments...);
    };
}

template<class RESULT,class...ARGUMENTS> void logger::setPutWithResult
(
    const pair<string,function<RESULT(ARGUMENTS...)>*>&namedFunc,
    const RESULT&result
)
{
    string name;
    function<RESULT(ARGUMENTS...)>*func;
    decompose(namedFunc,&name,&func);
    *func=[this,name,result] (ARGUMENTS&&...arguments)->RESULT
    {
        history_.push_back(call(name,arguments...));
        return result;
    };
}

template<class VALUE> SimpleString StringFrom(const VALUE&value)
{return SimpleString(describe(value).c_str());}

}

#endif
