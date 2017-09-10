#include"../nagarami.h"
#include"nagaramitest.h"
#include<CppUTest/TestHarness.h>
#include<windows.h>

namespace nm
{

TEST_GROUP(systemobject) {};

TEST(systemobject,DeleteDC)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteDC),TRUE);
        auto dc=make_shared<DeleteDC>((HDC)0x10);
        CHECK_EQUAL((HDC)0x10,dc->handle());
        CHECK_EQUAL(0,lg.history().size());
        dc.reset();
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL(call("pt.DeleteDC",(HDC)0x10),lg.history().at(0));
    }
}

TEST(systemobject,DeleteObject)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteObject),TRUE);
        auto object=make_shared<DeleteObject>((HGDIOBJ)0x10);
        CHECK_EQUAL((HGDIOBJ)0x10,object->handle());
        CHECK_EQUAL(0,lg.history().size());
        object.reset();
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (call("pt.DeleteObject",(HGDIOBJ)0x10),lg.history().at(0));
    }
}

TEST(systemobject,Buffer_create)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateCompatibleBitmap),(HBITMAP)0x10);
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateCompatibleDC),(HDC)0x20);
        lg.setPutWithResult(NAMED_ADDRESS(pt.SelectObject),(HGDIOBJ)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteDC),TRUE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteObject),TRUE);
        auto buffer=Buffer::create(SIZE({1,-2}),(HDC)0x30);
        CHECK_EQUAL((HBITMAP)0x10,buffer->bitmap());
        CHECK_EQUAL((HDC)0x20,buffer->dc());
        CHECK_EQUAL(SIZE({1,-2}),buffer->size());
        CHECK_EQUAL(3,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.CreateCompatibleBitmap",(HDC)0x30,1,-2),
            lg.history().at(0)
        );
        CHECK_EQUAL
        (call("pt.CreateCompatibleDC",(HDC)0x30),lg.history().at(1));
        CHECK_EQUAL
        (
            call("pt.SelectObject",(HDC)0x20,(HGDIOBJ)0x10),
            lg.history().at(2)
        );
        buffer.reset();
        CHECK_EQUAL(5,lg.history().size());
        CHECK_EQUAL(call("pt.DeleteDC",(HGDIOBJ)0x20),lg.history().at(3));
        CHECK_EQUAL
        (call("pt.DeleteObject",(HGDIOBJ)0x10),lg.history().at(4));
    }
}

TEST(systemobject,Buffer_load)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.LoadBitmap),(HBITMAP)0x10);
        lg.setPutWithBody(NAMED_ADDRESS(pt.GetObject),
        [] (HGDIOBJ object,int sizeOfBuffer,LPVOID buffer)->int
        {
            *(BITMAP*)buffer=BITMAP({1,2,3,4,5,6,(LPVOID)0x20});
            return sizeof(BITMAP);
        });
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateCompatibleDC),(HDC)0x30);
        lg.setPutWithResult(NAMED_ADDRESS(pt.SelectObject),(HGDIOBJ)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteDC),TRUE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteObject),TRUE);
        auto buffer=Buffer::load((HINSTANCE)0x40,TEXT("hoge"),(HDC)0x50);
        CHECK_EQUAL((HBITMAP)0x10,buffer->bitmap());
        CHECK_EQUAL((HDC)0x30,buffer->dc());
        CHECK_EQUAL(SIZE({2,3}),buffer->size());
        CHECK_EQUAL(4,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.LoadBitmap",(HINSTANCE)0x40,TEXT("hoge")),
            lg.history().at(0)
        );
        CHECK_EQUAL
        (
            call
            ("pt.GetObject",(HGDIOBJ)0x10,sizeof(BITMAP),OMIT_ARGUMENT),
            lg.history().at(1)
        );
        CHECK_EQUAL
        (call("pt.CreateCompatibleDC",(HDC)0x50),lg.history().at(2));
        CHECK_EQUAL
        (
            call("pt.SelectObject",(HDC)0x30,(HGDIOBJ)0x10),
            lg.history().at(3)
        );
        buffer.reset();
        CHECK_EQUAL(6,lg.history().size());
        CHECK_EQUAL(call("pt.DeleteDC",(HGDIOBJ)0x30),lg.history().at(4));
        CHECK_EQUAL
        (call("pt.DeleteObject",(HGDIOBJ)0x10),lg.history().at(5));
    }
}

TEST(systemobject,EndPaint)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.EndPaint),TRUE);
        PAINTSTRUCT paint({(HDC)0x10,TRUE,RECT({1,-2,-3,5}),FALSE,TRUE});
        auto dc=make_shared<EndPaint>((HWND)0x20,&paint,(HDC)0x10);
        CHECK_EQUAL((HDC)0x10,dc->handle());
        CHECK_EQUAL(0,lg.history().size());
        dc.reset();
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.EndPaint",
                (HWND)0x20,
                PAINTSTRUCT({(HDC)0x10,TRUE,RECT({1,-2,-3,5}),FALSE,TRUE})
            ),
            lg.history().at(0)
        );
    }
}

TEST(systemobject,ReleaseDC)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.ReleaseDC),1);
        auto dc=make_shared<ReleaseDC>((HWND)0x10,(HDC)0x20);
        CHECK_EQUAL((HDC)0x20,dc->handle());
        CHECK_EQUAL(0,lg.history().size());
        dc.reset();
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (call("pt.ReleaseDC",(HWND)0x10,(HDC)0x20),lg.history().at(0));
    }
}

TEST(systemobject,TimeEndPeriod)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.timeEndPeriod),(MMRESULT)TIMERR_NOERROR);
        auto period=make_shared<TimeEndPeriod>(1);
        CHECK_EQUAL(0,lg.history().size());
        period.reset();
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL(call("pt.timeEndPeriod",1),lg.history().at(0));
    }
}

TEST(systemobject,TimeKillEvent)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.timeKillEvent),(MMRESULT)TIMERR_NOERROR);
        auto event=make_shared<TimeKillEvent>(1);
        CHECK_EQUAL(0,lg.history().size());
        event.reset();
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL(call("pt.timeKillEvent",1),lg.history().at(0));
    }
}

TEST(systemobject,Timer)
{
    {
        logger lg;
        LPTIMECALLBACK proc;
        lg.setPutWithBody(NAMED_ADDRESS(pt.timeSetEvent),
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
        lg.setPutWithResult(NAMED_ADDRESS(pt.PostMessage),TRUE);
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.timeKillEvent),(MMRESULT)TIMERR_NOERROR);
        auto timer=make_shared<Timer>(2,3,(HWND)0x10);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.timeSetEvent",
                2,
                3,
                OMIT_ARGUMENT,
                (DWORD)0x10,
                TIME_CALLBACK_FUNCTION|TIME_KILL_SYNCHRONOUS|TIME_PERIODIC
            ),
            lg.history().at(0)
        );
        proc(4,5,(DWORD_PTR)0x20,6,7);
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.PostMessage",(HWND)0x20,WM_USERTIMER,0,0),
            lg.history().at(1)
        );
        timer.reset();
        CHECK_EQUAL(3,lg.history().size());
        CHECK_EQUAL(call("pt.timeKillEvent",1),lg.history().at(2));
    }
}

}
