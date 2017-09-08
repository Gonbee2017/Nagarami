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
        history hist;
        hist.setWithResult(NAMED_ADDRESS(pt().BeginPaint),(HDC)NULL);
        hist.setWithResult(NAMED_ADDRESS(pt().GetLastError),(DWORD)1);
        PAINTSTRUCT paint;
        fill(&paint,0);
        try
        {
            nm::BeginPaint((HWND)0x10,&paint);
            FAIL("Do not pass here.");
        } catch(const shared_ptr<runtime_error>&error)
        {CHECK_EQUAL("BeginPaint failed.(1)",string(error->what()));}
        CHECK_EQUAL(2,hist.calls().size());
        CHECK_EQUAL
        (
            call
            (
                "pt().BeginPaint",
                (HWND)0x10,
                PAINTSTRUCT({(HDC)0x0,FALSE,RECT({0,0,0,0}),FALSE,FALSE})
            ),
            hist.calls().at(0)
        );
        CHECK_EQUAL(call("pt().GetLastError"),hist.calls().at(1));
    }
}

}
