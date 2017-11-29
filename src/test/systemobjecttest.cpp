#include"../nagarami.h"
#include<memory>
#include"nagaramitest.h"
#include<CppUTest/TestHarness.h>
#include<windows.h>

namespace nm
{

TEST_GROUP(systemobject)
{
    TEST_SETUP()
    {
        im=make_shared<Imitator>();
        pt=make_shared<Port>();
    }
    TEST_TEARDOWN()
    {
        pt.reset();
        im.reset();
    }
    shared_ptr<Imitator> im;
};

TEST(systemobject,DeleteDC)
{
    im->mockUpWithResult(NAMED_ADDRESS(pt->DeleteDC),TRUE);
    auto dc=make_shared<DeleteDC>((HDC)0x10);
    CHECK_EQUAL((HDC)0x10,dc->handle());
    CHECK_EQUAL(0,im->history().size());
    dc.reset();
    CHECK_EQUAL(1,im->history().size());
    CHECK_EQUAL
    (call(NAMED_ADDRESS(pt->DeleteDC),(HDC)0x10),im->history().at(0));
}

TEST(systemobject,DeleteObject)
{
    im->mockUpWithResult(NAMED_ADDRESS(pt->DeleteObject),TRUE);
    auto object=make_shared<DeleteObject>((HGDIOBJ)0x10);
    CHECK_EQUAL((HGDIOBJ)0x10,object->handle());
    CHECK_EQUAL(0,im->history().size());
    object.reset();
    CHECK_EQUAL(1,im->history().size());
    CHECK_EQUAL
    (
        call(NAMED_ADDRESS(pt->DeleteObject),(HGDIOBJ)0x10),
        im->history().at(0)
    );
}

TEST(systemobject,Buffer_create)
{
    im->mockUpWithResult
    (NAMED_ADDRESS(pt->CreateCompatibleBitmap),(HBITMAP)0x10);
    im->mockUpWithResult
    (NAMED_ADDRESS(pt->CreateCompatibleDC),(HDC)0x20);
    im->mockUpWithResult(NAMED_ADDRESS(pt->SelectObject),(HGDIOBJ)NULL);
    im->mockUpWithResult(NAMED_ADDRESS(pt->DeleteDC),TRUE);
    im->mockUpWithResult(NAMED_ADDRESS(pt->DeleteObject),TRUE);
    auto buffer=Buffer::create(SIZE({1,-2}),(HDC)0x30);
    CHECK_EQUAL((HBITMAP)0x10,buffer->bitmap());
    CHECK_EQUAL((HDC)0x20,buffer->dc());
    CHECK_EQUAL(SIZE({1,-2}),buffer->size());
    CHECK_EQUAL(3,im->history().size());
    CHECK_EQUAL
    (
        call(NAMED_ADDRESS(pt->CreateCompatibleBitmap),(HDC)0x30,1,-2),
        im->history().at(0)
    );
    CHECK_EQUAL
    (
        call(NAMED_ADDRESS(pt->CreateCompatibleDC),(HDC)0x30),
        im->history().at(1)
    );
    CHECK_EQUAL
    (
        call(NAMED_ADDRESS(pt->SelectObject),(HDC)0x20,(HGDIOBJ)0x10),
        im->history().at(2)
    );
    buffer.reset();
    CHECK_EQUAL(5,im->history().size());
    CHECK_EQUAL
    (call(NAMED_ADDRESS(pt->DeleteDC),(HDC)0x20),im->history().at(3));
    CHECK_EQUAL
    (
        call(NAMED_ADDRESS(pt->DeleteObject),(HGDIOBJ)0x10),
        im->history().at(4)
    );
}

TEST(systemobject,Buffer_load)
{
    im->mockUpWithResult(NAMED_ADDRESS(pt->LoadBitmap),(HBITMAP)0x10);
    im->mockUpWithBody(NAMED_ADDRESS(pt->GetObject),
    [] (HGDIOBJ object,int sizeOfBuffer,LPVOID buffer)->int
    {
        *(BITMAP*)buffer=BITMAP({1,2,3,4,5,6,(LPVOID)0x20});
        return sizeof(BITMAP);
    });
    im->mockUpWithResult
    (NAMED_ADDRESS(pt->CreateCompatibleDC),(HDC)0x30);
    im->mockUpWithResult(NAMED_ADDRESS(pt->SelectObject),(HGDIOBJ)NULL);
    im->mockUpWithResult(NAMED_ADDRESS(pt->DeleteDC),TRUE);
    im->mockUpWithResult(NAMED_ADDRESS(pt->DeleteObject),TRUE);
    auto buffer=Buffer::load((HINSTANCE)0x40,"hoge",(HDC)0x50);
    CHECK_EQUAL((HBITMAP)0x10,buffer->bitmap());
    CHECK_EQUAL((HDC)0x30,buffer->dc());
    CHECK_EQUAL(SIZE({2,3}),buffer->size());
    CHECK_EQUAL(4,im->history().size());
    CHECK_EQUAL
    (
        call(NAMED_ADDRESS(pt->LoadBitmap),(HINSTANCE)0x40,"hoge"),
        im->history().at(0)
    );
    CHECK_EQUAL
    (
        call
        (
            NAMED_ADDRESS(pt->GetObject),
            (HGDIOBJ)0x10,
            (int)sizeof(BITMAP),
            (LPVOID)OMIT_POINTER
        ),
        im->history().at(1)
    );
    CHECK_EQUAL
    (
        call(NAMED_ADDRESS(pt->CreateCompatibleDC),(HDC)0x50),
        im->history().at(2)
    );
    CHECK_EQUAL
    (
        call(NAMED_ADDRESS(pt->SelectObject),(HDC)0x30,(HGDIOBJ)0x10),
        im->history().at(3)
    );
    buffer.reset();
    CHECK_EQUAL(6,im->history().size());
    CHECK_EQUAL
    (call(NAMED_ADDRESS(pt->DeleteDC),(HDC)0x30),im->history().at(4));
    CHECK_EQUAL
    (
        call(NAMED_ADDRESS(pt->DeleteObject),(HGDIOBJ)0x10),
        im->history().at(5)
    );
}

TEST(systemobject,EndPaint)
{
    im->mockUpWithResult(NAMED_ADDRESS(pt->EndPaint),TRUE);
    PAINTSTRUCT paint({(HDC)0x10,TRUE,RECT({1,-2,-3,5}),FALSE,TRUE});
    auto dc=make_shared<EndPaint>((HWND)0x20,&paint,(HDC)0x10);
    CHECK_EQUAL((HDC)0x10,dc->handle());
    CHECK_EQUAL(0,im->history().size());
    dc.reset();
    CHECK_EQUAL(1,im->history().size());
    CHECK_EQUAL
    (
        call
        (
            NAMED_ADDRESS(pt->EndPaint),
            (HWND)0x20,
            make_shared_pod<const PAINTSTRUCT>
            ((HDC)0x10,TRUE,RECT({1,-2,-3,5}),FALSE,TRUE).get()
        ),
        im->history().at(0)
    );
}

TEST(systemobject,ReleaseDC)
{
    im->mockUpWithResult(NAMED_ADDRESS(pt->ReleaseDC),1);
    auto dc=make_shared<ReleaseDC>((HWND)0x10,(HDC)0x20);
    CHECK_EQUAL((HDC)0x20,dc->handle());
    CHECK_EQUAL(0,im->history().size());
    dc.reset();
    CHECK_EQUAL(1,im->history().size());
    CHECK_EQUAL
    (
        call(NAMED_ADDRESS(pt->ReleaseDC),(HWND)0x10,(HDC)0x20),
        im->history().at(0)
    );
}

TEST(systemobject,TimeEndPeriod)
{
    im->mockUpWithResult
    (NAMED_ADDRESS(pt->timeEndPeriod),(MMRESULT)TIMERR_NOERROR);
    auto period=make_shared<TimeEndPeriod>(1);
    CHECK_EQUAL(0,im->history().size());
    period.reset();
    CHECK_EQUAL(1,im->history().size());
    CHECK_EQUAL
    (call(NAMED_ADDRESS(pt->timeEndPeriod),1u),im->history().at(0));
}

TEST(systemobject,TimeKillEvent)
{
    im->mockUpWithResult
    (NAMED_ADDRESS(pt->timeKillEvent),(MMRESULT)TIMERR_NOERROR);
    auto event=make_shared<TimeKillEvent>(1);
    CHECK_EQUAL(0,im->history().size());
    event.reset();
    CHECK_EQUAL(1,im->history().size());
    CHECK_EQUAL
    (call(NAMED_ADDRESS(pt->timeKillEvent),1u),im->history().at(0));
}

TEST(systemobject,Timer)
{
    LPTIMECALLBACK proc;
    im->mockUpWithBody(NAMED_ADDRESS(pt->timeSetEvent),
    [&proc]
    (
        UINT delay,
        UINT resolution,
        LPTIMECALLBACK procedure,
        DWORD user,
        UINT event
    )->UINT
    {
        proc=procedure;
        return 1;
    });
    im->mockUpWithResult(NAMED_ADDRESS(pt->PostMessage),TRUE);
    im->mockUpWithResult
    (NAMED_ADDRESS(pt->timeKillEvent),(MMRESULT)TIMERR_NOERROR);
    auto timer=make_shared<Timer>(2,3,(HWND)0x10);
    CHECK_EQUAL(1,im->history().size());
    CHECK_EQUAL
    (
        call
        (
            NAMED_ADDRESS(pt->timeSetEvent),
            2u,
            3u,
            (LPTIMECALLBACK)OMIT_POINTER,
            (DWORD)0x10,
            (UINT)
                TIME_CALLBACK_FUNCTION|
                TIME_KILL_SYNCHRONOUS|
                TIME_PERIODIC
        ),
        im->history().at(0)
    );
    proc(4,5,(DWORD_PTR)0x20,6,7);
    CHECK_EQUAL(2,im->history().size());
    CHECK_EQUAL
    (
        call(NAMED_ADDRESS(pt->PostMessage),(HWND)0x20,WM_USERTIMER,0u,0l),
        im->history().at(1)
    );
    timer.reset();
    CHECK_EQUAL(3,im->history().size());
    CHECK_EQUAL
    (call(NAMED_ADDRESS(pt->timeKillEvent),1u),im->history().at(2));
}

}
