#include<cmath>
#include<cstdlib>
#include<fstream>
#include<ios>
#include<iostream>
#include<istream>
#include<memory>
#include"nagarami.h"
#include<ostream>
#include<stdexcept>
#include<string>
#include<vector>
#include<windows.h>

namespace nm
{

Finalizer::Finalizer(const function<void()>&finalize):
    finalize_(finalize) {}

Finalizer::~Finalizer() {finalize_();}

api_error::api_error(const string&functionName):
    api_error(functionName,pt->GetLastError()) {}

api_error::api_error(const string&functionName,const DWORD&code):
    runtime_error(describe(functionName," failed.(",code,")")),
    functionName_(functionName),
    code_(code) {}

DWORD api_error::code() const {return code_;}

string api_error::functionName() const {return functionName_;}

string chomp(const string&str,const char&ch)
{
    string result;
    if(!str.empty())
    {
        if(str.back()==ch) result=str.substr(0,str.length()-1);
        else result=str;
    }
    return result;
}

bool contain(const RECT&rect,const POINT&pos)
{
    return
        pos.x>=rect.left&&pos.x<rect.right&&
        pos.y>=rect.top&&pos.y<rect.bottom;
}

POINT coordinates(LPARAM lParam)
{return POINT({(short)LOWORD(lParam),(short)HIWORD(lParam)});}

POINT cursor_pos(HWND window)
{
    POINT pos;
    nm::GetCursorPos(&pos);
    nm::ScreenToClient(window,&pos);
    return pos;
}

void describe_to_with(ostream&os,const string&separator) {}

LONG desktop_height() {return nm::GetSystemMetrics(SM_CYSCREEN);}

SIZE desktop_size() {return SIZE({desktop_width(),desktop_height()});}

LONG desktop_width() {return nm::GetSystemMetrics(SM_CXSCREEN);}

long double floating_point_number(const string&str)
{
    char*end;
    long double number=strtold(str.c_str(),&end);
    if(*end!='\0')
        throw runtime_error(describe
        ("'",str,"' is an invalid floating-point number."));
    return number;
}

long double floating_point_number
(const string&str,const long double&reserve)
{
    long double number=reserve;
    try {number=floating_point_number(str);} catch(...) {}
    return number;
}

vector<string> get_lines(istream&is)
{
    vector<string> lines;
    string line;
    while(getline(is,line)) lines.push_back(chomp(line,'\r'));
    return lines;
}

LONG height(const RECT&rect) {return rect.bottom-rect.top;}

shared_ptr<istream> input_file
(const string&name,const ios::openmode&mode)
{return make_shared<ifstream>(name,mode);}

long long integer(const string&str)
{
    char*end;
    long long number=strtoll(str.c_str(),&end,0);
    if(*end!='\0')
        throw runtime_error(describe("'",str,"' is an invalid integer."));
    return number;
}

long long integer(const string&str,const long long&reserve)
{
    long long number=reserve;
    try {number=integer(str);} catch(...) {}
    return number;
}

POINT operator*(const POINT&lhs,const LONG&rhs)
{return POINT({lhs.x*rhs,lhs.y*rhs});}

SIZE operator*(const SIZE&lhs,const LONG&rhs)
{return SIZE({lhs.cx*rhs,lhs.cy*rhs});}

POINT operator+(const POINT&lhs,const POINT&rhs)
{return POINT({lhs.x+rhs.x,lhs.y+rhs.y});}

POINT_DOUBLE operator+(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs)
{return POINT_DOUBLE({lhs.x+rhs.x,lhs.y+rhs.y});}

POINT&operator+=(POINT&lhs,const POINT&rhs) {return lhs=lhs+rhs;}

POINT_DOUBLE&operator+=(POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs)
{return lhs=lhs+rhs;}

POINT operator-(const POINT&lhs,const POINT&rhs)
{return POINT({lhs.x-rhs.x,lhs.y-rhs.y});}

SIZE operator-(const SIZE&lhs,const SIZE&rhs)
{return SIZE({lhs.cx-rhs.cx,lhs.cy-rhs.cy});}

POINT operator/(const POINT&lhs,const LONG&rhs)
{return POINT({lhs.x/rhs,lhs.y/rhs});}

SIZE operator/(const SIZE&lhs,const LONG&rhs)
{return SIZE({lhs.cx/rhs,lhs.cy/rhs});}

shared_ptr<ostream> output_file
(const string&name,const ios::openmode&mode)
{return make_shared<ofstream>(name,mode);}

POINT point(const POINT_DOUBLE&pointDouble)
{return POINT({(LONG)round(pointDouble.x),(LONG)round(pointDouble.y)});}

POINT point(const SIZE&size) {return POINT({size.cx,size.cy});}

POINT_DOUBLE point_double(const POINT&point)
{return POINT_DOUBLE({(double)point.x,(double)point.y});}

POINT pos(const RECT&rect) {return POINT({rect.left,rect.top});}

void put_lines(ostream&os,const vector<string>&lines)
{for(const string&line:lines) os<<line<<endl;}

RECT rect(const POINT&pos,const SIZE&size)
{return RECT({pos.x,pos.y,pos.x+size.cx,pos.y+size.cy});}

SIZE size(LPARAM lParam)
{return SIZE({(short)LOWORD(lParam),(short)HIWORD(lParam)});}

SIZE size(const POINT&pos) {return SIZE({pos.x,pos.y});}

SIZE size(const RECT&rect) {return SIZE({width(rect),height(rect)});}

vector<string> tokenize(const string&str,const string&dels)
{
    vector<string> tokens;
    for(string::size_type pos=0;pos<=str.length();)
    {
        string::size_type delPos=str.find_first_of(dels,pos);
        if(delPos==string::npos) delPos=str.length();
        if(delPos!=pos) tokens.push_back(str.substr(pos,delPos-pos));
        pos=delPos+1;
    }
    return tokens;
}

LONG width(const RECT&rect) {return rect.right-rect.left;}

}
