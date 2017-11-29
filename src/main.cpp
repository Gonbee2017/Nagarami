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
    pt=make_shared<Port>();
    pt->import();
    ps=make_shared<Properties>();
    ct=make_shared<Context>();
    return execute(instance,previousInstance,commandLine,commandShow);
}
