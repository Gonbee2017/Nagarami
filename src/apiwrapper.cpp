#include"nagarami.h"
#include<memory>
#include<windows.h>

namespace nm
{

shared_ptr<EndPaint> BeginPaint(HWND window,PAINTSTRUCT*paint)
{
    fill(paint,0);
    const HDC dc=pt->BeginPaint(window,paint);
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
        pt->BitBlt(destDC,destX,destY,width,height,srcDC,srcX,srcY,rop)
            ==FALSE
    ) throw api_error("BitBlt");
}

shared_ptr<DeleteObject> CreateCompatibleBitmap
(HDC destDC,int width,int height)
{
    const HBITMAP bitmap=pt->CreateCompatibleBitmap(destDC,width,height);
    if(bitmap==NULL) throw api_error("CreateCompatibleBitmap");
    return make_shared<DeleteObject>((HGDIOBJ)bitmap);
}

shared_ptr<DeleteDC> CreateCompatibleDC(HDC dc)
{
    const HDC compatibleDC=pt->CreateCompatibleDC(dc);
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
    const HFONT font=pt->CreateFont
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
    const HBRUSH brush=pt->CreatePatternBrush(bitmap);
    if(brush==NULL) throw api_error("CreatePatternBrush");
    return make_shared<DeleteObject>((HGDIOBJ)brush);
}

shared_ptr<DeleteObject> CreatePen(int style,int width,COLORREF color)
{
    const HPEN pen=pt->CreatePen(style,width,color);
    if(pen==NULL) throw api_error("CreatePen");
    return make_shared<DeleteObject>((HGDIOBJ)pen);
}

shared_ptr<DeleteObject> CreateSolidBrush(COLORREF color)
{
    const HBRUSH brush=pt->CreateSolidBrush(color);
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
    const HWND window=pt->CreateWindowEx
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
    if(*ct->error.what()) throw ct->error;
    if(window==NULL) throw api_error("CreateWindowEx");
    return window;
}

LRESULT DispatchMessage(CONST MSG*message)
{
    const LRESULT result=pt->DispatchMessage(message);
    if(*ct->error.what()) throw ct->error;
    return result;
}

int DrawText(HDC dc,LPCTSTR str,int count,LPRECT rect,UINT format)
{
    const int height=pt->DrawText(dc,str,count,rect,format);
    if(height==0) throw api_error("DrawText");
    return height;
}

void Ellipse(HDC dc,int left,int top,int right,int bottom)
{
    if(pt->Ellipse(dc,left,top,right,bottom)==FALSE)
        throw api_error("Ellipse");
}

void FillRect(HDC dc,CONST RECT*rect,HBRUSH brush)
{if(pt->FillRect(dc,rect,brush)==FALSE) throw api_error("FillRect");}

void GetClientRect(HWND window,LPRECT rect)
{
    fill(rect,0);
    if(pt->GetClientRect(window,rect)==FALSE)
        throw api_error("GetClientRect");
}

void GetCursorPos(LPPOINT pos)
{
    static POINT lastPos = POINT({0,0});
    fill(pos,0);
    if(pt->GetCursorPos(pos)==FALSE) {
        if (GetLastError() == 5) *pos = lastPos;
        else throw api_error("GetCursorPos");
    }
    lastPos = *pos;
}

shared_ptr<ReleaseDC> GetDC(HWND window)
{
    const HDC dc=pt->GetDC(window);
    if(dc==NULL) throw api_error("GetDC");
    return make_shared<ReleaseDC>(window,dc);
}

bool GetMessage(LPMSG message,HWND window,UINT first,UINT last)
{
    fill(message,0);
    int result=pt->GetMessage(message,window,first,last);
    if(result==-1) throw api_error("GetMessage");
    return result!=0;
}

COLORREF GetPixel(HDC dc,int x,int y)
{
    const COLORREF color=pt->GetPixel(dc,x,y);
    if(color==CLR_INVALID) throw api_error("GetPixel");
    return color;
}

int GetObject(HGDIOBJ object,int sizeOfBuffer,LPVOID buffer)
{
    const int got=pt->GetObject(object,sizeOfBuffer,buffer);
    if(got==0) throw api_error("GetObject");
    return got;
}

int GetSystemMetrics(int index)
{
    const int metrics=pt->GetSystemMetrics(index);
    if(metrics==0) throw api_error("GetSystemMetrics");
    return metrics;
}

void GetWindowPlacement(HWND window,WINDOWPLACEMENT*placement)
{
    fill(placement,0);
    placement->length=sizeof(WINDOWPLACEMENT);
    if(pt->GetWindowPlacement(window,placement)==FALSE)
        throw api_error("GetWindowPlacement");
}

shared_ptr<DeleteObject> LoadBitmap(HINSTANCE instance,LPCTSTR name)
{
    const HBITMAP bitmap=pt->LoadBitmap(instance,name);
    if(bitmap==NULL) throw api_error("LoadBitmap");
    return make_shared<DeleteObject>((HGDIOBJ)bitmap);
}

HCURSOR LoadCursor(HINSTANCE instance,LPCTSTR name)
{
    const HCURSOR cursor=pt->LoadCursor(instance,name);
    if(cursor==NULL) throw api_error("LoadCursor");
    return cursor;
}

void PostMessage(HWND window,UINT message,WPARAM wParam,LPARAM lParam)
{
    if(pt->PostMessage(window,message,wParam,lParam)==FALSE)
        throw api_error("PostMessage");
}

void RedrawWindow(HWND window,CONST RECT*rect,HRGN region,UINT flags)
{
    if(pt->RedrawWindow(window,rect,region,flags)==FALSE)
        throw api_error("RedrawWindow");
}

ATOM RegisterClassEx(CONST WNDCLASSEX*windowClass)
{
    const ATOM atom=pt->RegisterClassEx(windowClass);
    if(atom==0) throw api_error("RegisterClassEx");
    return atom;
}

void ReleaseCapture()
{if(pt->ReleaseCapture()==FALSE) throw api_error("ReleaseCapture");}

void ScreenToClient(HWND window,LPPOINT pos)
{
    if(pt->ScreenToClient(window,pos)==FALSE)
        throw api_error("ScreenToClient");
}

int SetBkMode(HDC dc,int mode)
{
    const int oldMode=pt->SetBkMode(dc,mode);
    if(oldMode==0) throw api_error("SetBkMode");
    return oldMode;
}

void SetBrushOrgEx(HDC dc,int x,int y,LPPOINT oldPos)
{
    if(oldPos) fill(oldPos,0);
    if(pt->SetBrushOrgEx(dc,x,y,oldPos)==FALSE)
        throw api_error("SetBrushOrgEx");
}

void SetForegroundWindow(HWND window)
{
    if(pt->SetForegroundWindow(window)==FALSE)
        throw api_error("SetForegroundWindow");
}

void SetLayeredWindowAttributes
(HWND window,COLORREF key,BYTE alpha,DWORD flags)
{
    if(pt->SetLayeredWindowAttributes(window,key,alpha,flags)==FALSE)
        throw api_error("SetLayeredWindowAttributes");
}

int SetStretchBltMode(HDC dc,int mode)
{
    const int oldMode=pt->SetStretchBltMode(dc,mode);
    if(oldMode==0) throw api_error("SetStretchBltMode");
    return oldMode;
}

COLORREF SetTextColor(HDC dc,COLORREF color)
{
    const COLORREF oldColor=pt->SetTextColor(dc,color);
    if(oldColor==CLR_INVALID) throw api_error("SetTextColor");
    return oldColor;
}

HINSTANCE ShellExecute
(
    HWND parent,
    LPCTSTR verb,
    LPCTSTR file,
    LPCTSTR parameters,
    LPCTSTR directory,
    INT showCommand
)
{
    const HINSTANCE instance=pt->ShellExecute
    (parent,verb,file,parameters,directory,showCommand);
    const DWORD errorCode=(DWORD)instance;
    if(errorCode<=32) throw api_error("ShellExecute",errorCode);
    return instance;
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
        pt->StretchBlt
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

shared_ptr<TimeEndPeriod> timeBeginPeriod(UINT period)
{
    const MMRESULT result=pt->timeBeginPeriod(period);
    if(result!=TIMERR_NOERROR) throw api_error("timeBeginPeriod",result);
    return make_shared<TimeEndPeriod>(period);
}

void timeGetDevCaps(LPTIMECAPS caps,UINT sizeOfCaps)
{
    fill(caps,0);
    const MMRESULT result=pt->timeGetDevCaps(caps,sizeOfCaps);
    if(result!=TIMERR_NOERROR) throw api_error("timeGetDevCaps",result);
}

shared_ptr<TimeKillEvent> timeSetEvent
(
    UINT delay,
    UINT resolution,
    LPTIMECALLBACK procedure,
    DWORD user,
    UINT event
)
{
    const UINT timerID=
        pt->timeSetEvent(delay,resolution,procedure,user,event);
    if(timerID==0) throw api_error("timeSetEvent",0);
    return make_shared<TimeKillEvent>(timerID);
}

}
