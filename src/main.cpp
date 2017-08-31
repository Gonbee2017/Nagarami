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
    return nagarami::execute
    (instance,previousInstance,commandLine,commandShow);
}
