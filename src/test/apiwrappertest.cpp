#include"../nagarami.h"
#include"nagaramitest.h"
#include<CppUTest/TestHarness.h>
#include<windows.h>

namespace nm
{

TEST_GROUP(apiwrapper) {};

TEST(apiwrapper,BeginPaint)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.BeginPaint),(HDC)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        PAINTSTRUCT paint;
        CHECK_THROWS_API_ERROR
        ("BeginPaint",1,nm::BeginPaint((HWND)0x10,&paint));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.BeginPaint",
                (HWND)0x10,
                PAINTSTRUCT({(HDC)0x0,FALSE,RECT({0,0,0,0}),FALSE,FALSE})
            ),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithBody(NAMED_ADDRESS(pt.BeginPaint),
        [] (HWND window,PAINTSTRUCT*paint)->HDC
        {
            *paint=PAINTSTRUCT
            ({(HDC)0x10,TRUE,RECT({1,-2,-3,5}),FALSE,TRUE});
            return (HDC)0x10;
        });
        lg.setPutWithResult(NAMED_ADDRESS(pt.EndPaint),TRUE);
        PAINTSTRUCT paint;
        auto dc=nm::BeginPaint((HWND)0x20,&paint);
        CHECK_EQUAL
        (
            describe
            (PAINTSTRUCT({(HDC)0x10,TRUE,RECT({1,-2,-3,5}),FALSE,TRUE})),
            describe(paint)
        );
        CHECK_EQUAL((HDC)0x10,dc->handle());
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.BeginPaint",
                (HWND)0x20,
                PAINTSTRUCT({(HDC)0x0,FALSE,RECT({0,0,0,0}),FALSE,FALSE})
            ),
            lg.history().at(0)
        );
        dc.reset();
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.EndPaint",
                (HWND)0x20,
                PAINTSTRUCT({(HDC)0x10,TRUE,RECT({1,-2,-3,5}),FALSE,TRUE})
            ),
            lg.history().at(1)
        );
    }
}

TEST(apiwrapper,BitBlt)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.BitBlt),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        (
            "BitBlt",
            1,
            nm::BitBlt
            (
                (HDC)0x10,
                1,
                -2,
                -3,
                5,
                (HDC)0x20,
                -8,
                13,
                SRCCOPY
            )
        );
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.BitBlt",
                (HDC)0x10,
                1,
                -2,
                -3,
                5,
                (HDC)0x20,
                -8,
                13,
                SRCCOPY
            ),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.BitBlt),TRUE);
        nm::BitBlt
        (
            (HDC)0x10,
            1,
            -2,
            -3,
            5,
            (HDC)0x20,
            -8,
            13,
            SRCCOPY
        );
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.BitBlt",
                (HDC)0x10,
                1,
                -2,
                -3,
                5,
                (HDC)0x20,
                -8,
                13,
                SRCCOPY
            ),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,CreateCompatibleBitmap)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateCompatibleBitmap),(HBITMAP)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        (
            "CreateCompatibleBitmap",
            1,
            nm::CreateCompatibleBitmap((HDC)0x10,1,-2)
        );
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.CreateCompatibleBitmap",(HDC)0x10,1,-2),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateCompatibleBitmap),(HBITMAP)0x10);
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteObject),TRUE);
        auto dc=nm::CreateCompatibleBitmap((HDC)0x20,1,-2);
        CHECK_EQUAL((HDC)0x10,dc->handle());
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.CreateCompatibleBitmap",(HDC)0x20,1,-2),
            lg.history().at(0)
        );
        dc.reset();
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.DeleteObject",(HGDIOBJ)0x10),lg.history().at(1));
    }
}

TEST(apiwrapper,CreateCompatibleDC)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateCompatibleDC),(HDC)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        (
            "CreateCompatibleDC",
            1,
            nm::CreateCompatibleDC((HDC)0x10)
        );
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.CreateCompatibleDC",(HDC)0x10),lg.history().at(0));
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateCompatibleDC),(HDC)0x10);
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteDC),TRUE);
        auto dc=nm::CreateCompatibleDC((HDC)0x20);
        CHECK_EQUAL((HDC)0x10,dc->handle());
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (call("pt.CreateCompatibleDC",(HDC)0x20),lg.history().at(0));
        dc.reset();
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL(call("pt.DeleteDC",(HDC)0x10),lg.history().at(1));
    }
}

TEST(apiwrapper,CreateFont)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.CreateFont),(HFONT)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        (
            "CreateFont",
            1,
            nm::CreateFont(1,2,3,4,5,6,7,8,9,10,11,12,13,TEXT("hoge"))
        );
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.CreateFont",
                1,
                2,
                3,
                4,
                5,
                6,
                7,
                8,
                9,
                10,
                11,
                12,
                13,
                TEXT("hoge")
            ),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.CreateFont),(HFONT)0x10);
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteObject),TRUE);
        auto font=
            nm::CreateFont(1,2,3,4,5,6,7,8,9,10,11,12,13,TEXT("hoge"));
        CHECK_EQUAL((HGDIOBJ)0x10,font->handle());
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.CreateFont",
                1,
                2,
                3,
                4,
                5,
                6,
                7,
                8,
                9,
                10,
                11,
                12,
                13,
                TEXT("hoge")
            ),
            lg.history().at(0)
        );
        font.reset();
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.DeleteObject",(HGDIOBJ)0x10),lg.history().at(1));
    }
}

TEST(apiwrapper,CreatePatternBrush)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreatePatternBrush),(HBRUSH)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        (
            "CreatePatternBrush",
            1,
            nm::CreatePatternBrush((HBITMAP)0x10)
        );
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.CreatePatternBrush",(HBITMAP)0x10),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreatePatternBrush),(HBRUSH)0x10);
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteObject),TRUE);
        auto brush=nm::CreatePatternBrush((HBITMAP)0x20);
        CHECK_EQUAL((HGDIOBJ)0x10,brush->handle());
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.CreatePatternBrush",(HBITMAP)0x20),
            lg.history().at(0)
        );
        brush.reset();
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.DeleteObject",(HGDIOBJ)0x10),lg.history().at(1));
    }
}

TEST(apiwrapper,CreatePen)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.CreatePen),(HPEN)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        ("CreatePen",1,nm::CreatePen(1,2,RGB(1,2,3)));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.CreatePen",1,2,RGB(1,2,3)),lg.history().at(0));
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.CreatePen),(HPEN)0x10);
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteObject),TRUE);
        auto pen=nm::CreatePen(1,2,RGB(1,2,3));
        CHECK_EQUAL((HGDIOBJ)0x10,pen->handle());
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (call("pt.CreatePen",1,2,RGB(1,2,3)),lg.history().at(0));
        pen.reset();
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.DeleteObject",(HGDIOBJ)0x10),lg.history().at(1));
    }
}

TEST(apiwrapper,CreateSolidBrush)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateSolidBrush),(HBRUSH)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        ("CreateSolidBrush",1,nm::CreateSolidBrush(RGB(1,2,3)));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.CreateSolidBrush",RGB(1,2,3)),lg.history().at(0));
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateSolidBrush),(HBRUSH)0x10);
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteObject),TRUE);
        auto brush=nm::CreateSolidBrush(RGB(1,2,3));
        CHECK_EQUAL((HGDIOBJ)0x10,brush->handle());
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (call("pt.CreateSolidBrush",RGB(1,2,3)),lg.history().at(0));
        brush.reset();
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.DeleteObject",(HGDIOBJ)0x10),lg.history().at(1));
    }
}

TEST(apiwrapper,CreateWindowEx)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateWindowEx),(HWND)NULL);
        ct.error=runtime_error("hoge");
        CHECK_THROWS_RUNTIME_ERROR
        (
            "hoge",
            nm::CreateWindowEx
            (
                1,
                "fuga",
                "piyo",
                2,
                3,
                4,
                5,
                6,
                (HWND)0x10,
                (HMENU)0x20,
                (HINSTANCE)0x30,
                (LPVOID)0x40
            )
        );
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.CreateWindowEx",
                1,
                TEXT("fuga"),
                TEXT("piyo"),
                2,
                3,
                4,
                5,
                6,
                (HWND)0x10,
                (HMENU)0x20,
                (HINSTANCE)0x30,
                (LPVOID)0x40
            ),
            lg.history().at(0)
        );
    }
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateWindowEx),(HWND)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        (
            "CreateWindowEx",
            1,
            nm::CreateWindowEx
            (
                1,
                TEXT("hoge"),
                TEXT("fuga"),
                2,
                3,
                4,
                5,
                6,
                (HWND)0x10,
                (HMENU)0x20,
                (HINSTANCE)0x30,
                (LPVOID)0x40
            )
        );
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.CreateWindowEx",
                1,
                TEXT("hoge"),
                TEXT("fuga"),
                2,
                3,
                4,
                5,
                6,
                (HWND)0x10,
                (HMENU)0x20,
                (HINSTANCE)0x30,
                (LPVOID)0x40
            ),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.CreateWindowEx),(HWND)0x10);
        CHECK_EQUAL
        (
            (HWND)0x10,
            nm::CreateWindowEx
            (
                1,
                TEXT("hoge"),
                TEXT("fuga"),
                2,
                3,
                4,
                5,
                6,
                (HWND)0x20,
                (HMENU)0x30,
                (HINSTANCE)0x40,
                (LPVOID)0x50
            )
        );
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.CreateWindowEx",
                1,
                TEXT("hoge"),
                TEXT("fuga"),
                2,
                3,
                4,
                5,
                6,
                (HWND)0x20,
                (HMENU)0x30,
                (HINSTANCE)0x40,
                (LPVOID)0x50
            ),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,DispatchMessage)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.DispatchMessage),(LRESULT)1);
        MSG message({(HWND)0x10,1,2,3,4,POINT({1,-2})});
        ct.error=runtime_error("hoge");
        CHECK_THROWS_RUNTIME_ERROR("hoge",nm::DispatchMessage(&message));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.DispatchMessage",
                MSG({(HWND)0x10,1,2,3,4,POINT({1,-2})})
            ),
            lg.history().at(0)
        );
    }
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.DispatchMessage),(LRESULT)1);
        MSG message({(HWND)0x10,1,2,3,4,POINT({1,-2})});
        CHECK_EQUAL(1,nm::DispatchMessage(&message));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.DispatchMessage",
                MSG({(HWND)0x10,1,2,3,4,POINT({1,-2})})
            ),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,DrawText)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.DrawText),0);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        RECT rect({1,-2,-3,5});
        CHECK_THROWS_API_ERROR
        (
            "DrawText",
            1,
            nm::DrawText((HDC)0x10,TEXT("hoge"),1,&rect,2)
        );
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.DrawText",
                (HDC)0x10,
                TEXT("hoge"),
                1,
                RECT({1,-2,-3,5}),
                2
            ),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.DrawText),1);
        RECT rect({1,-2,-3,5});
        CHECK_EQUAL(1,nm::DrawText((HDC)0x10,TEXT("hoge"),1,&rect,2));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.DrawText",
                (HDC)0x10,
                TEXT("hoge"),
                1,
                RECT({1,-2,-3,5}),
                2
            ),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,Ellipse)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.Ellipse),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        ("Ellipse",1,nm::Ellipse((HDC)0x10,1,2,3,4));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.Ellipse",(HDC)0x10,1,2,3,4),lg.history().at(0));
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.Ellipse),TRUE);
        nm::Ellipse((HDC)0x10,1,2,3,4);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (call("pt.Ellipse",(HDC)0x10,1,2,3,4),lg.history().at(0));
    }
}

TEST(apiwrapper,FillRect)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.FillRect),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        RECT rect({1,-2,-3,5});
        CHECK_THROWS_API_ERROR
        ("FillRect",1,nm::FillRect((HDC)0x10,&rect,(HBRUSH)0x20));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            ("pt.FillRect",(HDC)0x10,RECT({1,-2,-3,5}),(HBRUSH)0x20),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.FillRect),TRUE);
        RECT rect({1,-2,-3,5});
        nm::FillRect((HDC)0x10,&rect,(HBRUSH)0x20);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            ("pt.FillRect",(HDC)0x10,RECT({1,-2,-3,5}),(HBRUSH)0x20),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,GetClientRect)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetClientRect),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        RECT rect;
        CHECK_THROWS_API_ERROR
        ("GetClientRect",1,nm::GetClientRect((HWND)0x10,&rect));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.GetClientRect",(HWND)0x10,RECT({0,0,0,0})),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithBody(NAMED_ADDRESS(pt.GetClientRect),
        [] (HWND window,LPRECT rect)->BOOL
        {
            *rect=RECT({1,-2,-3,5});
            return TRUE;
        });
        RECT rect;
        nm::GetClientRect((HWND)0x10,&rect);
        CHECK_EQUAL(RECT({1,-2,-3,5}),rect);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.GetClientRect",(HWND)0x10,RECT({0,0,0,0})),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,GetCursorPos)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetCursorPos),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        POINT pos;
        CHECK_THROWS_API_ERROR("GetCursorPos",1,nm::GetCursorPos(&pos));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.GetCursorPos",POINT({0,0})),lg.history().at(0));
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithBody(NAMED_ADDRESS(pt.GetCursorPos),
        [] (LPPOINT pos)->BOOL
        {
            *pos=POINT({1,-2});
            return TRUE;
        });
        POINT pos;
        nm::GetCursorPos(&pos);
        CHECK_EQUAL(POINT({1,-2}),pos);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (call("pt.GetCursorPos",POINT({0,0})),lg.history().at(0));
    }
}

TEST(apiwrapper,GetDC)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetDC),(HDC)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR("GetDC",1,nm::GetDC((HWND)0x10));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL(call("pt.GetDC",(HWND)0x10),lg.history().at(0));
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetDC),(HDC)0x10);
        lg.setPutWithResult(NAMED_ADDRESS(pt.ReleaseDC),1);
        auto dc=nm::GetDC((HWND)0x20);
        CHECK_EQUAL((HDC)0x10,dc->handle());
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL(call("pt.GetDC",(HWND)0x20),lg.history().at(0));
        dc.reset();
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.ReleaseDC",(HWND)0x20,(HDC)0x10),lg.history().at(1));
    }
}

TEST(apiwrapper,GetMessage)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetMessage),-1);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        MSG message;
        CHECK_THROWS_API_ERROR
        ("GetMessage",1,nm::GetMessage(&message,(HWND)0x10,1,2));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.GetMessage",
                MSG({(HWND)0x0,0,0,0,0,POINT({0,0})}),
                (HWND)0x10,
                1,
                2
            ),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithBody(NAMED_ADDRESS(pt.GetMessage),
        [] (LPMSG message,HWND window,UINT first,UINT last)->BOOL
        {
            *message=MSG({(HWND)0x10,1,2,3,4,POINT({1,-2})});
            return 1;
        });
        MSG message;
        CHECK(nm::GetMessage(&message,(HWND)0x20,1,2));
        CHECK_EQUAL
        (
            describe(MSG({(HWND)0x10,1,2,3,4,POINT({1,-2})})),
            describe(message)
        );
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.GetMessage",
                MSG({(HWND)0x0,0,0,0,0,POINT({0,0})}),
                (HWND)0x20,
                1,
                2
            ),
            lg.history().at(0)
        );
    }
    {
        logger lg;
        lg.setPutWithBody(NAMED_ADDRESS(pt.GetMessage),
        [] (LPMSG message,HWND window,UINT first,UINT last)->BOOL
        {
            *message=MSG({(HWND)0x10,1,2,3,4,POINT({1,-2})});
            return 0;
        });
        MSG message;
        CHECK_FALSE(nm::GetMessage(&message,(HWND)0x20,1,2));
        CHECK_EQUAL
        (
            describe(MSG({(HWND)0x10,1,2,3,4,POINT({1,-2})})),
            describe(message)
        );
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.GetMessage",
                MSG({(HWND)0x0,0,0,0,0,POINT({0,0})}),
                (HWND)0x20,
                1,
                2
            ),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,GetObject)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetObject),0);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        ("GetObject",1,nm::GetObject((HGDIOBJ)0x10,1,(LPVOID)0x20));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.GetObject",(HGDIOBJ)0x10,1,(LPVOID)0x20),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetObject),1);
        MSG message;
        CHECK_EQUAL(1,nm::GetObject((HGDIOBJ)0x10,1,(LPVOID)0x20));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.GetObject",(HGDIOBJ)0x10,1,(LPVOID)0x20),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,GetSystemMetrics)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetSystemMetrics),0);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        ("GetSystemMetrics",1,nm::GetSystemMetrics(1));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL(call("pt.GetSystemMetrics",1),lg.history().at(0));
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetSystemMetrics),1);
        MSG message;
        CHECK_EQUAL(1,nm::GetSystemMetrics(1));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL(call("pt.GetSystemMetrics",1),lg.history().at(0));
    }
}

TEST(apiwrapper,GetWindowPlacement)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetWindowPlacement),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        WINDOWPLACEMENT placement;
        CHECK_THROWS_API_ERROR
        (
            "GetWindowPlacement",
            1,
            nm::GetWindowPlacement((HWND)0x10,&placement)
        );
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.GetWindowPlacement",
                (HWND)0x10,
                WINDOWPLACEMENT(
                {
                    sizeof(WINDOWPLACEMENT),
                    0,
                    0,
                    POINT({0,0}),
                    POINT({0,0}),
                    RECT({0,0,0,0})
                })
            ),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithBody(NAMED_ADDRESS(pt.GetWindowPlacement),
        [] (HWND window,WINDOWPLACEMENT*placement)->BOOL
        {
            *placement=WINDOWPLACEMENT
            ({1,2,3,POINT({1,-2}),POINT({-3,5}),RECT({-8,13,21,-34})});
            return TRUE;
        });
        WINDOWPLACEMENT placement;
        nm::GetWindowPlacement((HWND)0x10,&placement);
        CHECK_EQUAL
        (
            describe
            (
                WINDOWPLACEMENT
                ({1,2,3,POINT({1,-2}),POINT({-3,5}),RECT({-8,13,21,-34})})
            ),
            describe(placement)
        );
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.GetWindowPlacement",
                (HWND)0x10,
                WINDOWPLACEMENT(
                {
                    sizeof(WINDOWPLACEMENT),
                    0,
                    0,
                    POINT({0,0}),
                    POINT({0,0}),
                    RECT({0,0,0,0})
                })
            ),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,LoadBitmap)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.LoadBitmap),(HBITMAP)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        ("LoadBitmap",1,nm::LoadBitmap((HINSTANCE)0x10,TEXT("hoge")));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.LoadBitmap",(HINSTANCE)0x10,TEXT("hoge")),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.LoadBitmap),(HBITMAP)0x10);
        lg.setPutWithResult(NAMED_ADDRESS(pt.DeleteObject),TRUE);
        auto bitmap=nm::LoadBitmap((HINSTANCE)0x20,TEXT("hoge"));
        CHECK_EQUAL((HGDIOBJ)0x10,bitmap->handle());
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.LoadBitmap",(HINSTANCE)0x20,TEXT("hoge")),
            lg.history().at(0)
        );
        bitmap.reset();
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.DeleteObject",(HGDIOBJ)0x10),lg.history().at(1));
    }
}

TEST(apiwrapper,LoadCursor)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.LoadCursor),(HCURSOR)NULL);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        ("LoadCursor",1,nm::LoadCursor((HINSTANCE)0x10,TEXT("hoge")));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.LoadCursor",(HINSTANCE)0x10,TEXT("hoge")),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.LoadCursor),(HCURSOR)0x10);
        CHECK_EQUAL
        ((HCURSOR)0x10,nm::LoadCursor((HINSTANCE)0x20,TEXT("hoge")));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.LoadCursor",(HINSTANCE)0x20,TEXT("hoge")),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,PostMessage)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.PostMessage),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        ("PostMessage",1,nm::PostMessage((HWND)0x10,1,2,3));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.PostMessage",
                (HWND)0x10,
                1,
                2,
                3
            ),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.PostMessage),TRUE);
        nm::PostMessage((HWND)0x10,1,2,3);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.PostMessage",
                (HWND)0x10,
                1,
                2,
                3
            ),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,RedrawWindow)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.RedrawWindow),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        RECT rect({1,-2,-3,5});
        CHECK_THROWS_API_ERROR
        (
            "RedrawWindow",
            1,
            nm::RedrawWindow((HWND)0x10,&rect,(HRGN)0x20,1)
        );
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.RedrawWindow",
                (HWND)0x10,
                RECT({1,-2,-3,5}),
                (HRGN)0x20,
                1
            ),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.RedrawWindow),TRUE);
        RECT rect({1,-2,-3,5});
        nm::RedrawWindow((HWND)0x10,&rect,(HRGN)0x20,1);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.RedrawWindow",
                (HWND)0x10,
                RECT({1,-2,-3,5}),
                (HRGN)0x20,
                1
            ),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,RegisterClassEx)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.RegisterClassEx),(ATOM)0);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        WNDCLASSEX clazz(
        {
            1,
            2,
            (WNDPROC)0x10,
            3,
            4,
            (HINSTANCE)0x20,
            (HICON)0x30,
            (HCURSOR)0x40,
            (HBRUSH)0x50,
            TEXT("hoge"),
            TEXT("fuga"),
            (HICON)0x60
        });
        CHECK_THROWS_API_ERROR
        ("RegisterClassEx",1,nm::RegisterClassEx(&clazz));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.RegisterClassEx",
                WNDCLASSEX(
                {
                    1,
                    2,
                    (WNDPROC)0x10,
                    3,
                    4,
                    (HINSTANCE)0x20,
                    (HICON)0x30,
                    (HCURSOR)0x40,
                    (HBRUSH)0x50,
                    TEXT("hoge"),
                    TEXT("fuga"),
                    (HICON)0x60
                })
            ),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.RegisterClassEx),(ATOM)1);
        WNDCLASSEX clazz(
        {
            1,
            2,
            (WNDPROC)0x10,
            3,
            4,
            (HINSTANCE)0x20,
            (HICON)0x30,
            (HCURSOR)0x40,
            (HBRUSH)0x50,
            TEXT("hoge"),
            TEXT("fuga"),
            (HICON)0x60
        });
        CHECK_EQUAL((ATOM)1,nm::RegisterClassEx(&clazz));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.RegisterClassEx",
                WNDCLASSEX(
                {
                    1,
                    2,
                    (WNDPROC)0x10,
                    3,
                    4,
                    (HINSTANCE)0x20,
                    (HICON)0x30,
                    (HCURSOR)0x40,
                    (HBRUSH)0x50,
                    TEXT("hoge"),
                    TEXT("fuga"),
                    (HICON)0x60
                })
            ),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,ReleaseCapture)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.ReleaseCapture),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR("ReleaseCapture",1,nm::ReleaseCapture());
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL(call("pt.ReleaseCapture"),lg.history().at(0));
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.ReleaseCapture),TRUE);
        nm::ReleaseCapture();
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL(call("pt.ReleaseCapture"),lg.history().at(0));
    }
}

TEST(apiwrapper,ScreenToClient)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.ScreenToClient),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        POINT pos({1,-2});
        CHECK_THROWS_API_ERROR
        ("ScreenToClient",1,nm::ScreenToClient((HWND)0x10,&pos));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.ScreenToClient",(HWND)0x10,POINT({1,-2})),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithBody(NAMED_ADDRESS(pt.ScreenToClient),
        [] (HWND window,LPPOINT pos)->BOOL
        {
            *pos=POINT({-3,5});
            return TRUE;
        });
        POINT pos({1,-2});
        nm::ScreenToClient((HWND)0x10,&pos);
        CHECK_EQUAL(POINT({-3,5}),pos);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.ScreenToClient",(HWND)0x10,POINT({1,-2})),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,SetBkMode)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.SetBkMode),0);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR("SetBkMode",1,nm::SetBkMode((HDC)0x10,1));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.SetBkMode",(HDC)0x10,1),lg.history().at(0));
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.SetBkMode),1);
        CHECK_EQUAL(1,nm::SetBkMode((HDC)0x10,2));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (call("pt.SetBkMode",(HDC)0x10,2),lg.history().at(0));
    }
}

TEST(apiwrapper,SetBrushOrgEx)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.SetBrushOrgEx),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        ("SetBrushOrgEx",1,nm::SetBrushOrgEx((HDC)0x10,1,2,NULL));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.SetBrushOrgEx",(HDC)0x10,1,2,(LPPOINT)NULL),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithBody(NAMED_ADDRESS(pt.SetBrushOrgEx),
        [] (HDC dc,int x,int y,LPPOINT oldPos)->BOOL
        {
            *oldPos=POINT({-3,5});
            return TRUE;
        });
        POINT oldPos;
        nm::SetBrushOrgEx((HDC)0x10,1,2,&oldPos);
        CHECK_EQUAL(POINT({-3,5}),oldPos);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.SetBrushOrgEx",(HDC)0x10,1,2,POINT({0,0})),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,SetForegroundWindow)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.SetForegroundWindow),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        POINT oldPos;
        CHECK_THROWS_API_ERROR
        ("SetForegroundWindow",1,nm::SetForegroundWindow((HWND)0x10));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.SetForegroundWindow",(HWND)0x10),lg.history().at(0));
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.SetForegroundWindow),TRUE);
        nm::SetForegroundWindow((HWND)0x10);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (call("pt.SetForegroundWindow",(HWND)0x10),lg.history().at(0));
    }
}

TEST(apiwrapper,SetStretchBltMode)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.SetStretchBltMode),0);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        ("SetStretchBltMode",1,nm::SetStretchBltMode((HDC)0x10,1));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (call("pt.SetStretchBltMode",(HDC)0x10,1),lg.history().at(0));
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.SetStretchBltMode),1);
        CHECK_EQUAL(1,nm::SetStretchBltMode((HDC)0x10,2));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (call("pt.SetStretchBltMode",(HDC)0x10,2),lg.history().at(0));
    }
}

TEST(apiwrapper,SetTextColor)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.SetTextColor),(COLORREF)CLR_INVALID);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        ("SetTextColor",1,nm::SetTextColor((HDC)0x10,RGB(1,2,3)));
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.SetTextColor",(HDC)0x10,RGB(1,2,3)),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.SetTextColor),RGB(1,2,3));
        CHECK_EQUAL(RGB(1,2,3),nm::SetTextColor((HDC)0x10,RGB(4,5,6)));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.SetTextColor",(HDC)0x10,RGB(4,5,6)),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,ShellExecute)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.ShellExecute),(HINSTANCE)32);
        CHECK_THROWS_API_ERROR
        (
            "ShellExecute",
            32,
            nm::ShellExecute
            (
                (HWND)0x10,
                TEXT("hoge"),
                TEXT("fuga"),
                TEXT("piyo"),
                TEXT("foo"),
                2
            )
        );
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.ShellExecute",
                (HWND)0x10,
                TEXT("hoge"),
                TEXT("fuga"),
                TEXT("piyo"),
                TEXT("foo"),
                2
            ),
            lg.history().at(0)
        );
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.ShellExecute),(HINSTANCE)33);
        CHECK_EQUAL
        (
            (HINSTANCE)33,
            nm::ShellExecute
            (
                (HWND)0x10,
                TEXT("hoge"),
                TEXT("fuga"),
                TEXT("piyo"),
                TEXT("foo"),
                2
            )
        );
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.ShellExecute",
                (HWND)0x10,
                TEXT("hoge"),
                TEXT("fuga"),
                TEXT("piyo"),
                TEXT("foo"),
                2
            ),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,StretchBlt)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.StretchBlt),FALSE);
        lg.setPutWithResult(NAMED_ADDRESS(pt.GetLastError),(DWORD)1);
        CHECK_THROWS_API_ERROR
        (
            "StretchBlt",
            1,
            nm::StretchBlt
            (
                (HDC)0x10,
                1,
                -2,
                -3,
                5,
                (HDC)0x20,
                -8,
                13,
                21,
                -34,
                SRCCOPY
            )
        );
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.StretchBlt",
                (HDC)0x10,
                1,
                -2,
                -3,
                5,
                (HDC)0x20,
                -8,
                13,
                21,
                -34,
                SRCCOPY
            ),
            lg.history().at(0)
        );
        CHECK_EQUAL(call("pt.GetLastError"),lg.history().at(1));
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.StretchBlt),TRUE);
        nm::StretchBlt
        (
            (HDC)0x10,
            1,
            -2,
            -3,
            5,
            (HDC)0x20,
            -8,
            13,
            21,
            -34,
            SRCCOPY
        );
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call
            (
                "pt.StretchBlt",
                (HDC)0x10,
                1,
                -2,
                -3,
                5,
                (HDC)0x20,
                -8,
                13,
                21,
                -34,
                SRCCOPY
            ),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,timeBeginPeriod)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.timeBeginPeriod),(MMRESULT)TIMERR_NOCANDO);
        CHECK_THROWS_API_ERROR
        ("timeBeginPeriod",TIMERR_NOCANDO,nm::timeBeginPeriod(1));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL(call("pt.timeBeginPeriod",1),lg.history().at(0));
    }
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.timeBeginPeriod),(MMRESULT)TIMERR_NOERROR);
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.timeEndPeriod),(MMRESULT)TIMERR_NOERROR);
        auto period=nm::timeBeginPeriod(1);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL(call("pt.timeBeginPeriod",1),lg.history().at(0));
        period.reset();
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL(call("pt.timeEndPeriod",1),lg.history().at(1));
    }
}

TEST(apiwrapper,timeGetDevCaps)
{
    {
        logger lg;
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.timeGetDevCaps),(MMRESULT)TIMERR_STRUCT);
        TIMECAPS caps;
        CHECK_THROWS_API_ERROR
        (
            "timeGetDevCaps",
            TIMERR_STRUCT,
            nm::timeGetDevCaps(&caps,sizeof(TIMECAPS))
        );
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.timeGetDevCaps",TIMECAPS({0,0}),sizeof(TIMECAPS)),
            lg.history().at(0)
        );
    }
    {
        logger lg;
        lg.setPutWithBody(NAMED_ADDRESS(pt.timeGetDevCaps),
        [] (LPTIMECAPS caps,UINT sizeOfCaps)->MMRESULT
        {
            *caps=TIMECAPS({1,2});
            return TIMERR_NOERROR;
        });
        TIMECAPS caps;
        nm::timeGetDevCaps(&caps,sizeof(TIMECAPS));
        CHECK_EQUAL(describe(TIMECAPS({1,2})),describe(caps));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.timeGetDevCaps",TIMECAPS({0,0}),sizeof(TIMECAPS)),
            lg.history().at(0)
        );
    }
}

TEST(apiwrapper,timeSetEvent)
{
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.timeSetEvent),(UINT)NULL);
        CHECK_THROWS_API_ERROR
        ("timeSetEvent",0,nm::timeSetEvent(1,2,(LPTIMECALLBACK)0x10,3,4));
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.timeSetEvent",1,2,(LPTIMECALLBACK)0x10,3,4),
            lg.history().at(0)
        );
    }
    {
        logger lg;
        lg.setPutWithResult(NAMED_ADDRESS(pt.timeSetEvent),(UINT)1);
        lg.setPutWithResult
        (NAMED_ADDRESS(pt.timeKillEvent),(MMRESULT)TIMERR_NOERROR);
        auto event=nm::timeSetEvent(1,2,(LPTIMECALLBACK)0x10,3,4);
        CHECK_EQUAL(1,lg.history().size());
        CHECK_EQUAL
        (
            call("pt.timeSetEvent",1,2,(LPTIMECALLBACK)0x10,3,4),lg.
            history().at(0)
        );
        event.reset();
        CHECK_EQUAL(2,lg.history().size());
        CHECK_EQUAL(call("pt.timeKillEvent",1),lg.history().at(1));
    }
}

}
