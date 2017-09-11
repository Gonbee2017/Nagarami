#include"nagarami.h"
#include<memory>
#include<windows.h>

using namespace nm;

int WINAPI WinMain
(
    HINSTANCE instance,
    HINSTANCE previousInstance,
    LPSTR commandLine,
    int commandShow
)
{
    pt=make_shared<port>();
    pt->import();
    ct=make_shared<context>();
    int result=execute(instance,previousInstance,commandLine,commandShow);
    ct.reset();
    pt.reset();
    return result;
}
