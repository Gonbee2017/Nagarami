#include<functional>
#include"../nagarami.h"
#include<memory>
#include"nagaramitest.h"
#include<CppUTest/TestHarness.h>

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

TEST(helper,api_error_constructor)
{
    {
        const auto cp=make_shared<ClearPort>();
        history hist;
        hist.set(NAMED_ADDRESS(pt().GetLastError),(DWORD)1);
        api_error error("hoge");
        CHECK_EQUAL("hoge failed.(1)",string(error.what()));
        CHECK_EQUAL(1,hist.calls().size());
        CHECK_EQUAL(call("pt().GetLastError"),hist.calls().at(0));
    }
}

}
