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

TEST_GROUP(helper)
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

TEST(helper,bind_object)
{
    string str="hoge";
    const function<size_t()> length=bind_object(&string::length,&str);
    CHECK_EQUAL(4,length());
    const function<char&(size_t pos)> at=bind_object(&string::at,&str);
    CHECK_EQUAL('g',at(2));
}

TEST(helper,decompose)
{
    string name;
    int number;
    decompose(make_pair(string("hoge"),1),&name,&number);
    CHECK_EQUAL("hoge",name);
    CHECK_EQUAL(1,number);
}

TEST(helper,describe)
{
    CHECK_EQUAL("hoge",describe("hoge"));
    CHECK_EQUAL("fuga10x10",describe("fuga",1,(const void*)0x10));
}

TEST(helper,describe_with)
{
    CHECK_EQUAL("hoge",describe_with(",","hoge"));
    CHECK_EQUAL
    ("fuga,1,0x10",describe_with(",","fuga",1,(const void*)0x10));
}

TEST(helper,operator_output_procedure)
{
    ostringstream oss;
    oss<<(void(CALLBACK*)())0x10;
    CHECK_EQUAL("0x10",oss.str());
}

TEST(helper,fill)
{
    POINT p;
    fill(&p,0);
    CHECK_EQUAL(0,p.x);
    CHECK_EQUAL(0,p.y);
}

TEST(helper,Finalizer)
{
    function<void()> finalize;
    lg->setPut(NAMED_ADDRESS(finalize));
    auto finalizer=make_shared<Finalizer>(finalize);
    CHECK_EQUAL(0,lg->history().size());
    finalizer.reset();
    CHECK_EQUAL(1,lg->history().size());
    CHECK_EQUAL(call("finalize"),lg->history().at(0));
}

TEST(helper,api_error)
{
    lg->setPutWithResult(NAMED_ADDRESS(pt->GetLastError),(DWORD)1);
    CHECK_THROWS_API_ERROR("hoge",1,throw api_error("hoge"));
    CHECK_EQUAL(1,lg->history().size());
    CHECK_EQUAL(call("pt->GetLastError"),lg->history().at(0));
    CHECK_THROWS_API_ERROR("fuga",2,throw api_error("fuga",2));
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
    lg->setPutWithBody(NAMED_ADDRESS(pt->GetCursorPos),
    [] (LPPOINT point)->BOOL
    {
        *point=POINT({1,2});
        return TRUE;
    });
    lg->setPutWithBody(NAMED_ADDRESS(pt->ScreenToClient),
    [] (HWND window,LPPOINT point)->BOOL
    {
        *point=POINT({-2,-4});
        return TRUE;
    });
    CHECK_EQUAL(POINT({-2,-4}),cursor_pos((HWND)0x10));
    CHECK_EQUAL(2,lg->history().size());
    CHECK_EQUAL(call("pt->GetCursorPos",POINT({0,0})),lg->history().at(0));
    CHECK_EQUAL
    (
        call("pt->ScreenToClient",(HWND)0x10,POINT({1,2})),
        lg->history().at(1)
    );
}

TEST(helper,desktop_size)
{
    lg->setPutWithBody(NAMED_ADDRESS(pt->GetSystemMetrics),
    [this] (int index)->int {return lg->count("pt->GetSystemMetrics");});
    CHECK_EQUAL(SIZE({1,2}),desktop_size());
    CHECK_EQUAL(2,lg->history().size());
    CHECK_EQUAL
    (call("pt->GetSystemMetrics",SM_CXSCREEN),lg->history().at(0));
    CHECK_EQUAL
    (call("pt->GetSystemMetrics",SM_CYSCREEN),lg->history().at(1));
}

TEST(helper,floating_point_number)
{
    CHECK_THROWS_RUNTIME_ERROR
    (
        "'hoge' is an invalid floating-point number.",
        floating_point_number("hoge")
    );
    CHECK_EQUAL(1.5,floating_point_number("1.5"));
    CHECK_EQUAL(-2.25,floating_point_number("-2.25"));
}

TEST(helper,getlines)
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

TEST(helper,height)
{
    CHECK_EQUAL(3,height(RECT({1,2,3,5})));
    CHECK_EQUAL(13,height(RECT({-34,-21,-13,-8})));
}

TEST(helper,integer)
{
    CHECK_THROWS_RUNTIME_ERROR
    ("'hoge' is an invalid integer.",integer("hoge"));
    CHECK_EQUAL(1,integer("1"));
    CHECK_EQUAL(-2,integer("-2"));
    CHECK_EQUAL(3,integer("0x3"));
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
        POINT p({1,-2});
        CHECK_EQUAL(POINT({-2,3}),p+=POINT({-3,5}));
    }
    {
        POINT p({-8,13});
        CHECK_EQUAL(POINT({13,-21}),p+=POINT({21,-34}));
    }
}

TEST(helper,operator_add_assign_POINT_DOUBLE_POINT_DOUBLE)
{
    {
        POINT_DOUBLE p({1.5,-2.25});
        CHECK_EQUAL
        (POINT_DOUBLE({-1.625,2.75}),p+=POINT_DOUBLE({-3.125,5.0}));
    }
    {
        POINT_DOUBLE p({-8.5,13.25});
        CHECK_EQUAL
        (POINT_DOUBLE({12.625,-20.75}),p+=POINT_DOUBLE({21.125,-34.0}));
    }
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
    const auto tokens=tokenize("hoge:fuga,,piyo",":,");
    CHECK_EQUAL(3,tokens.size());
    CHECK_EQUAL("hoge",tokens.at(0));
    CHECK_EQUAL("fuga",tokens.at(1));
    CHECK_EQUAL("piyo",tokens.at(2));
}

TEST(helper,width)
{
    CHECK_EQUAL(-4,width(RECT({1,-2,-3,5})));
    CHECK_EQUAL(29,width(RECT({-8,13,21,-34})));
}

}
