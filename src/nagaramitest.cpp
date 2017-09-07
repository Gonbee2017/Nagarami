#include<algorithm>
#include"nagaramitest.h"

namespace nm
{

ClearPort::ClearPort():Finalizer([] {pt().clear();}) {}

string call::arguments() const {return arguments_;}

string call::name() const {return name_;}

bool call::operator!=(const call&rhs) const {return !(*this==rhs);}

bool call::operator==(const call&rhs) const
{return name_==rhs.name_&&arguments_==rhs.arguments_;}

ostream&operator<<(ostream&os,const call&call_)
{
    ostringstream oss;
    oss<<call_.name_;
    if(!call_.arguments_.empty()) oss<<':'<<call_.arguments_;
    return os<<oss.str();
}

const vector<call>&history::calls() const {return calls_;}

size_t history::count(const string&name) const
{
    return count_if
    (
        calls_.begin(),
        calls_.end(),
        [&name] (const call&call_)->bool {return call_.name()==name;}
    );
}

}
