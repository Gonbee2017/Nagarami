#include"nagarami.h"
#include<memory>
#include<stdexcept>
#include<windows.h>

namespace nm
{

DeleteDC::DeleteDC(HDC handle):
    Finalizer([handle] {pt->DeleteDC(handle);}),handle_(handle) {}

HDC DeleteDC::handle() const {return handle_;}

DeleteObject::DeleteObject(HGDIOBJ handle):
    Finalizer([handle] {pt->DeleteObject(handle);}),handle_(handle) {}

HGDIOBJ DeleteObject::handle() const {return handle_;}

HBITMAP Buffer::bitmap() const {return (HBITMAP)bitmap_->handle();}

shared_ptr<Buffer> Buffer::create(const SIZE&size,HDC destDC)
{
    auto bitmap=nm::CreateCompatibleBitmap(destDC,size.cx,size.cy);
    auto dc=nm::CreateCompatibleDC(destDC);
    pt->SelectObject(dc->handle(),bitmap->handle());
    return shared_ptr<Buffer>(new Buffer(bitmap,dc,size));
}

HDC Buffer::dc() const {return dc_->handle();}

shared_ptr<Buffer> Buffer::load(HINSTANCE instance,LPCTSTR name,HDC destDC)
{
    auto bitmap=nm::LoadBitmap(instance,name);
    BITMAP bmp;
    fill(&bmp,0);
    nm::GetObject((HGDIOBJ)bitmap->handle(),sizeof(BITMAP),&bmp);
    auto dc=nm::CreateCompatibleDC(destDC);
    pt->SelectObject(dc->handle(),bitmap->handle());
    return shared_ptr<Buffer>
    (new Buffer(bitmap,dc,SIZE({bmp.bmWidth,bmp.bmHeight})));
}

const SIZE&Buffer::size() const {return size_;}

Buffer::Buffer
(
    const shared_ptr<DeleteObject>&bitmap,
    const shared_ptr<DeleteDC>&dc,
    const SIZE&size
):bitmap_(bitmap),dc_(dc),size_(size) {}

EndPaint::EndPaint(HWND window,PAINTSTRUCT*paint,HDC handle):
    Finalizer([window,paint] {pt->EndPaint(window,paint);}),
    handle_(handle) {}

HDC EndPaint::handle() const {return handle_;}

ReleaseDC::ReleaseDC(HWND window,HDC handle):
    Finalizer([window,handle] {pt->ReleaseDC(window,handle);}),
    handle_(handle) {}

HDC ReleaseDC::handle() const {return handle_;}

TimeEndPeriod::TimeEndPeriod(UINT value):
    Finalizer([value] {pt->timeEndPeriod(value);}),value_(value) {}

UINT TimeEndPeriod::value() const {return value_;}

TimeKillEvent::TimeKillEvent(UINT timerID):
    Finalizer([timerID] {pt->timeKillEvent(timerID);}) {}

Timer::Timer(UINT delay,UINT resolution,HWND dest)
{
    event_=nm::timeSetEvent
    (
        delay,
        resolution,
        &procedure,
        (DWORD)dest,
        TIME_CALLBACK_FUNCTION|TIME_KILL_SYNCHRONOUS|TIME_PERIODIC
    );
}

void CALLBACK Timer::procedure
(
    UINT timerID,
    UINT message,
    DWORD_PTR user,
    DWORD_PTR reserved1,
    DWORD_PTR reserved2
) {try{nm::PostMessage((HWND)user,WM_USERTIMER,0,0);} catch(...) {}}

}
