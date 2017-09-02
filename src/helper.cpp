#include<cmath>
#include<cstdlib>
#include"nagarami.h"
#include<istream>
#include<memory>
#include<ostream>
#include<stdexcept>
#include<string>
#include<vector>
#include<windows.h>

namespace nagarami
{

Initializer::Initializer(function<void()> initialize) {initialize();}

Finalizer::Finalizer(function<void()> finalize):
    finalize_(finalize) {}

Finalizer::~Finalizer() {finalize_();}

api_error::api_error(const string&name):
    runtime_error(describe(name," failed.(",GetLastError(),")")) {}

bool contain(const POINT&center,const LONG&squaredRadius,const POINT&pos)
{return squared_distance(pos,center)<=squaredRadius;}

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
    nagarami::GetCursorPos(&pos);
    nagarami::ScreenToClient(window,&pos);
    return pos;
}

void describe_to_with
(ostream&os,const string&separator,const char*argument)
{if(argument!=nullptr) os<<argument;}

void describe_to_with(ostream&os,const string&separator) {}

SIZE desktop_size()
{
    return SIZE(
    {
        nagarami::GetSystemMetrics(SM_CXSCREEN),
        nagarami::GetSystemMetrics(SM_CYSCREEN)
    });
}

double fpn(const string&str)
{
    char*end;
    double number=strtod(str.c_str(),&end);
    if(*end!='\0')
        throw make_shared<runtime_error>(describe
        ("'",str,"' is an invalid floating-point number."));
    return number;
}

vector<string> getlines(istream&in)
{
    vector<string> lines;
    string line;
    while(getline(in,line))
    {
        if(!line.empty()&&line.back()=='\r')
            line=line.substr(0,line.length()-1);
        lines.push_back(line);
    }
    return lines;
}

LONG height(const RECT&rect) {return rect.bottom-rect.top;}

long integer(const string&str)
{
    char*end;
    long number=strtol(str.c_str(),&end,0);
    if(*end!='\0')
        throw make_shared<runtime_error>(describe
        ("'",str,"' is an invalid integer."));
    return number;
}

POINT operator*(const POINT&lhs,const LONG&rhs)
{return POINT({lhs.x*rhs,lhs.y*rhs});}

POINT_DOUBLE operator*(const POINT_DOUBLE&lhs,const LONG&rhs)
{return POINT_DOUBLE({lhs.x*rhs,lhs.y*rhs});}

SIZE operator*(const SIZE&lhs,const LONG&rhs)
{return SIZE({lhs.cx*rhs,lhs.cy*rhs});}

POINT operator+(const POINT&lhs,const LONG&rhs)
{return POINT({lhs.x+rhs,lhs.y+rhs});}

POINT operator+(const POINT&lhs,const POINT&rhs)
{return POINT({lhs.x+rhs.x,lhs.y+rhs.y});}

POINT_DOUBLE operator+(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs)
{return POINT_DOUBLE({lhs.x+rhs.x,lhs.y+rhs.y});}

POINT&operator+=(POINT&lhs,const POINT&rhs) {return lhs=lhs+rhs;}

POINT_DOUBLE&operator+=(POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs)
{return lhs=lhs+rhs;}

POINT operator/(const POINT&lhs,const LONG&rhs)
{return POINT({lhs.x/rhs,lhs.y/rhs});}

POINT_DOUBLE operator/(const POINT_DOUBLE&lhs,const LONG&rhs)
{return POINT_DOUBLE({lhs.x/rhs,lhs.y/rhs});}

SIZE operator/(const SIZE&lhs,const LONG&rhs)
{return SIZE({lhs.cx/rhs,lhs.cy/rhs});}

POINT operator-(const POINT&point) {return POINT({-point.x,-point.y});}

SIZE operator-(const SIZE&lhs,const LONG&rhs)
{return SIZE({lhs.cx-rhs,lhs.cy-rhs});}

SIZE operator-(const SIZE&lhs,const SIZE&rhs)
{return SIZE({lhs.cx-rhs.cx,lhs.cy-rhs.cy});}

POINT point(const POINT_DOUBLE&pointDouble)
{return POINT({(LONG)round(pointDouble.x),(LONG)round(pointDouble.y)});}

POINT point(const SIZE&size) {return POINT({size.cx,size.cy});}

POINT pos(const RECT&rect) {return POINT({rect.left,rect.top});}

POINT_DOUBLE point_double(const POINT&point)
{return POINT_DOUBLE({(double)point.x,(double)point.y});}

void putlines(ostream&out,const vector<string>&lines)
{for(const string&line:lines) out<<line<<endl;}

RECT rect(const POINT&pos,const SIZE&size)
{return RECT({pos.x,pos.y,pos.x+size.cx,pos.y+size.cy});}

SIZE size(LPARAM lParam)
{return SIZE({(short)LOWORD(lParam),(short)HIWORD(lParam)});}

SIZE size(const POINT&pos) {return SIZE({pos.x,pos.y});}

SIZE size(const RECT&rect) {return SIZE({width(rect),height(rect)});}

POINT sliding_offset(HWND window,const POINT&base)
{
    POINT cursorPos=cursor_pos(window);
    return POINT({cursorPos.x-base.x,cursorPos.y-base.y});
}

LONG squared_distance(const POINT&p,const POINT&q)
{
    LONG dx=p.x-q.x;
    LONG dy=p.y-q.y;
    return dx*dx+dy*dy;
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
