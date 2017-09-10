#include"nagarami.h"
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
    pt.import();
    return execute(instance,previousInstance,commandLine,commandShow);
}
