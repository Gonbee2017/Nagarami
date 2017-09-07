#include<algorithm>
#include<functional>
#include<iomanip>
#include<ios>
#include<map>
#include<memory>
#include"nagarami.h"
#include<stdexcept>
#include<string>
#include<utility>
#include<vector>
#include<windows.h>

namespace nm
{
void properties::adjust()
{
    fps=min(max(fps,(UINT)1),(UINT)60);
    hole=min(max(hole,(LONG)0),MAXIMUM_HOLE);
    scale=min(max(scale,(LONG)0),MAXIMUM_SCALE);
    const SIZE desktopSize=desktop_size();
    window_size.cx=
        min(max(window_size.cx,MINIMUM_WINDOW_WIDTH),desktopSize.cx);
    window_size.cy=
        min(max(window_size.cy,MINIMUM_WINDOW_WIDTH),desktopSize.cy);
    window_pos.x=min(max(window_pos.x,-window_size.cx+1),desktopSize.cx-1);
    window_pos.y=min(max(window_pos.y,-window_size.cy+1),desktopSize.cy-1);
}

void properties::initialize()
{
    alpha=DEFAULT_ALPHA;
    back_color1=DEFAULT_BACK_COLOR1;
    back_color2=DEFAULT_BACK_COLOR2;
    component_color1=DEFAULT_COMPONENT_COLOR1;
    component_color2=DEFAULT_COMPONENT_COLOR2;
    control_mode_alt=DEFAULT_CONTROL_MODE_ALT;
    control_mode_ctrl=DEFAULT_CONTROL_MODE_CTRL;
    control_mode_shift=DEFAULT_CONTROL_MODE_SHIFT;
    fps=DEFAULT_FPS;
    halftone=DEFAULT_HALFTONE;
    hole=DEFAULT_HOLE;
    scale=DEFAULT_SCALE;
    view_base=POINT_DOUBLE({0,0});
    const SIZE desktopSize=desktop_size();
    window_size=desktopSize/DEFAULT_WINDOW_SIZE_DIVISOR;
    window_pos=point(desktopSize-window_size)/2;
}

vector<string> properties::lines() const
{
    vector<string> result;
    result.push_back(describe("alpha","=",(UINT)alpha));
    result.push_back(describe
    ("back_color1","=",hex,showbase,back_color1));
    result.push_back(describe
    ("back_color2","=",hex,showbase,back_color2));
    result.push_back(describe
    ("component_color1","=",hex,showbase,component_color1));
    result.push_back(describe
    ("component_color2","=",hex,showbase,component_color2));
    result.push_back(describe
    ("control_mode_alt","=",control_mode_alt));
    result.push_back(describe
    ("control_mode_ctrl","=",control_mode_ctrl));
    result.push_back(describe
    ("control_mode_shift","=",control_mode_shift));
    result.push_back(describe("fps","=",fps));
    result.push_back(describe("halftone","=",halftone));
    result.push_back(describe("hole","=",hole));
    result.push_back(describe("scale","=",scale));
    result.push_back(describe("view_base.x","=",view_base.x));
    result.push_back(describe("view_base.y","=",view_base.y));
    result.push_back(describe("window_pos.x","=",window_pos.x));
    result.push_back(describe("window_pos.y","=",window_pos.y));
    result.push_back(describe("window_size.cx","=",window_size.cx));
    result.push_back(describe("window_size.cy","=",window_size.cy));
    return result;
}

pair<string,string> properties::parse(const string&expression)
{
    const string::size_type pos=expression.find('=');
    string name,value;
    if(pos!=string::npos)
    {
        name=expression.substr(0,pos);
        value=expression.substr(pos+1);
    } else name=expression;
    return make_pair(name,value);
}

void properties::set(const string&name,const string&value)
{
    using SET_PROPERTY=function<void(const string&value)>;
    static const map<string,SET_PROPERTY> setPropertyMap=
    {
        {
            "alpha",
            [this] (const string&value)
            {try {alpha=integer(value);} catch(...) {}}
        },
        {
            "back_color1",
            [this] (const string&value)
            {try {back_color1=integer(value);} catch(...) {}}
        },
        {
            "back_color2",
            [this] (const string&value)
            {try {back_color2=integer(value);} catch(...) {}}
        },
        {
            "component_color1",
            [this] (const string&value)
            {try {component_color1=integer(value);} catch(...) {}}
        },
        {
            "component_color2",
            [this] (const string&value)
            {try {component_color2=integer(value);} catch(...) {}}
        },
        {
            "control_mode_alt",
            [this] (const string&value)
            {try {control_mode_alt=integer(value);} catch(...) {}}
        },
        {
            "control_mode_ctrl",
            [this] (const string&value)
            {try {control_mode_ctrl=integer(value);} catch(...) {}}
        },
        {
            "control_mode_shift",
            [this] (const string&value)
            {try {control_mode_shift=integer(value);} catch(...) {}}
        },
        {
            "fps",
            [this] (const string&value)
            {try {fps=integer(value);} catch(...) {}}
        },
        {
            "halftone",
            [this] (const string&value)
            {try {halftone=integer(value);} catch(...) {}}
        },
        {
            "hole",
            [this] (const string&value)
            {try {hole=integer(value);} catch(...) {}}
        },
        {
            "scale",
            [this] (const string&value)
            {try {scale=integer(value);} catch(...) {}}
        },
        {
            "view_base.x",
            [this] (const string&value)
            {try {view_base.x=floating_point_number(value);} catch(...) {}}
        },
        {
            "view_base.y",
            [this] (const string&value)
            {try {view_base.y=floating_point_number(value);} catch(...) {}}
        },
        {
            "window_pos.x",
            [this] (const string&value)
            {try {window_pos.x=integer(value);} catch(...) {}}
        },
        {
            "window_pos.y",
            [this] (const string&value)
            {try {window_pos.y=integer(value);} catch(...) {}}
        },
        {
            "window_size.cx",
            [this] (const string&value)
            {try {window_size.cx=integer(value);} catch(...) {}}
        },
        {
            "window_size.cy",
            [this] (const string&value)
            {try {window_size.cy=integer(value);} catch(...) {}}
        },
    };
    if(setPropertyMap.find(name)!=setPropertyMap.end())
        setPropertyMap.at(name)(value);
}

void context::initialize(HINSTANCE instance)
{
    ps.initialize();
    almost_black_brush=nm::CreateSolidBrush(ALMOST_BLACK_COLOR);
    back_brush1=nm::CreateSolidBrush(ps.back_color1);
    back_brush2=nm::CreateSolidBrush(ps.back_color2);
    black_pen=nm::CreatePen(PS_SOLID,0,BLACK_COLOR);
    black_brush=nm::CreateSolidBrush(BLACK_COLOR);
    component_pen1=nm::CreatePen(PS_SOLID,0,ps.component_color1);
    component_brush1=nm::CreateSolidBrush(ps.component_color1);
    component_pen2=nm::CreatePen(PS_SOLID,0,ps.component_color2);
    component_brush2=nm::CreateSolidBrush(ps.component_color2);
    font=nm::CreateFont
    (
        UNIT_LENGTH-4,
        0,
        0,
        0,
        FW_BOLD,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        FF_MODERN|FIXED_PITCH,
        NULL
    );
    this->instance=instance;
    target=NULL;
    white_brush=nm::CreateSolidBrush(WHITE_COLOR);
}

void port::clear()
{
    BeginPaint=nullptr;
    BitBlt=nullptr;
    CreateCompatibleBitmap=nullptr;
    CreateCompatibleDC=nullptr;
    CreateFont=nullptr;
    CreatePatternBrush=nullptr;
    CreatePen=nullptr;
    CreateSolidBrush=nullptr;
    CreateWindowEx=nullptr;
    DefWindowProc=nullptr;
    DeleteDC=nullptr;
    DeleteObject=nullptr;
    DestroyWindow=nullptr;
    DispatchMessage=nullptr;
    DrawText=nullptr;
    Ellipse=nullptr;
    EndPaint=nullptr;
    FillRect=nullptr;
    GetClientRect=nullptr;
    GetCursorPos=nullptr;
    GetDC=nullptr;
    GetForegroundWindow=nullptr;
    GetKeyState=nullptr;
    GetLastError=nullptr;
    GetMessage=nullptr;
    GetObject=nullptr;
    GetSystemMetrics=nullptr;
    GetWindowPlacement=nullptr;
    IsIconic=nullptr;
    IsWindow=nullptr;
    IsWindowVisible=nullptr;
    IsZoomed=nullptr;
    LoadBitmap=nullptr;
    LoadCursor=nullptr;
    MessageBox=nullptr;
    PostQuitMessage=nullptr;
    RedrawWindow=nullptr;
    ReleaseDC=nullptr;
    RegisterClassEx=nullptr;
    ReleaseCapture=nullptr;
    ScreenToClient=nullptr;
    SelectObject=nullptr;
    SendMessageW=nullptr;
    SetBkMode=nullptr;
    SetBrushOrgEx=nullptr;
    SetCapture=nullptr;
    SetCursor=nullptr;
    SetForegroundWindow=nullptr;
    SetLayeredWindowAttributes=nullptr;
    SetStretchBltMode=nullptr;
    SetTextColor=nullptr;
    ShellExecute=nullptr;
    ShowWindow=nullptr;
    StretchBlt=nullptr;
    input_file=nullptr;
    output_file=nullptr;
    timeBeginPeriod=nullptr;
    timeEndPeriod=nullptr;
    timeGetDevCaps=nullptr;
    timeKillEvent=nullptr;
    timeSetEvent=nullptr;
}

void port::import()
{
    BeginPaint=&::BeginPaint;
    BitBlt=&::BitBlt;
    CreateCompatibleBitmap=&::CreateCompatibleBitmap;
    CreateCompatibleDC=&::CreateCompatibleDC;
    CreateFont=&::CreateFont;
    CreatePatternBrush=&::CreatePatternBrush;
    CreatePen=&::CreatePen;
    CreateSolidBrush=&::CreateSolidBrush;
    CreateWindowEx=&::CreateWindowEx;
    DefWindowProc=&::DefWindowProc;
    DeleteDC=&::DeleteDC;
    DeleteObject=&::DeleteObject;
    DestroyWindow=&::DestroyWindow;
    DispatchMessage=&::DispatchMessage;
    DrawText=&::DrawText;
    Ellipse=&::Ellipse;
    EndPaint=&::EndPaint;
    FillRect=&::FillRect;
    GetClientRect=&::GetClientRect;
    GetCursorPos=&::GetCursorPos;
    GetDC=&::GetDC;
    GetForegroundWindow=&::GetForegroundWindow;
    GetKeyState=&::GetKeyState;
    GetLastError=&::GetLastError;
    GetMessage=&::GetMessage;
    GetObject=&::GetObject;
    GetSystemMetrics=&::GetSystemMetrics;
    GetWindowPlacement=&::GetWindowPlacement;
    IsIconic=&::IsIconic;
    IsWindow=&::IsWindow;
    IsWindowVisible=&::IsWindowVisible;
    IsZoomed=&::IsZoomed;
    LoadBitmap=&::LoadBitmap;
    LoadCursor=&::LoadCursor;
    MessageBox=&::MessageBox;
    PostQuitMessage=&::PostQuitMessage;
    RedrawWindow=&::RedrawWindow;
    ReleaseDC=&::ReleaseDC;
    RegisterClassEx=&::RegisterClassEx;
    ReleaseCapture=&::ReleaseCapture;
    ScreenToClient=&::ScreenToClient;
    SelectObject=&::SelectObject;
    SendMessageW=&::SendMessageW;
    SetBkMode=&::SetBkMode;
    SetBrushOrgEx=&::SetBrushOrgEx;
    SetCapture=&::SetCapture;
    SetCursor=&::SetCursor;
    SetForegroundWindow=&::SetForegroundWindow;
    SetLayeredWindowAttributes=&::SetLayeredWindowAttributes;
    SetStretchBltMode=&::SetStretchBltMode;
    SetTextColor=&::SetTextColor;
    ShellExecute=&::ShellExecute;
    ShowWindow=&::ShowWindow;
    StretchBlt=&::StretchBlt;
    input_file=&nm::input_file;
    output_file=&nm::output_file;
    timeBeginPeriod=&::timeBeginPeriod;
    timeEndPeriod=&::timeEndPeriod;
    timeGetDevCaps=&::timeGetDevCaps;
    timeKillEvent=&::timeKillEvent;
    timeSetEvent=&::timeSetEvent;
}

context&ct()
{
    static context instance;
    return instance;
}

bool control_mode()
{
    return
        (!ct().ps.control_mode_alt||pt().GetKeyState(VK_MENU)<0)&&
        (!ct().ps.control_mode_ctrl||pt().GetKeyState(VK_CONTROL)<0)&&
        (!ct().ps.control_mode_shift||pt().GetKeyState(VK_SHIFT)<0);
}

int execute
(
    HINSTANCE instance,
    HINSTANCE previousInstance,
    LPSTR commandLine,
    int commandShow
)
{
    static const string psFile=
        describe(APPLICATION_NAME,".",PROPERTIES_FILE_EXTENSION);
    int result=0;
    try
    {
        ct().initialize(instance);
        auto psIn=pt().input_file(psFile,ios_base::in);
        if(*psIn)
        {
            auto lines=getlines(*psIn);
            ct().ps.load(lines.begin(),lines.end());
        }
        auto arguments=tokenize(commandLine," ");
        ct().ps.load(arguments.begin(),arguments.end());
        ct().ps.adjust();
        auto main_window=make_shared<MainWindow>();
        pt().ShowWindow(main_window->handle(),commandShow);
        const UINT delay=1000/ct().ps.fps;
        auto timer=make_shared<Timer>(delay,main_window->handle());
        MSG msg;
        while(nm::GetMessage(&msg,NULL,0,0)) nm::DispatchMessage(&msg);
        result=msg.wParam;
        auto psOut=pt().output_file(psFile,ios_base::out|ios_base::trunc);
        if(*psOut) putlines(*psOut,ct().ps.lines());
    } catch(const shared_ptr<runtime_error>&error)
    {
        pt().MessageBox
        (
            NULL,
            TEXT(describe
            (
                "An unexpected error occured.\n",
                error->what()
            ).c_str()),
            TEXT(APPLICATION_NAME),
            MB_ICONERROR|MB_OK
        );
        result=1;
    }
    return result;
}

port&pt()
{
    static port instance;
    return instance;
}

}
