#include"../nagarami.h"
#include<functional>
#include<memory>
#include"nagaramitest.h"
#include<sstream>
#include<string>
#include<CppUTest/TestHarness.h>
#include<windows.h>

namespace nm
{

TEST_GROUP(helper) {};

TEST(helper,bind_object)
{
    {
        string str="hoge";
        function<size_t()> length=bind_object(&string::length,&str);
        CHECK_EQUAL(4,length());
        function<char&(size_t pos)> at=bind_object(&string::at,&str);
        CHECK_EQUAL('g',at(2));
    }
}

TEST(helper,describe)
{
    CHECK_EQUAL("hoge",describe("hoge"));
    CHECK_EQUAL("fuga1",describe("fuga",1));
}

TEST(helper,describe_with)
{
    CHECK_EQUAL("hoge",describe_with(",","hoge"));
    CHECK_EQUAL("fuga,1",describe_with(",","fuga",1));
}

TEST(helper,Finalizer_destructor)
{
    {
        history hist;
        function<void()> finalize;
        hist.set(NAMED_ADDRESS(finalize));
        auto finalizer=make_shared<Finalizer>(finalize);
        CHECK_EQUAL(0,hist.calls().size());
        finalizer.reset();
        CHECK_EQUAL(1,hist.calls().size());
        CHECK_EQUAL(call("finalize"),hist.calls().at(0));
    }
}

TEST(helper,Initializer_constructor)
{
    {
        history hist;
        function<void()> initialize;
        hist.set(NAMED_ADDRESS(initialize));
        Initializer initializer(initialize);
        CHECK_EQUAL(1,hist.calls().size());
        CHECK_EQUAL(call("initialize"),hist.calls().at(0));
    }
}

TEST(helper,api_error_constructor)
{
    {
        history hist;
        hist.setWithResult(NAMED_ADDRESS(pt().GetLastError),(DWORD)1);
        api_error error("hoge");
        CHECK_EQUAL("hoge failed.(1)",string(error.what()));
        CHECK_EQUAL(1,hist.calls().size());
        CHECK_EQUAL(call("pt().GetLastError"),hist.calls().at(0));
    }
}

TEST(helper,contain_circle)
{
    CHECK(contain(POINT({1,2}),SQUARE(1),POINT({2,2})));
    CHECK_FALSE(contain(POINT({1,2}),SQUARE(1),POINT({2,3})));
    CHECK(contain(POINT({-2,-4}),SQUARE(2),POINT({-3,-5})));
    CHECK_FALSE(contain(POINT({-2,-4}),SQUARE(2),POINT({-4,-5})));
}

TEST(helper,contain_rect)
{
    CHECK(contain(RECT({-1,-1,1,1}),POINT({-1,-1})));
    CHECK_FALSE(contain(RECT({-1,-1,1,1}),POINT({1,1})));
    CHECK(contain(RECT({-2,-2,2,2}),POINT({1,1})));
    CHECK_FALSE(contain(RECT({-2,-2,2,2}),POINT({-2,-3})));
}

TEST(helper,coordinates)
{
    CHECK_EQUAL(POINT({1,2}),coordinates(MAKELONG(1,2)));
    CHECK_EQUAL(POINT({-2,-4}),coordinates(MAKELONG(-2,-4)));
}

TEST(helper,cursor_pos)
{
    {
        history hist;
        hist.setWithBody(NAMED_ADDRESS(pt().GetCursorPos),
        [] (LPPOINT point)->BOOL
        {
            *point=POINT({1,2});
            return TRUE;
        });
        hist.setWithBody(NAMED_ADDRESS(pt().ScreenToClient),
        [] (HWND window,LPPOINT point)->BOOL
        {
            *point=POINT({-2,-4});
            return TRUE;
        });
        CHECK_EQUAL(POINT({-2,-4}),cursor_pos((HWND)0x10));
        CHECK_EQUAL(2,hist.calls().size());
        CHECK_EQUAL
        (call("pt().GetCursorPos",POINT({0,0})),hist.calls().at(0));
        CHECK_EQUAL
        (
            call("pt().ScreenToClient",(HWND)0x10,POINT({1,2})),
            hist.calls().at(1)
        );
    }
}

TEST(helper,desktop_size)
{
    {
        history hist;
        hist.setWithBody(NAMED_ADDRESS(pt().GetSystemMetrics),
        [&hist] (int index)->int
        {return hist.count("pt().GetSystemMetrics");});
        CHECK_EQUAL(SIZE({1,2}),desktop_size());
        CHECK_EQUAL(2,hist.calls().size());
        CHECK_EQUAL
        (call("pt().GetSystemMetrics",SM_CXSCREEN),hist.calls().at(0));
        CHECK_EQUAL
        (call("pt().GetSystemMetrics",SM_CYSCREEN),hist.calls().at(1));
    }
}

TEST(helper,floating_point_number)
{
    try
    {
        floating_point_number("hoge");
        FAIL("Do not pass here.");
    } catch(const shared_ptr<runtime_error>&error)
    {
        CHECK_EQUAL
        (
            "'hoge' is an invalid floating-point number.",
            string(error->what())
        );
    }
    CHECK_EQUAL(1.5,floating_point_number("1.5"));
    CHECK_EQUAL(-2.25,floating_point_number("-2.25"));
}

TEST(helper,getlines)
{
    {
        istringstream iss
        (
            "hoge\r\n"
            "fuga\r\n"
            "piyo\r\n"
        );
        const auto lines=getlines(iss);
        CHECK_EQUAL(3,lines.size());
        CHECK_EQUAL("hoge",lines.at(0));
        CHECK_EQUAL("fuga",lines.at(1));
        CHECK_EQUAL("piyo",lines.at(2));
    }
}

TEST(helper,height)
{
    CHECK_EQUAL(3,height(RECT({1,2,3,5})));
    CHECK_EQUAL(13,height(RECT({-34,-21,-13,-8})));
}

TEST(helper,squared_distance)
{
    CHECK_EQUAL(9,squared_distance(POINT({0,1}),POINT({0,-2})));
    CHECK_EQUAL(45,squared_distance(POINT({-2,-3}),POINT({4,-6})));
}

}
