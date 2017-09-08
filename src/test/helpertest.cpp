#include"../nagarami.h"
#include<functional>
#include<memory>
#include"nagaramitest.h"
#include<sstream>
#include<stdexcept>
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
        const function<size_t()> length=bind_object(&string::length,&str);
        CHECK_EQUAL(4,length());
        const function<char&(size_t pos)> at=bind_object(&string::at,&str);
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

TEST(helper,set)
{
    {
        string name;
        int number;
        set(make_pair(string("hoge"),1),&name,&number);
        CHECK_EQUAL("hoge",name);
        CHECK_EQUAL(1,number);
    }
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

TEST(helper,api_error)
{
    {
        history hist;
        hist.setWithResult(NAMED_ADDRESS(pt().GetLastError),(DWORD)1);
        try
        {
            throw api_error("hoge");
            FAIL("Do not pass here.");
        } catch(const runtime_error&error)
        {CHECK_EQUAL("hoge failed.(1)",string(error.what()));}
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
    } catch(const runtime_error&error)
    {
        CHECK_EQUAL
        (
            "'hoge' is an invalid floating-point number.",
            string(error.what())
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

TEST(helper,integer)
{
    try
    {
        integer("hoge");
        FAIL("Do not pass here.");
    } catch(const runtime_error&error)
    {CHECK_EQUAL("'hoge' is an invalid integer.",string(error.what()));}
    CHECK_EQUAL(1,integer("1"));
    CHECK_EQUAL(-2,integer("-2"));
    CHECK_EQUAL(3,integer("0x3"));
}

TEST(helper,operator_not_equal_POINT)
{
    CHECK(POINT({1,-2})!=POINT({2,-1}));
    CHECK_FALSE(POINT({-3,5})!=POINT({-3,5}));
}

TEST(helper,operator_not_equal_POINT_DOUBLE)
{
    CHECK(POINT_DOUBLE({1.5,-2.25})!=POINT_DOUBLE({2.5,-1.25}));
    CHECK_FALSE(POINT_DOUBLE({-3.125,5.0})!=POINT_DOUBLE({-3.125,5.0}));
}

TEST(helper,operator_not_equal_RECT)
{
    CHECK(RECT({1,-2,3,-5})!=RECT({-1,2,-3,5}));
    CHECK_FALSE(RECT({-8,13,-21,34})!=RECT({-8,13,-21,34}));
}

TEST(helper,operator_not_equal_SIZE)
{
    CHECK(SIZE({1,-2})!=SIZE({2,-1}));
    CHECK_FALSE(SIZE({-3,5})!=SIZE({-3,5}));
}

TEST(helper,operator_multiply_POINT_LONG)
{
    CHECK_EQUAL(POINT({3,-6}),POINT({1,-2})*3);
    CHECK_EQUAL(POINT({65,-104}),POINT({-5,8})*-13);
}

TEST(helper,operator_multiply_SIZE_LONG)
{
    CHECK_EQUAL(SIZE({3,-6}),SIZE({1,-2})*3);
    CHECK_EQUAL(SIZE({65,-104}),SIZE({-5,8})*-13);
}

TEST(helper,operator_add_POINT_POINT)
{
    CHECK_EQUAL(POINT({-2,3}),POINT({1,-2})+POINT({-3,5}));
    CHECK_EQUAL(POINT({13,-21}),POINT({-8,13})+POINT({21,-34}));
}

TEST(helper,operator_add_POINT_DOUBLE_POINT_DOUBLE)
{
    CHECK_EQUAL
    (
        POINT_DOUBLE({-1.625,2.75}),
        POINT_DOUBLE({1.5,-2.25})+POINT_DOUBLE({-3.125,5.0})
    );
    CHECK_EQUAL
    (
        POINT_DOUBLE({12.625,-20.75}),
        POINT_DOUBLE({-8.5,13.25})+POINT_DOUBLE({21.125,-34.0})
    );
}

TEST(helper,operator_add_assign_POINT_POINT)
{
    {
        POINT p=POINT({1,-2});
        CHECK_EQUAL(POINT({-2,3}),p+=POINT({-3,5}));
    }
    {
        POINT p=POINT({-8,13});
        CHECK_EQUAL(POINT({13,-21}),p+=POINT({21,-34}));
    }
}

TEST(helper,operator_add_assign_POINT_DOUBLE_POINT_DOUBLE)
{
    {
        POINT_DOUBLE p=POINT_DOUBLE({1.5,-2.25});
        CHECK_EQUAL
        (POINT_DOUBLE({-1.625,2.75}),p+=POINT_DOUBLE({-3.125,5.0}));
    }
    {
        POINT_DOUBLE p=POINT_DOUBLE({-8.5,13.25});
        CHECK_EQUAL
        (POINT_DOUBLE({12.625,-20.75}),p+=POINT_DOUBLE({21.125,-34.0}));
    }
}

TEST(helper,operator_negate_POINT)
{
    CHECK_EQUAL(POINT({-1,2}),-POINT({1,-2}));
    CHECK_EQUAL(POINT({3,-5}),-POINT({-3,5}));
}

TEST(helper,operator_subtract_POINT_POINT)
{
    CHECK_EQUAL(POINT({4,-7}),POINT({1,-2})-POINT({-3,5}));
    CHECK_EQUAL(POINT({-29,47}),POINT({-8,13})-POINT({21,-34}));
}

TEST(helper,operator_subtract_SIZE_SIZE)
{
    CHECK_EQUAL(SIZE({4,-7}),SIZE({1,-2})-SIZE({-3,5}));
    CHECK_EQUAL(SIZE({-29,47}),SIZE({-8,13})-SIZE({21,-34}));
}

TEST(helper,operator_divide_POINT_LONG)
{
    CHECK_EQUAL(POINT({2,-4}),POINT({5,-8})/2);
    CHECK_EQUAL(POINT({-4,7}),POINT({-13,21})/3);
}

TEST(helper,operator_divide_SIZE_LONG)
{
    CHECK_EQUAL(SIZE({2,-4}),SIZE({5,-8})/2);
    CHECK_EQUAL(SIZE({-4,7}),SIZE({-13,21})/3);
}

TEST(helper,operator_output_char_pointer)
{
    {
        ostringstream oss;
        oss<<"hoge"<<((const char*)nullptr)<<"fuga";
        CHECK_EQUAL("hogefuga",oss.str());
    }
}

TEST(helper,operator_output_PAINTSTRUCT)
{
    {
        ostringstream oss;
        oss<<
            PAINTSTRUCT({(HDC)0x10,TRUE,RECT({1,-2,-3,5}),FALSE,TRUE})<<
            ","<<
            PAINTSTRUCT({(HDC)0x20,FALSE,RECT({-8,13,21,-34}),TRUE,FALSE});
        CHECK_EQUAL
        ("{0x10,1,{1,-2,-3,5},0,1},{0x20,0,{-8,13,21,-34},1,0}",oss.str());
    }
}

TEST(helper,operator_output_POINT)
{
    {
        ostringstream oss;
        oss<<POINT({1,-2})<<","<<POINT({-3,5});
        CHECK_EQUAL("{1,-2},{-3,5}",oss.str());
    }
}

TEST(helper,operator_output_POINT_DOUBLE)
{
    {
        ostringstream oss;
        oss<<POINT_DOUBLE({1.5,-2.25})<<","<<POINT_DOUBLE({-3.125,5.0});
        CHECK_EQUAL("{1.5,-2.25},{-3.125,5}",oss.str());
    }
}

TEST(helper,operator_output_RECT)
{
    {
        ostringstream oss;
        oss<<RECT({1,-2,-3,5})<<","<<RECT({-8,13,21,-34});
        CHECK_EQUAL("{1,-2,-3,5},{-8,13,21,-34}",oss.str());
    }
}

TEST(helper,operator_output_SIZE)
{
    {
        ostringstream oss;
        oss<<SIZE({1,-2})<<","<<SIZE({-3,5});
        CHECK_EQUAL("{1,-2},{-3,5}",oss.str());
    }
}

TEST(helper,operator_equal_POINT)
{
    CHECK_FALSE(POINT({1,-2})==POINT({2,-1}));
    CHECK(POINT({-3,5})==POINT({-3,5}));
}

TEST(helper,operator_equal_POINT_DOUBLE)
{
    CHECK_FALSE(POINT_DOUBLE({1.5,-2.25})==POINT_DOUBLE({2.5,-1.25}));
    CHECK(POINT_DOUBLE({-3.125,5.0})==POINT_DOUBLE({-3.125,5.0}));
}

TEST(helper,operator_equal_RECT)
{
    CHECK_FALSE(RECT({1,-2,3,-5})==RECT({-1,2,-3,5}));
    CHECK(RECT({-8,13,-21,34})==RECT({-8,13,-21,34}));
}

TEST(helper,operator_equal_SIZE)
{
    CHECK_FALSE(SIZE({1,-2})==SIZE({2,-1}));
    CHECK(SIZE({-3,5})==SIZE({-3,5}));
}

TEST(helper,point_POINT_DOUBLE)
{
    CHECK_EQUAL(POINT({2,-2}),point(POINT_DOUBLE({1.5,-2.25})));
    CHECK_EQUAL(POINT({-4,5}),point(POINT_DOUBLE({-3.625,5.0})));
}

TEST(helper,point_SIZE)
{
    CHECK_EQUAL(POINT({1,-2}),point(SIZE({1,-2})));
    CHECK_EQUAL(POINT({-3,5}),point(SIZE({-3,5})));
}

TEST(helper,point_double_POINT)
{
    CHECK_EQUAL(POINT_DOUBLE({1,-2}),point_double(POINT({1,-2})));
    CHECK_EQUAL(POINT_DOUBLE({-3,5}),point_double(POINT({-3,5})));
}

TEST(helper,pos)
{
    CHECK_EQUAL(POINT({1,-2}),pos(RECT({1,-2,-3,5})));
    CHECK_EQUAL(POINT({-8,13}),pos(RECT({-8,13,21,-34})));
}

TEST(helper,putlines)
{
    {
        ostringstream oss;
        putlines(oss,vector<string>({"hoge","fuga","piyo"}));
        CHECK_EQUAL
        (
            "hoge\n"
            "fuga\n"
            "piyo\n",
            oss.str()
        );
    }
}

TEST(helper,rect)
{
    CHECK_EQUAL(RECT({1,-2,-2,3}),rect(POINT({1,-2}),SIZE({-3,5})));
    CHECK_EQUAL(RECT({-8,13,13,-21}),rect(POINT({-8,13}),SIZE({21,-34})));
}

TEST(helper,size_LPARAM)
{
    CHECK_EQUAL(SIZE({1,-2}),size(MAKELONG(1,-2)));
    CHECK_EQUAL(SIZE({-3,5}),size(MAKELONG(-3,5)));
}

TEST(helper,size_POINT)
{
    CHECK_EQUAL(SIZE({1,-2}),size(POINT({1,-2})));
    CHECK_EQUAL(SIZE({-3,5}),size(POINT({-3,5})));
}

TEST(helper,size_RECT)
{
    CHECK_EQUAL(SIZE({-4,7}),size(RECT({1,-2,-3,5})));
    CHECK_EQUAL(SIZE({29,-47}),size(RECT({-8,13,21,-34})));
}

TEST(helper,squared_distance)
{
    CHECK_EQUAL(9,squared_distance(POINT({0,1}),POINT({0,-2})));
    CHECK_EQUAL(45,squared_distance(POINT({-2,-3}),POINT({4,-6})));
}

TEST(helper,tokenize)
{
    {
        const auto tokens=tokenize("hoge:fuga,,piyo",":,");
        CHECK_EQUAL(3,tokens.size());
        CHECK_EQUAL("hoge",tokens.at(0));
        CHECK_EQUAL("fuga",tokens.at(1));
        CHECK_EQUAL("piyo",tokens.at(2));
    }
}

TEST(helper,width)
{
    CHECK_EQUAL(-4,width(RECT({1,-2,-3,5})));
    CHECK_EQUAL(29,width(RECT({-8,13,21,-34})));
}

}
