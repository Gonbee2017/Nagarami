#include"nagarami.h"
#include<memory>
#include<windows.h>

namespace nm
{

shared_ptr<EndPaint> BeginPaint(HWND window,PAINTSTRUCT*paint)
{
    HDC dc=pt().BeginPaint(window,paint);
    if(dc==NULL) throw api_error("BeginPaint");
    return make_shared<EndPaint>(window,paint,dc);
}

void BitBlt
(
    HDC destDC,
    int destX,
    int destY,
    int width,
    int height,
    HDC srcDC,
    int srcX,
    int srcY,
    DWORD rop
)
{
    if
    (
        pt().BitBlt(destDC,destX,destY,width,height,srcDC,srcX,srcY,rop)
            ==FALSE
    ) throw api_error("BitBlt");
}

shared_ptr<DeleteObject> CreateCompatibleBitmap
(HDC destDC,int width,int height)
{
    HBITMAP bitmap=pt().CreateCompatibleBitmap(destDC,width,height);
    if(bitmap==NULL) throw api_error("CreateCompatibleBitmap");
    return make_shared<DeleteObject>((HGDIOBJ)bitmap);
}

shared_ptr<DeleteDC> CreateCompatibleDC(HDC dc)
{
    HDC compatibleDC=pt().CreateCompatibleDC(dc);
    if(compatibleDC==NULL) throw api_error("CreateCompatibleDC");
    return make_shared<DeleteDC>(compatibleDC);
}

shared_ptr<DeleteObject> CreateFont
(
    int height,
    int width,
    int escapement,
    int orientation,
    int weight,
    DWORD italic,
    DWORD underline,
    DWORD strikeOut,
    DWORD charSet,
    DWORD outputPrecision,
    DWORD clipPrecision,
    DWORD quality,
    DWORD pitchAndFamily,
    LPCTSTR face
)
{
    HFONT font=pt().CreateFont
    (
        height,
        width,
        escapement,
        orientation,
        weight,
        italic,
        underline,
        strikeOut,
        charSet,
        outputPrecision,
        clipPrecision,
        quality,
        pitchAndFamily,
        face
    );
    if(font==NULL) throw api_error("CreateFont");
    return make_shared<DeleteObject>((HGDIOBJ)font);
}

shared_ptr<DeleteObject> CreatePatternBrush(HBITMAP bitmap)
{
    HBRUSH brush=pt().CreatePatternBrush(bitmap);
    if(brush==NULL) throw api_error("CreatePetternBrush");
    return make_shared<DeleteObject>((HGDIOBJ)brush);
}

shared_ptr<DeleteObject> CreatePen(int style,int width,COLORREF color)
{
    HPEN pen=pt().CreatePen(style,width,color);
    if(pen==NULL) throw api_error("CreatePen");
    return make_shared<DeleteObject>((HGDIOBJ)pen);
}

shared_ptr<DeleteObject> CreateSolidBrush(COLORREF color)
{
    HBRUSH brush=pt().CreateSolidBrush(color);
    if(brush==NULL) throw api_error("CreateSolidBrush");
    return make_shared<DeleteObject>((HGDIOBJ)brush);
}

HWND CreateWindowEx
(
    DWORD exStyle,
    LPCTSTR className,
    LPCTSTR windowName,
    DWORD style,
    int x,
    int y,
    int width,
    int height,
    HWND parent,
    HMENU menu,
    HINSTANCE instance,
    LPVOID param
)
{
    HWND handle=pt().CreateWindowEx
    (
        exStyle,
        className,
        windowName,
        style,
        x,
        y,
        width,
        height,
        parent,
        menu,
        instance,
        param
    );
    if(*ct().error.what()) throw ct().error;
    if(handle==NULL) throw api_error("CreateWindowEx");
    return handle;
}

void DispatchMessage(CONST MSG*message)
{
    pt().DispatchMessage(message);
    if(*ct().error.what()) throw ct().error;
}

int DrawText(HDC dc,LPCTSTR str,int count,LPRECT rect,UINT format)
{
    int height=pt().DrawText(dc,str,count,rect,format);
    if(height==0) throw api_error("DrawText");
    return height;
}

void Ellipse(HDC dc,int left,int top,int right,int bottom)
{
    if(pt().Ellipse(dc,left,top,right,bottom)==FALSE)
        throw api_error("Ellipse");
}

void FillRect(HDC dc,CONST RECT*rect,HBRUSH brush)
{if(pt().FillRect(dc,rect,brush)==FALSE) throw api_error("FillRect");}

void GetClientRect(HWND window,LPRECT rect)
{
    if(pt().GetClientRect(window,rect)==FALSE)
    throw api_error("GetClientRect");
}

void GetCursorPos(LPPOINT point)
{if(pt().GetCursorPos(point)==FALSE) throw api_error("GetCursorPos");}

shared_ptr<ReleaseDC> GetDC(HWND window)
{
    HDC dc=pt().GetDC(window);
    if(dc==NULL) throw api_error("GetDC");
    return make_shared<ReleaseDC>(window,dc);
}

bool GetMessage(LPMSG msg,HWND window,UINT first,UINT last)
{
    int result=pt().GetMessage(msg,window,first,last);
    if(result==-1) throw api_error("GetMessage");
    return result!=0;
}

int GetObject(HGDIOBJ object,int sizeOfBuffer,LPVOID buffer)
{
    int read=pt().GetObject(object,sizeOfBuffer,buffer);
    if(read==0) throw api_error("GetObject");
    return read;
}

int GetSystemMetrics(int index)
{
    int result=pt().GetSystemMetrics(index);
    if(result==0) throw api_error("GetSystemMetrics");
    return result;
}

void GetWindowPlacement(HWND window,WINDOWPLACEMENT*placement)
{
    if(pt().GetWindowPlacement(window,placement)==FALSE)
        throw api_error("GetWindowPlacement");
}

shared_ptr<DeleteObject> LoadBitmap(HINSTANCE instance,LPCTSTR name)
{
    HBITMAP bitmap=pt().LoadBitmap(instance,name);
    if(bitmap==NULL) throw api_error("LoadBitmap");
    return make_shared<DeleteObject>((HGDIOBJ)bitmap);
}

HCURSOR LoadCursor(HINSTANCE instance,LPCTSTR name)
{
    HCURSOR cursor=pt().LoadCursor(instance,name);
    if(cursor==NULL) throw api_error("LoadCursor");
    return cursor;
}

void RedrawWindow(HWND window,CONST RECT*rect,HRGN region,UINT flags)
{
    if(pt().RedrawWindow(window,rect,region,flags)==FALSE)
        throw api_error("RedrawWindow");
}

ATOM RegisterClassEx(CONST WNDCLASSEX*windowClass)
{
    ATOM atom=pt().RegisterClassEx(windowClass);
    if(atom==0) throw api_error("RegisterClassEx");
    return atom;
}

void ReleaseCapture()
{if(pt().ReleaseCapture()==FALSE) throw api_error("ReleaseCapture");}

void ScreenToClient(HWND window,LPPOINT point)
{
    if(pt().ScreenToClient(window,point)==FALSE)
        throw api_error("ScreenToClient");
}

int SetBkMode(HDC dc,int mode)
{
    int oldMode=pt().SetBkMode(dc,mode);
    if(oldMode==0) throw api_error("SetBkMode");
    return oldMode;
}

void SetBrushOrgEx(HDC dc,int x,int y,LPPOINT oldPoint)
{
    if(pt().SetBrushOrgEx(dc,x,y,oldPoint)==FALSE)
        throw api_error("SetBrushOrgEx");
}

void SetForegroundWindow(HWND window)
{
    if(pt().SetForegroundWindow(window)==FALSE)
        throw api_error("SetForegroundWindow");
}

void SetLayeredWindowAttributes
(HWND window,COLORREF key,BYTE alpha,DWORD flags)
{
    if(pt().SetLayeredWindowAttributes(window,key,alpha,flags)==FALSE)
        throw api_error("SetLayeredWindowAttributes");
}

int SetStretchBltMode(HDC dc,int mode)
{
    int oldMode=pt().SetStretchBltMode(dc,mode);
    if(oldMode==0) throw api_error("SetStretchBltMode");
    return oldMode;
}

COLORREF SetTextColor(HDC dc,COLORREF color)
{
    COLORREF oldColor=pt().SetTextColor(dc,color);
    if(oldColor==CLR_INVALID) throw api_error("SetTextColor");
    return oldColor;
}

void StretchBlt
(
    HDC destDC,
    int destX,
    int destY,
    int destWidth,
    int destHeight,
    HDC srcDC,
    int srcX,
    int srcY,
    int srcWidth,
    int srcHeight,
    DWORD rop
)
{
    if
    (
        pt().StretchBlt
        (
            destDC,
            destX,
            destY,
            destWidth,
            destHeight,
            srcDC,
            srcX,
            srcY,
            srcWidth,
            srcHeight,
            rop
        )==FALSE
    ) throw api_error("StretchBlt");
}

}
