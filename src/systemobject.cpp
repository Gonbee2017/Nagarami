#include"nagarami.h"
#include<memory>
#include<stdexcept>
#include<windows.h>

namespace nm
{

DeleteDC::DeleteDC(HDC handle):
    Finalizer([handle] {pt().DeleteDC(handle);}),handle_(handle) {}

HDC DeleteDC::handle() {return handle_;}

DeleteObject::DeleteObject(HGDIOBJ handle):
    Finalizer([handle] {pt().DeleteObject(handle);}),handle_(handle) {}

HGDIOBJ DeleteObject::handle() {return handle_;}

EndPaint::EndPaint(HWND window,PAINTSTRUCT*paint,HDC handle):
    Finalizer([window,paint] {pt().EndPaint(window,paint);}),
    handle_(handle) {}

HDC EndPaint::handle() {return handle_;}

ReleaseDC::ReleaseDC(HWND window,HDC handle):
    Finalizer([window,handle] {pt().ReleaseDC(window,handle);}),
    handle_(handle) {}

HDC ReleaseDC::handle() {return handle_;}

TimeEndPeriod::TimeEndPeriod(UINT period):
    Finalizer([period] {pt().timeEndPeriod(period);}) {}

TimeKillEvent::TimeKillEvent(UINT timerID):
    Finalizer([timerID] {pt().timeKillEvent(timerID);}) {}

HBITMAP Buffer::bitmap() {return (HBITMAP)bitmap_->handle();}

shared_ptr<Buffer> Buffer::create(const SIZE&size,HDC destDC)
{
    auto bitmap=nm::CreateCompatibleBitmap(destDC,size.cx,size.cy);
    auto dc=nm::CreateCompatibleDC(destDC);
    pt().SelectObject(dc->handle(),bitmap->handle());
    return shared_ptr<Buffer>(new Buffer(bitmap,dc,size));
}

HDC Buffer::dc() {return dc_->handle();}

shared_ptr<Buffer> Buffer::load(HINSTANCE instance,LPCTSTR name,HDC destDC)
{
    auto bitmap=nm::LoadBitmap(instance,name);
    BITMAP bmp;
    nm::GetObject((HGDIOBJ)bitmap->handle(),sizeof(BITMAP),&bmp);
    auto dc=nm::CreateCompatibleDC(destDC);
    pt().SelectObject(dc->handle(),bitmap->handle());
    return shared_ptr<Buffer>
    (new Buffer(bitmap,dc,SIZE({bmp.bmWidth,bmp.bmHeight})));
}

const SIZE&Buffer::size() {return size_;}

Buffer::Buffer
(
    const shared_ptr<DeleteObject>&bitmap,
    const shared_ptr<DeleteDC>&dc,
    const SIZE&size
):bitmap_(bitmap),dc_(dc),size_(size) {}

Timer::Timer(UINT delay,HWND dest):delay_(delay),dest_(dest)
{
    static const auto beginPeriod=make_shared<Initializer>([]
    {
        TIMECAPS caps;
        nm::timeGetDevCaps(&caps,sizeof(TIMECAPS));
        period_=caps.wPeriodMin;
        static const auto endPeriod=nm::timeBeginPeriod(period_);
    });
    killEvent_=nm::timeSetEvent
    (
        delay_,
        period_,
        &procedure,
        (DWORD)dest_,
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
) {PostMessage((HWND)user,WM_USERTIMER,0,0);}

UINT Timer::period_;

shared_ptr<TimeEndPeriod> timeBeginPeriod(UINT period)
{
    MMRESULT mmr;
    if((mmr=pt().timeBeginPeriod(period))!=TIMERR_NOERROR)
        throw runtime_error(describe("timeBeginPeriod failed.(",mmr,")"));
    return make_shared<TimeEndPeriod>(period);
}

void timeGetDevCaps(LPTIMECAPS caps,UINT sizeOfCaps)
{
    MMRESULT mmr;
    if((mmr=pt().timeGetDevCaps(caps,sizeOfCaps)!=TIMERR_NOERROR))
        throw runtime_error(describe("timeGetDevCaps failed.(",mmr,")"));
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
    UINT timer=pt().timeSetEvent(delay,resolution,procedure,user,event);
    if(timer==0) throw runtime_error(describe("timeSetEvent failed."));
    return make_shared<TimeKillEvent>(timer);
}

}
