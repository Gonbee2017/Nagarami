#include<algorithm>
#include<sstream>
#include"nagaramitest.h"

namespace nm
{

const vector<string>&call::arguments() const {return arguments_;}

string call::name() const {return name_;}

bool call::operator!=(const call&rhs) const {return !(*this==rhs);}

bool call::operator==(const call&rhs) const
{
    bool result=true;
    if(name_!=rhs.name_) result=false;
    else if(arguments_.size()!=rhs.arguments_.size()) result=false;
    else
    {
        for(size_t i=0;i<arguments_.size();i++)
        {
            if
            (
                arguments_.at(i)!=OMIT_ARGUMENT&&
                rhs.arguments_.at(i)!=OMIT_ARGUMENT&&
                arguments_.at(i)!=rhs.arguments_.at(i)
            )
            {
                result=false;
                break;
            }
        }
    }
    return result;
}

ostream&operator<<(ostream&os,const call&call_)
{
    ostringstream oss;
    oss<<call_.name_;
    if(!call_.arguments_.empty())
    {
        oss<<':';
        for(size_t i=0;i<call_.arguments_.size();i++)
        {
            if(i!=0) oss<<',';
            oss<<call_.arguments_.at(i);
        }
    }
    return os<<oss.str();
}

const vector<call>&logger::history() const {return history_;}

size_t logger::count(const string&name) const
{
    return count_if
    (
        history_.begin(),
        history_.end(),
        [&name] (const call&call_)->bool {return call_.name()==name;}
    );
}

void describe_each_to(vector<string>&strs) {}

bool operator!=(const POINT&lhs,const POINT&rhs) {return !(lhs==rhs);}

bool operator!=(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs)
{return !(lhs==rhs);}

bool operator!=(const RECT&lhs,const RECT&rhs) {return !(lhs==rhs);}

bool operator!=(const SIZE&lhs,const SIZE&rhs) {return !(lhs==rhs);}

ostream&operator<<(ostream&os,const BITMAP&bitmap)
{
    return os<<describe(
    "{",
        describe_with
        (
            ",",
            bitmap.bmType,
            bitmap.bmWidth,
            bitmap.bmHeight,
            bitmap.bmWidthBytes,
            bitmap.bmPlanes,
            bitmap.bmBitsPixel,
            bitmap.bmBits
        ),
    "}");
}

ostream&operator<<(ostream&os,const BITMAP*const bitmap)
{
    if(bitmap) os<<*bitmap;
    return os;
}

ostream&operator<<(ostream&os,const MSG&message)
{
    return os<<describe(
    "{",
        describe_with
        (
            ",",
            message.hwnd,
            message.message,
            message.wParam,
            message.lParam,
            message.time,
            message.pt
        ),
    "}");
}

ostream&operator<<(ostream&os,const MSG*const message)
{
    if(message) os<<*message;
    return os;
}

ostream&operator<<(ostream&os,const PAINTSTRUCT&paint)
{
    return os<<describe(
    "{",
        describe_with
        (
            ",",
            paint.hdc,
            paint.fErase,
            paint.rcPaint,
            paint.fRestore,
            paint.fIncUpdate
        ),
    "}");
}

ostream&operator<<(ostream&os,const PAINTSTRUCT*const paint)
{
    if(paint) os<<*paint;
    return os;
}

ostream&operator<<(ostream&os,const POINT&point)
{return os<<describe("{",describe_with(",",point.x,point.y),"}");}

ostream&operator<<(ostream&os,const POINT*const point)
{
    if(point) os<<*point;
    return os;
}

ostream&operator<<(ostream&os,const POINT_DOUBLE&point)
{return os<<describe("{",describe_with(",",point.x,point.y),"}");}

ostream&operator<<(ostream&os,const RECT&rect)
{
    return os<<describe(
    "{",describe_with
        (",",rect.left,rect.top,rect.right,rect.bottom),
    "}");
}

ostream&operator<<(ostream&os,const RECT*rect)
{
    if(rect) os<<*rect;
    return os;
}

ostream&operator<<(ostream&os,const SIZE&size)
{return os<<describe("{",describe_with(",",size.cx,size.cy),"}");}

ostream&operator<<(ostream&os,const TIMECAPS&caps)
{
     return os<<describe
     ("{",describe_with(",",caps.wPeriodMin,caps.wPeriodMax),"}");
}

ostream&operator<<(ostream&os,const TIMECAPS*const caps)
{
    if(caps) os<<*caps;
    return os;
}

ostream&operator<<(ostream&os,const TOOLINFOW&toolInfo)
{
     return os<<describe(
    "{",describe_with
        (
            ",",
            toolInfo.cbSize,
            toolInfo.uFlags,
            toolInfo.hwnd,
            toolInfo.uId,
            toolInfo.rect,
            toolInfo.hinst,
            toolInfo.lpszText,
            toolInfo.lParam,
            toolInfo.lpReserved
        ),
    "}");
}

ostream&operator<<(ostream&os,const TOOLINFOW*toolInfo)
{
    if(toolInfo) os<<*toolInfo;
    return os;
}

ostream&operator<<(ostream&os,const WNDCLASSEX&clazz)
{
     return os<<describe(
    "{",describe_with
        (
            ",",
            clazz.cbSize,
            clazz.style,
            clazz.lpfnWndProc,
            clazz.cbClsExtra,
            clazz.cbWndExtra,
            clazz.hInstance,
            clazz.hIcon,
            clazz.hCursor,
            clazz.hbrBackground,
            clazz.lpszMenuName,
            clazz.lpszClassName,
            clazz.hIconSm
        ),
    "}");
}

ostream&operator<<(ostream&os,const WNDCLASSEX*clazz)
{
    if(clazz) os<<*clazz;
    return os;
}

ostream&operator<<(ostream&os,const WINDOWPLACEMENT&placement)
{
    return os<<describe(
    "{",describe_with
        (
            ",",
            placement.length,
            placement.flags,
            placement.showCmd,
            placement.ptMinPosition,
            placement.ptMaxPosition,
            placement.rcNormalPosition
        ),
    "}");
}

ostream&operator<<(ostream&os,const WINDOWPLACEMENT*const placement)
{
    if(placement) os<<*placement;
    return os;
}

ostream&operator<<(ostream&os,const char*const ascii)
{
    string str;
    if(ascii) str=string(ascii);
    else str=string(NULL_STRING);
    return os<<str;
}

ostream&operator<<(ostream&os,const wchar_t*const ascii)
{
    shared_ptr<char> buffer;
    if(ascii)
    {
        int length=WideCharToMultiByte
        (CP_UTF8,0,ascii,-1,NULL,0,NULL,NULL);
        if(length==0) throw api_error("WideCharToMultiByte");
        buffer=shared_ptr<char>(new char[length],default_delete<char[]>());
        if
        (
            WideCharToMultiByte
            (CP_UTF8,0,ascii,-1,buffer.get(),length,NULL,NULL)!=length
        ) throw api_error("WideCharToMultiByte");
    }
    return os<<buffer.get();
}

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

}
