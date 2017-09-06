#include"nagarami.h"
#include<windows.h>

int WINAPI WinMain
(
    HINSTANCE instance,
    HINSTANCE previousInstance,
    LPSTR commandLine,
    int commandShow
)
{
    nm::import();
    return nm::execute(instance,previousInstance,commandLine,commandShow);
}
