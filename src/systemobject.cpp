#include"nagarami.h"
#include<memory>
#include<stdexcept>
#include<windows.h>

namespace nagarami
{

DeleteDC::DeleteDC(HDC handle):
    Finalizer([handle] () {::DeleteDC(handle);}),handle_(handle) {}

HDC DeleteDC::handle() {return handle_;}

DeleteObject::DeleteObject(HGDIOBJ handle):
    Finalizer([handle] () {::DeleteObject(handle);}),
    handle_(handle) {}

HGDIOBJ DeleteObject::handle() {return handle_;}

EndPaint::EndPaint(HWND window,PAINTSTRUCT*paint,HDC handle):
    Finalizer([window,paint] () {::EndPaint(window,paint);}),
    handle_(handle) {}

HDC EndPaint::handle() {return handle_;}

ReleaseDC::ReleaseDC(HWND window,HDC handle):
    Finalizer([window,handle] () {::ReleaseDC(window,handle);}),
    handle_(handle) {}

HDC ReleaseDC::handle() {return handle_;}

TimeEndPeriod::TimeEndPeriod(UINT resolution):
    Finalizer([resolution] () {::timeEndPeriod(resolution);}) {}

TimeKillEvent::TimeKillEvent(UINT timer):
    Finalizer([timer] () {::timeKillEvent(timer);}) {}

HBITMAP Buffer::bitmap() {return (HBITMAP)bitmap_->handle();}

shared_ptr<Buffer> Buffer::create(const SIZE&size,HDC destDC)
{
    auto bitmap=nagarami::CreateCompatibleBitmap
    (destDC,size.cx,size.cy);
    auto dc=nagarami::CreateCompatibleDC(destDC);
    SelectObject(dc->handle(),bitmap->handle());
    return shared_ptr<Buffer>(new Buffer(bitmap,dc,size));
}

HDC Buffer::dc() {return dc_->handle();}

shared_ptr<Buffer> Buffer::load(HINSTANCE instance,LPCTSTR name,HDC destDC)
{
    auto bitmap=nagarami::LoadBitmap(instance,name);
    BITMAP bmp;
    nagarami::GetObject((HGDIOBJ)bitmap->handle(),sizeof(BITMAP),&bmp);
    auto dc=nagarami::CreateCompatibleDC(destDC);
    SelectObject(dc->handle(),bitmap->handle());
    return shared_ptr<Buffer>(new Buffer
    (bitmap,dc,SIZE({bmp.bmWidth,bmp.bmHeight})));
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
    static const auto begin=make_shared<Initializer>([] ()
    {
        TIMECAPS caps;
        nagarami::timeGetDevCaps(&caps,sizeof(TIMECAPS));
        resolution_=caps.wPeriodMin;
        static const auto end=nagarami::timeBeginPeriod(resolution_);
    });
    kill_=nagarami::timeSetEvent
    (
        delay_,
        resolution_,
        &timeProcedure,
        (DWORD)dest_,
        TIME_CALLBACK_FUNCTION|TIME_KILL_SYNCHRONOUS|TIME_PERIODIC
    );
}

void CALLBACK Timer::timeProcedure
(
    UINT timerID,
    UINT message,
    DWORD_PTR user,
    DWORD_PTR reserved1,
    DWORD_PTR reserved2
) {PostMessage((HWND)user,WM_USERTIMER,0,0);}

UINT Timer::resolution_;

shared_ptr<TimeEndPeriod> timeBeginPeriod(UINT resolution)
{
    MMRESULT mmr;
    if((mmr=::timeBeginPeriod(resolution))!=TIMERR_NOERROR)
        throw runtime_error(describe("timeBeginPeriod failed.(",mmr,")"));
    return make_shared<TimeEndPeriod>(resolution);
}

void timeGetDevCaps(LPTIMECAPS caps,UINT sizeOfCaps)
{
    MMRESULT mmr;
    if((mmr=::timeGetDevCaps(caps,sizeOfCaps)!=TIMERR_NOERROR))
        throw runtime_error(describe("timeGetDevCaps failed.(",mmr,")"));
}

shared_ptr<TimeKillEvent> timeSetEvent
(
    UINT delay,
    UINT resolution,
    LPTIMECALLBACK time_procedure,
    DWORD user,
    UINT event
)
{
    UINT timer=::timeSetEvent(delay,resolution,time_procedure,user,event);
    if(timer==0) throw runtime_error(describe("timeSetEvent failed."));
    return make_shared<TimeKillEvent>(timer);
}

}
