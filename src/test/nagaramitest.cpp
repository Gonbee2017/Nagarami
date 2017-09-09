#include<algorithm>
#include<sstream>
#include"nagaramitest.h"

namespace nm
{

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

logger::~logger()
{
    ct().error=runtime_error("");
    pt().clear();
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

bool operator!=(const POINT&lhs,const POINT&rhs) {return !(lhs==rhs);}

bool operator!=(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs)
{return !(lhs==rhs);}

bool operator!=(const RECT&lhs,const RECT&rhs) {return !(lhs==rhs);}

bool operator!=(const SIZE&lhs,const SIZE&rhs) {return !(lhs==rhs);}

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

ostream&operator<<(ostream&os,const WNDCLASSEX&clazz)
{
     return os<<describe(
    "{",describe_with
        (
            ",",
            clazz.cbSize,
            clazz.style,
            (const void*)clazz.lpfnWndProc,
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
