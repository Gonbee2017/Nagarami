#include<cmath>
#include<cstdlib>
#include<fstream>
#include<ios>
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

Initializer::Initializer(const function<void()>&initialize)
{initialize();}

Finalizer::Finalizer(const function<void()>&finalize):
    finalize_(finalize) {}

Finalizer::~Finalizer() {finalize_();}

api_error::api_error(const string&name):
    runtime_error(describe(name," failed.(",pt().GetLastError(),")")) {}

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

bool contain(const POINT&center,const LONG&squaredRadius,const POINT&pos)
{return squared_distance(center,pos)<=squaredRadius;}

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
    POINT pos=ZERO_POS;
    nm::GetCursorPos(&pos);
    nm::ScreenToClient(window,&pos);
    return pos;
}

void describe_to_with(ostream&os,const string&separator) {}

void describe_to_with
(ostream&os,const string&separator,const char*str)
{if(str!=nullptr) os<<str;}

void describe_to_with
(ostream&os,const string&separator,LPPOINT point) {os<<*point;}

SIZE desktop_size()
{
    return SIZE(
    {
        nm::GetSystemMetrics(SM_CXSCREEN),
        nm::GetSystemMetrics(SM_CYSCREEN)
    });
}

double floating_point_number(const string&str)
{
    char*end;
    double number=strtod(str.c_str(),&end);
    if(*end!='\0')
        throw make_shared<runtime_error>(describe
        ("'",str,"' is an invalid floating-point number."));
    return number;
}

vector<string> getlines(istream&is)
{
    vector<string> lines;
    string line;
    while(getline(is,line)) lines.push_back(chomp(line,'\r'));
    return lines;
}

LONG height(const RECT&rect) {return rect.bottom-rect.top;}

shared_ptr<istream> input_file
(const string&name,const ios_base::openmode&mode)
{return make_shared<ifstream>(name,mode);}

long integer(const string&str)
{
    char*end;
    long number=strtol(str.c_str(),&end,0);
    if(*end!='\0')
        throw make_shared<runtime_error>(describe
        ("'",str,"' is an invalid integer."));
    return number;
}

bool operator!=(const POINT&lhs,const POINT&rhs) {return !(lhs==rhs);}

bool operator!=(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs)
{return !(lhs==rhs);}

bool operator!=(const RECT&lhs,const RECT&rhs) {return !(lhs==rhs);}

bool operator!=(const SIZE&lhs,const SIZE&rhs) {return !(lhs==rhs);}

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

POINT operator-(const POINT&point) {return POINT({-point.x,-point.y});}

POINT operator-(const POINT&lhs,const POINT&rhs)
{return POINT({lhs.x-rhs.x,lhs.y-rhs.y});}

SIZE operator-(const SIZE&lhs,const SIZE&rhs)
{return SIZE({lhs.cx-rhs.cx,lhs.cy-rhs.cy});}

POINT operator/(const POINT&lhs,const LONG&rhs)
{return POINT({lhs.x/rhs,lhs.y/rhs});}

SIZE operator/(const SIZE&lhs,const LONG&rhs)
{return SIZE({lhs.cx/rhs,lhs.cy/rhs});}

ostream&operator<<(ostream&os,const POINT&point)
{return os<<describe("{",describe_with(",",point.x,point.y),"}");}

ostream&operator<<(ostream&os,const POINT_DOUBLE&point)
{return os<<describe("{",describe_with(",",point.x,point.y),"}");}

ostream&operator<<(ostream&os,const RECT&rect)
{
    return os<<describe(
    "{",describe_with
        (",",rect.left,rect.top,rect.right,rect.bottom),
    "}");
}

ostream&operator<<(ostream&os,const SIZE&size)
{return os<<describe("{",describe_with(",",size.cx,size.cy),"}");}

bool operator==(const POINT&lhs,const POINT&rhs)
{return lhs.x==rhs.x&&lhs.y==rhs.y;}

bool operator==(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs)
{return lhs.x==rhs.x&&lhs.y==rhs.y;}

bool operator==(const RECT&lhs,const RECT&rhs)
{
    return
        lhs.left==rhs.left&&
        lhs.top==rhs.top&&
        lhs.right==rhs.right&&
        lhs.bottom==rhs.bottom;
}

bool operator==(const SIZE&lhs,const SIZE&rhs)
{return lhs.cx==rhs.cx&&lhs.cy==rhs.cy;}

shared_ptr<ostream> output_file
(const string&name,const ios_base::openmode&mode)
{return make_shared<ofstream>(name,mode);}

POINT point(const POINT_DOUBLE&pointDouble)
{return POINT({(LONG)round(pointDouble.x),(LONG)round(pointDouble.y)});}

POINT point(const SIZE&size) {return POINT({size.cx,size.cy});}

POINT_DOUBLE point_double(const POINT&point)
{return POINT_DOUBLE({(double)point.x,(double)point.y});}

POINT pos(const RECT&rect) {return POINT({rect.left,rect.top});}

void putlines(ostream&os,const vector<string>&lines)
{for(const string&line:lines) os<<line<<endl;}

RECT rect(const POINT&pos,const SIZE&size)
{return RECT({pos.x,pos.y,pos.x+size.cx,pos.y+size.cy});}

SIZE size(LPARAM lParam)
{return SIZE({(short)LOWORD(lParam),(short)HIWORD(lParam)});}

SIZE size(const POINT&pos) {return SIZE({pos.x,pos.y});}

SIZE size(const RECT&rect) {return SIZE({width(rect),height(rect)});}

LONG squared_distance(const POINT&p,const POINT&q)
{
    const LONG dx=p.x-q.x;
    const LONG dy=p.y-q.y;
    return SQUARE(dx)+SQUARE(dy);
}

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
