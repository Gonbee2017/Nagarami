#include"../nagarami.h"
#include<memory>
#include"nagaramitest.h"
#include<CppUTest/TestHarness.h>
#include<windows.h>

namespace nm
{

TEST_GROUP(component)
{
    TEST_SETUP()
    {
        lg=make_shared<logger>();
        pt=make_shared<port>();
        ct=make_shared<context>();
    }
    TEST_TEARDOWN()
    {
        ct.reset();
        pt.reset();
        lg.reset();
    }
    shared_ptr<logger> lg;
};

TEST(component,Component)
{
    lg->setPutWithResult(NAMED_ADDRESS(pt->SendMessageW),(LRESULT)TRUE);
    lg->setPutWithBody(NAMED_ADDRESS(pt->LoadBitmap),
    [this] (HINSTANCE instance,LPCTSTR name)->HBITMAP
    {return (HBITMAP)(0x10+lg->count("pt->LoadBitmap"));});
    lg->setPutWithBody(NAMED_ADDRESS(pt->GetObject),
    [this] (HGDIOBJ object,int sizeOfBuffer,LPVOID buffer)->int
    {
        *(BITMAP*)buffer=BITMAP
        ({1,2,3,4,5,6,(LPVOID)(0x20+lg->count("pt->GetObject"))});
        return sizeof(BITMAP);
    });
    lg->setPutWithBody
    (NAMED_ADDRESS(pt->CreateCompatibleDC),[this] (HDC dc)->HDC
    {return (HDC)(0x30+lg->count("pt->CreateCompatibleDC"));});
    lg->setPutWithResult(NAMED_ADDRESS(pt->SelectObject),(HGDIOBJ)NULL);
    lg->setPutWithBody
    (NAMED_ADDRESS(pt->CreateCompatibleBitmap),
    [this] (HDC destDC,int width,int height)->HBITMAP
    {return (HBITMAP)(0x40+lg->count("pt->CreateCompatibleBitmap"));});
    lg->setPutWithResult(NAMED_ADDRESS(pt->FillRect),TRUE);
    lg->setPutWithResult(NAMED_ADDRESS(pt->Ellipse),TRUE);
    lg->setPutWithResult(NAMED_ADDRESS(pt->BitBlt),TRUE);
    lg->setPutWithBody(NAMED_ADDRESS(pt->CreatePen),
    [this] (int style,int width,COLORREF color)->HPEN
    {return (HPEN)(0x50+lg->count("pt->CreatePen"));});
    lg->setPutWithBody(NAMED_ADDRESS(pt->CreateSolidBrush),
    [this] (COLORREF color)->HBRUSH
    {return (HBRUSH)(0x60+lg->count("pt->CreateSolidBrush"));});
    lg->setPutWithResult(NAMED_ADDRESS(pt->DeleteDC),TRUE);
    lg->setPutWithResult(NAMED_ADDRESS(pt->DeleteObject),TRUE);

    ct->instance=(HINSTANCE)0x70;
    ct->black_brush=nm::CreateSolidBrush(RGB(1,2,3));
    ct->black_pen=nm::CreatePen(1,2,RGB(1,2,3));
    ct->component_brush1=nm::CreateSolidBrush(RGB(1,2,3));
    ct->component_brush2=nm::CreateSolidBrush(RGB(1,2,3));
    ct->component_pen1=nm::CreatePen(1,2,RGB(1,2,3));
    ct->component_pen2=nm::CreatePen(1,2,RGB(1,2,3));
    ct->white_brush=nm::CreateSolidBrush(RGB(1,2,3));

    auto pushButton=make_shared<PushButton>
    (
        "hoge",
        (HDC)0x80,
        POINT({1,-2}),
        (HWND)0x90,
        (HWND)0xa0,
        7,
        L"fuga"
    );
    const size_t p1=lg->history().size();

    pushButton->activateTool();
    CHECK_EQUAL(p1+1,lg->history().size());
    CHECK_EQUAL
    (
        call("pt->SendMessageW",(HWND)0xa0,TTM_ACTIVATE,TRUE,0),
        lg->history().at(p1+0)
    );
    const size_t p2=lg->history().size();

    pushButton->activateTool();
    CHECK_EQUAL(p2+0,lg->history().size());
    const size_t p3=lg->history().size();

    pushButton->deactivateTool();
    CHECK_EQUAL(p3+1,lg->history().size());
    CHECK_EQUAL
    (
        call("pt->SendMessageW",(HWND)0xa0,TTM_ACTIVATE,FALSE,0),
        lg->history().at(p3+0)
    );
    const size_t p4=lg->history().size();

    pushButton->deactivateTool();
    CHECK_EQUAL(p4+0,lg->history().size());
}

TEST(component,Button)
{
    lg->setPutWithBody(NAMED_ADDRESS(pt->SendMessageW),
    [this] (HWND window,UINT message,WPARAM wParam,LPARAM lParam)->LRESULT
    {
        switch(lg->count("pt->SendMessageW"))
        {
        case 3:
            CHECK_EQUAL
            (
                describe(TOOLINFOW(
                {
                    TTTOOLINFOW_V2_SIZE,
                    TTF_SUBCLASS,
                    (HWND)0x90,
                    7,
                    RECT({32,152,48,168}),
                    NULL,
                    (LPWSTR)L"fuga",
                    0,
                    (LPVOID)NULL
                })),
                describe((TOOLINFOW*)lParam)
            );
            break;
        }
        return TRUE;
    });
    lg->setPutWithBody(NAMED_ADDRESS(pt->LoadBitmap),
    [this] (HINSTANCE instance,LPCTSTR name)->HBITMAP
    {return (HBITMAP)(0x10+lg->count("pt->LoadBitmap"));});
    lg->setPutWithBody(NAMED_ADDRESS(pt->GetObject),
    [this] (HGDIOBJ object,int sizeOfBuffer,LPVOID buffer)->int
    {
        *(BITMAP*)buffer=BITMAP
        ({1,2,3,4,5,6,(LPVOID)(0x20+lg->count("pt->GetObject"))});
        return sizeof(BITMAP);
    });
    lg->setPutWithBody
    (NAMED_ADDRESS(pt->CreateCompatibleDC),[this] (HDC dc)->HDC
    {return (HDC)(0x30+lg->count("pt->CreateCompatibleDC"));});
    lg->setPutWithResult(NAMED_ADDRESS(pt->SelectObject),(HGDIOBJ)NULL);
    lg->setPutWithBody
    (NAMED_ADDRESS(pt->CreateCompatibleBitmap),
    [this] (HDC destDC,int width,int height)->HBITMAP
    {return (HBITMAP)(0x40+lg->count("pt->CreateCompatibleBitmap"));});
    lg->setPutWithResult(NAMED_ADDRESS(pt->FillRect),TRUE);
    lg->setPutWithResult(NAMED_ADDRESS(pt->Ellipse),TRUE);
    lg->setPutWithResult(NAMED_ADDRESS(pt->BitBlt),TRUE);
    lg->setPutWithBody(NAMED_ADDRESS(pt->CreatePen),
    [this] (int style,int width,COLORREF color)->HPEN
    {return (HPEN)(0x50+lg->count("pt->CreatePen"));});
    lg->setPutWithBody(NAMED_ADDRESS(pt->CreateSolidBrush),
    [this] (COLORREF color)->HBRUSH
    {return (HBRUSH)(0x60+lg->count("pt->CreateSolidBrush"));});
    lg->setPutWithResult(NAMED_ADDRESS(pt->DeleteDC),TRUE);
    lg->setPutWithResult(NAMED_ADDRESS(pt->DeleteObject),TRUE);

    ct->instance=(HINSTANCE)0x70;
    ct->black_brush=nm::CreateSolidBrush(RGB(1,2,3));
    ct->black_pen=nm::CreatePen(1,2,RGB(1,2,3));
    ct->component_brush1=nm::CreateSolidBrush(RGB(1,2,3));
    ct->component_brush2=nm::CreateSolidBrush(RGB(1,2,3));
    ct->component_pen1=nm::CreatePen(1,2,RGB(1,2,3));
    ct->component_pen2=nm::CreatePen(1,2,RGB(1,2,3));
    ct->white_brush=nm::CreateSolidBrush(RGB(1,2,3));

    auto pushButton=make_shared<PushButton>
    (
        "hoge",
        (HDC)0x80,
        POINT({1,-2}),
        (HWND)0x90,
        (HWND)0xa0,
        7,
        L"fuga"
    );
    const size_t p1=lg->history().size();

    pushButton->relocate(SIZE({100,200}));
    CHECK_EQUAL(p1+1,lg->history().size());
    CHECK_EQUAL
    (
        call
        ("pt->SendMessageW",(HWND)0xa0,TTM_NEWTOOLRECTW,0,OMIT_ARGUMENT),
        lg->history().at(p1+0)
    );
    const size_t p2=lg->history().size();

    pushButton->activate(POINT({1,-2}));
    CHECK(pushButton->active());
    CHECK_EQUAL(p2+9,lg->history().size());
    CHECK_EQUAL
    (
        call("pt->FillRect",(HDC)0x32,RECT({0,0,16,16}),(HBRUSH)0x63),
        lg->history().at(p2+0)
    );
    CHECK_EQUAL
    (
        call
        (
            "pt->BitBlt",
            (HDC)0x31,
            0,
            0,
            16,
            16,
            (HDC)0x33,
            0,
            0,
            NOTSRCCOPY
        ),
        lg->history().at(p2+1)
    );
    CHECK_EQUAL
    (
        call
        (
            "pt->BitBlt",
            (HDC)0x32,
            0,
            0,
            16,
            16,
            (HDC)0x31,
            0,
            0,
            SRCAND
        ),
        lg->history().at(p2+2)
    );
    CHECK_EQUAL
    (
        call("pt->FillRect",(HDC)0x31,RECT({0,0,16,16}),(HBRUSH)0x61),
        lg->history().at(p2+3)
    );
    CHECK_EQUAL
    (
        call("pt->SelectObject",(HDC)0x31,(HGDIOBJ)0x53),
        lg->history().at(p2+4)
    );
    CHECK_EQUAL
    (
        call("pt->SelectObject",(HDC)0x31,(HGDIOBJ)0x62),
        lg->history().at(p2+5)
    );
    CHECK_EQUAL
    (
        call("pt->Ellipse",(HDC)0x31,0,0,16,16),
        lg->history().at(p2+6)
    );
    CHECK_EQUAL
    (
        call
        (
            "pt->BitBlt",
            (HDC)0x31,
            0,
            0,
            16,
            16,
            (HDC)0x33,
            0,
            0,
            SRCAND
        ),
        lg->history().at(p2+7)
    );
    CHECK_EQUAL
    (
        call
        (
            "pt->BitBlt",
            (HDC)0x31,
            0,
            0,
            16,
            16,
            (HDC)0x32,
            0,
            0,
            SRCPAINT
        ),
        lg->history().at(p2+8)
    );
    const size_t p3=lg->history().size();

    pushButton->paint((HDC)0xb0);
    CHECK_EQUAL(p3+2,lg->history().size());
    CHECK_EQUAL
    (
        call
        (
            "pt->BitBlt",
            (HDC)0xb0,
            32,
            152,
            16,
            16,
            (HDC)0x34,
            0,
            0,
            SRCAND
        ),
        lg->history().at(p3+0)
    );
    CHECK_EQUAL
    (
        call
        (
            "pt->BitBlt",
            (HDC)0xb0,
            32,
            152,
            16,
            16,
            (HDC)0x31,
            0,
            0,
            SRCPAINT
        ),
        lg->history().at(p3+1)
    );

    CHECK_FALSE(pushButton->hitTest(POINT({39,152})));
    CHECK(pushButton->hitTest(POINT({40,152})));
    CHECK_FALSE(pushButton->hitTest(POINT({32,159})));
    CHECK(pushButton->hitTest(POINT({32,160})));
    CHECK_FALSE(pushButton->hitTest(POINT({44,167})));
    CHECK(pushButton->hitTest(POINT({43,167})));
    CHECK_FALSE(pushButton->hitTest(POINT({47,164})));
    CHECK(pushButton->hitTest(POINT({47,163})));

    CHECK_FALSE(pushButton->hitTestTool(POINT({39,152})));
    CHECK(pushButton->hitTestTool(POINT({40,152})));
    CHECK_FALSE(pushButton->hitTestTool(POINT({32,159})));
    CHECK(pushButton->hitTestTool(POINT({32,160})));
    CHECK_FALSE(pushButton->hitTestTool(POINT({44,167})));
    CHECK(pushButton->hitTestTool(POINT({43,167})));
    CHECK_FALSE(pushButton->hitTestTool(POINT({47,164})));
    CHECK(pushButton->hitTestTool(POINT({47,163})));
}

TEST(component,PushButton)
{
    lg->setPutWithBody(NAMED_ADDRESS(pt->SendMessageW),
    [this] (HWND window,UINT message,WPARAM wParam,LPARAM lParam)->LRESULT
    {
        switch(lg->count("pt->SendMessageW"))
        {
        case 1:
            CHECK_EQUAL
            (
                describe(TOOLINFOW(
                {
                    TTTOOLINFOW_V2_SIZE,
                    TTF_SUBCLASS,
                    (HWND)0x90,
                    7,
                    RECT({0,0,0,0}),
                    NULL,
                    (LPWSTR)L"fuga",
                    0,
                    (LPVOID)NULL
                })),
                describe((TOOLINFOW*)lParam)
            );
            break;
        }
        return TRUE;
    });
    lg->setPutWithBody(NAMED_ADDRESS(pt->LoadBitmap),
    [this] (HINSTANCE instance,LPCTSTR name)->HBITMAP
    {return (HBITMAP)(0x10+lg->count("pt->LoadBitmap"));});
    lg->setPutWithBody(NAMED_ADDRESS(pt->GetObject),
    [this] (HGDIOBJ object,int sizeOfBuffer,LPVOID buffer)->int
    {
        *(BITMAP*)buffer=BITMAP
        ({1,2,3,4,5,6,(LPVOID)(0x20+lg->count("pt->GetObject"))});
        return sizeof(BITMAP);
    });
    lg->setPutWithBody
    (NAMED_ADDRESS(pt->CreateCompatibleDC),[this] (HDC dc)->HDC
    {return (HDC)(0x30+lg->count("pt->CreateCompatibleDC"));});
    lg->setPutWithResult(NAMED_ADDRESS(pt->SelectObject),(HGDIOBJ)NULL);
    lg->setPutWithBody
    (NAMED_ADDRESS(pt->CreateCompatibleBitmap),
    [this] (HDC destDC,int width,int height)->HBITMAP
    {return (HBITMAP)(0x40+lg->count("pt->CreateCompatibleBitmap"));});
    lg->setPutWithResult(NAMED_ADDRESS(pt->FillRect),TRUE);
    lg->setPutWithResult(NAMED_ADDRESS(pt->Ellipse),TRUE);
    lg->setPutWithResult(NAMED_ADDRESS(pt->BitBlt),TRUE);
    lg->setPutWithBody(NAMED_ADDRESS(pt->CreatePen),
    [this] (int style,int width,COLORREF color)->HPEN
    {return (HPEN)(0x50+lg->count("pt->CreatePen"));});
    lg->setPutWithBody(NAMED_ADDRESS(pt->CreateSolidBrush),
    [this] (COLORREF color)->HBRUSH
    {return (HBRUSH)(0x60+lg->count("pt->CreateSolidBrush"));});
    lg->setPutWithResult(NAMED_ADDRESS(pt->DeleteDC),TRUE);
    lg->setPutWithResult(NAMED_ADDRESS(pt->DeleteObject),TRUE);

    ct->instance=(HINSTANCE)0x70;
    ct->black_brush=nm::CreateSolidBrush(RGB(1,2,3));
    ct->black_pen=nm::CreatePen(1,2,RGB(1,2,3));
    ct->component_brush1=nm::CreateSolidBrush(RGB(1,2,3));
    ct->component_brush2=nm::CreateSolidBrush(RGB(1,2,3));
    ct->component_pen1=nm::CreatePen(1,2,RGB(1,2,3));
    ct->component_pen2=nm::CreatePen(1,2,RGB(1,2,3));
    ct->white_brush=nm::CreateSolidBrush(RGB(1,2,3));
    const size_t p1=lg->history().size();

    auto pushButton=make_shared<PushButton>
    (
        "hoge",
        (HDC)0x80,
        POINT({1,-2}),
        (HWND)0x90,
        (HWND)0xa0,
        7,
        L"fuga"
    );
    CHECK_FALSE(pushButton->active());
    CHECK_EQUAL(p1+28,lg->history().size());
    CHECK_EQUAL
    (
        call("pt->SendMessageW",(HWND)0xa0,TTM_ADDTOOLW,0,OMIT_ARGUMENT),
        lg->history().at(p1+0)
    );
    CHECK_EQUAL
    (
        call("pt->SendMessageW",(HWND)0xa0,TTM_ACTIVATE,FALSE,0),
        lg->history().at(p1+1)
    );
    CHECK_EQUAL
    (
        call("pt->CreateCompatibleBitmap",(HDC)0x80,16,16),
        lg->history().at(p1+2)
    );
    CHECK_EQUAL
    (
        call("pt->CreateCompatibleDC",(HDC)0x80),
        lg->history().at(p1+3)
    );
    CHECK_EQUAL
    (
        call("pt->SelectObject",(HDC)0x31,(HGDIOBJ)0x41),
        lg->history().at(p1+4)
    );
    CHECK_EQUAL
    (
        call("pt->CreateCompatibleBitmap",(HDC)0x80,16,16),
        lg->history().at(p1+5)
    );
    CHECK_EQUAL
    (
        call("pt->CreateCompatibleDC",(HDC)0x80),
        lg->history().at(p1+6)
    );
    CHECK_EQUAL
    (
        call("pt->SelectObject",(HDC)0x32,(HGDIOBJ)0x42),
        lg->history().at(p1+7)
    );
    CHECK_EQUAL
    (
        call("pt->LoadBitmap",(HINSTANCE)0x70,"hoge"),
        lg->history().at(p1+8)
    );
    CHECK_EQUAL
    (
        call("pt->GetObject",(HGDIOBJ)0x11,sizeof(BITMAP),OMIT_ARGUMENT),
        lg->history().at(p1+9)
    );
    CHECK_EQUAL
    (
        call("pt->CreateCompatibleDC",(HDC)0x80),
        lg->history().at(p1+10)
    );
    CHECK_EQUAL
    (
        call("pt->SelectObject",(HDC)0x33,(HGDIOBJ)0x11),
        lg->history().at(p1+11)
    );
    CHECK_EQUAL
    (
        call("pt->CreateCompatibleBitmap",(HDC)0x80,16,16),
        lg->history().at(p1+12)
    );
    CHECK_EQUAL
    (
        call("pt->CreateCompatibleDC",(HDC)0x80),
        lg->history().at(p1+13)
    );
    CHECK_EQUAL
    (
        call("pt->SelectObject",(HDC)0x34,(HGDIOBJ)0x43),
        lg->history().at(p1+14)
    );
    CHECK_EQUAL
    (
        call("pt->FillRect",(HDC)0x34,RECT({0,0,16,16}),(HBRUSH)0x64),
        lg->history().at(p1+15)
    );
    CHECK_EQUAL
    (
        call("pt->SelectObject",(HDC)0x34,(HGDIOBJ)0x51),
        lg->history().at(p1+16)
    );
    CHECK_EQUAL
    (
        call("pt->SelectObject",(HDC)0x34,(HGDIOBJ)0x61),
        lg->history().at(p1+17)
    );
    CHECK_EQUAL
    (
        call("pt->Ellipse",(HDC)0x34,0,0,16,16),
        lg->history().at(p1+18)
    );
    CHECK_EQUAL
    (
        call("pt->FillRect",(HDC)0x32,RECT({0,0,16,16}),(HBRUSH)0x62),
        lg->history().at(p1+19)
    );
    CHECK_EQUAL
    (
        call
        (
            "pt->BitBlt",
            (HDC)0x31,
            0,
            0,
            16,
            16,
            (HDC)0x33,
            0,
            0,
            NOTSRCCOPY
        ),
        lg->history().at(p1+20)
    );
    CHECK_EQUAL
    (
        call
        (
            "pt->BitBlt",
            (HDC)0x32,
            0,
            0,
            16,
            16,
            (HDC)0x31,
            0,
            0,
            SRCAND
        ),
        lg->history().at(p1+21)
    );
    CHECK_EQUAL
    (
        call("pt->FillRect",(HDC)0x31,RECT({0,0,16,16}),(HBRUSH)0x61),
        lg->history().at(p1+22)
    );
    CHECK_EQUAL
    (
        call("pt->SelectObject",(HDC)0x31,(HGDIOBJ)0x52),
        lg->history().at(p1+23)
    );
    CHECK_EQUAL
    (
        call("pt->SelectObject",(HDC)0x31,(HGDIOBJ)0x63),
        lg->history().at(p1+24)
    );
    CHECK_EQUAL
    (
        call("pt->Ellipse",(HDC)0x31,0,0,16,16),
        lg->history().at(p1+25)
    );
    CHECK_EQUAL
    (
        call
        (
            "pt->BitBlt",
            (HDC)0x31,
            0,
            0,
            16,
            16,
            (HDC)0x33,
            0,
            0,
            SRCAND
        ),
        lg->history().at(p1+26)
    );
    CHECK_EQUAL
    (
        call
        (
            "pt->BitBlt",
            (HDC)0x31,
            0,
            0,
            16,
            16,
            (HDC)0x32,
            0,
            0,
            SRCPAINT
        ),
        lg->history().at(p1+27)
    );
    const size_t p2=lg->history().size();

    pushButton.reset();
    CHECK_EQUAL(p2+8,lg->history().size());
    CHECK_EQUAL(call("pt->DeleteDC",(HDC)0x34),lg->history().at(p2+0));
    CHECK_EQUAL(call("pt->DeleteObject",(HDC)0x43),lg->history().at(p2+1));
    CHECK_EQUAL(call("pt->DeleteDC",(HDC)0x33),lg->history().at(p2+2));
    CHECK_EQUAL(call("pt->DeleteObject",(HDC)0x11),lg->history().at(p2+3));
    CHECK_EQUAL(call("pt->DeleteDC",(HDC)0x32),lg->history().at(p2+4));
    CHECK_EQUAL(call("pt->DeleteObject",(HDC)0x42),lg->history().at(p2+5));
    CHECK_EQUAL(call("pt->DeleteDC",(HDC)0x31),lg->history().at(p2+6));
    CHECK_EQUAL(call("pt->DeleteObject",(HDC)0x41),lg->history().at(p2+7));
}

}
