#include"nagarami.h"
#include<memory>
#include<windows.h>

namespace nagarami
{

shared_ptr<EndPaint> BeginPaint(HWND window,PAINTSTRUCT*paintStruct)
{
    HDC dc=::BeginPaint(window,paintStruct);
    if(dc==NULL) throw make_shared<api_error>("BeginPaint");
    return make_shared<EndPaint>(window,paintStruct,dc);
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
    (::BitBlt(destDC,destX,destY,width,height,srcDC,srcX,srcY,rop)==FALSE)
        throw make_shared<api_error>("BitBlt");
}

shared_ptr<DeleteObject> CreateCompatibleBitmap
(HDC destDC,int width,int height)
{
    HBITMAP bitmap=::CreateCompatibleBitmap(destDC,width,height);
    if(bitmap==NULL)
        throw make_shared<api_error>("CreateCompatibleBitmap");
    return make_shared<DeleteObject>((HGDIOBJ)bitmap);
}

shared_ptr<DeleteDC> CreateCompatibleDC(HDC dc)
{
    HDC compatibleDC=::CreateCompatibleDC(dc);
    if(compatibleDC==NULL)
        throw make_shared<api_error>("CreateCompatibleDC");
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
    HFONT font=::CreateFont
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
    if(font==NULL) throw make_shared<api_error>("CreateFont");
    return make_shared<DeleteObject>((HGDIOBJ)font);
}

shared_ptr<DeleteObject> CreatePatternBrush(HBITMAP bitmap)
{
    HBRUSH brush=::CreatePatternBrush(bitmap);
    if(brush==NULL) throw make_shared<api_error>("CreatePetternBrush");
    return make_shared<DeleteObject>((HGDIOBJ)brush);
}

shared_ptr<DeleteObject> CreatePen(int style,int width,COLORREF color)
{
    HPEN pen=::CreatePen(style,width,color);
    if(pen==NULL) throw make_shared<api_error>("CreatePen");
    return make_shared<DeleteObject>((HGDIOBJ)pen);
}

shared_ptr<DeleteObject> CreateSolidBrush(COLORREF color)
{
    HBRUSH brush=::CreateSolidBrush(color);
    if(brush==NULL) throw make_shared<api_error>("CreateSolidBrush");
    return make_shared<DeleteObject>((HGDIOBJ)brush);
}

int DrawText(HDC dc,LPCTSTR str,int count,LPRECT rect,UINT format)
{
    int height=::DrawText(dc,str,count,rect,format);
    if(height==0) throw make_shared<api_error>("DrawText");
    return height;
}

void Ellipse(HDC dc,int left,int top,int right,int bottom)
{
    if(::Ellipse(dc,left,top,right,bottom)==FALSE)
        throw make_shared<api_error>("Ellipse");
}

void FillRect(HDC dc,CONST RECT*rect,HBRUSH brush)
{
    if(::FillRect(dc,rect,brush)==FALSE)
        throw make_shared<api_error>("FillRect");
}

void GetClientRect(HWND window,LPRECT rect)
{
    if(::GetClientRect(window,rect)==FALSE)
        throw make_shared<api_error>("GetClientRect");
}

void GetCursorPos(LPPOINT point)
{
    if(::GetCursorPos(point)==FALSE)
        throw make_shared<api_error>("GetCursorPos");
}

shared_ptr<ReleaseDC> GetDC(HWND window)
{
    HDC dc=::GetDC(window);
    if(dc==NULL) throw make_shared<api_error>("GetDC");
    return make_shared<ReleaseDC>(window,dc);
}

bool GetMessage(LPMSG msg,HWND window,UINT first,UINT last)
{
    int result=::GetMessage(msg,window,first,last);
    if(result==-1) throw make_shared<api_error>("GetMessage");
    return result!=0;
}

int GetObject(HGDIOBJ object,int sizeOfBuffer,LPVOID buffer)
{
    int read=::GetObject(object,sizeOfBuffer,buffer);
    if(read==0) throw make_shared<api_error>("GetObject");
    return read;
}

int GetSystemMetrics(int index)
{
    int result=::GetSystemMetrics(index);
    if(result==0) throw make_shared<api_error>("GetSystemMetrics");
    return result;
}

void GetWindowPlacement(HWND window,WINDOWPLACEMENT*placement)
{
    if(::GetWindowPlacement(window,placement)==FALSE)
        throw make_shared<api_error>("GetWindowPlacement");
}

shared_ptr<DeleteObject> LoadBitmap(HINSTANCE instance,LPCTSTR name)
{
    HBITMAP bitmap=::LoadBitmap(instance,name);
    if(bitmap==NULL) throw make_shared<api_error>("LoadBitmap");
    return make_shared<DeleteObject>((HGDIOBJ)bitmap);
}

HCURSOR LoadCursor(HINSTANCE instance,LPCTSTR name)
{
    HCURSOR cursor=::LoadCursor(instance,name);
    if(cursor==NULL) throw make_shared<api_error>("LoadCursor");
    return cursor;
}

void RedrawWindow(HWND window,CONST RECT*rect,HRGN region,UINT flags)
{
    if(::RedrawWindow(window,rect,region,flags)==FALSE)
        throw make_shared<api_error>("RedrawWindow");
}

ATOM RegisterClassEx(CONST WNDCLASSEX*windowClass)
{
    ATOM atom=::RegisterClassEx(windowClass);
    if(atom==0) throw make_shared<api_error>("RegisterClassEx");
    return atom;
}

void ScreenToClient(HWND window,LPPOINT point)
{
    if(::ScreenToClient(window,point)==FALSE)
        throw make_shared<api_error>("ScreenToClient");
}

int SetBkMode(HDC dc,int mode)
{
    int oldMode=::SetBkMode(dc,mode);
    if(oldMode==0) throw make_shared<api_error>("SetBkMode");
    return oldMode;
}

void SetBrushOrgEx(HDC dc,int x,int y,LPPOINT oldPoint)
{
    if(::SetBrushOrgEx(dc,x,y,oldPoint)==FALSE)
        throw make_shared<api_error>("SetBrushOrgEx");
}

void SetForegroundWindow(HWND window)
{
    if(::SetForegroundWindow(window)==FALSE)
        throw make_shared<api_error>("SetForegroundWindow");
}

void SetLayeredWindowAttributes
(HWND window,COLORREF key,BYTE alpha,DWORD flags)
{
    if(::SetLayeredWindowAttributes(window,key,alpha,flags)==FALSE)
        throw make_shared<api_error>("SetLayeredWindowAttributes");
}

int SetStretchBltMode(HDC dc,int mode)
{
    int oldMode=::SetStretchBltMode(dc,mode);
    if(oldMode==0) throw make_shared<api_error>("SetStretchBltMode");
    return oldMode;
}

COLORREF SetTextColor(HDC dc,COLORREF color)
{
    COLORREF oldColor=::SetTextColor(dc,color);
    if(oldColor==CLR_INVALID)
        throw make_shared<api_error>("SetTextColor");
    return oldColor;
}

void SetWindowPos
(HWND window,HWND after,int x,int y,int width,int height,UINT flags)
{
    if(::SetWindowPos(window,after,x,y,width,height,flags)==FALSE)
        throw make_shared<api_error>("SetWindowPos");
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
        ::StretchBlt
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
    ) throw make_shared<api_error>("StretchBlt");
}

}
