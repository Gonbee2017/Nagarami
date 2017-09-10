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

properties::properties()
{
    value_setter_map_["alpha"]=[this] (const string&value)
    {try {alpha=integer(value);} catch(...) {}};
    value_setter_map_["back_color1"]=[this] (const string&value)
    {try {back_color1=integer(value);} catch(...) {}};
    value_setter_map_["back_color2"]=[this] (const string&value)
    {try {back_color2=integer(value);} catch(...) {}};
    value_setter_map_["component_color1"]=[this] (const string&value)
    {try {component_color1=integer(value);} catch(...) {}};
    value_setter_map_["component_color2"]=[this] (const string&value)
    {try {component_color2=integer(value);} catch(...) {}};
    value_setter_map_["control_mode_alt"]=[this] (const string&value)
    {try {control_mode_alt=integer(value);} catch(...) {}};
    value_setter_map_["control_mode_ctrl"]=[this] (const string&value)
    {try {control_mode_ctrl=integer(value);} catch(...) {}};
    value_setter_map_["control_mode_shift"]=[this] (const string&value)
    {try {control_mode_shift=integer(value);} catch(...) {}};
    value_setter_map_["fps"]=[this] (const string&value)
    {try {fps=integer(value);} catch(...) {}};
    value_setter_map_["halftone"]=[this] (const string&value)
    {try {halftone=integer(value);} catch(...) {}};
    value_setter_map_["hole"]=[this] (const string&value)
    {try {hole=integer(value);} catch(...) {}};
    value_setter_map_["scale"]=[this] (const string&value)
    {try {scale=integer(value);} catch(...) {}};
    value_setter_map_["view_base.x"]=[this] (const string&value)
    {try {view_base.x=floating_point_number(value);} catch(...) {}};
    value_setter_map_["view_base.y"]=[this] (const string&value)
    {try {view_base.y=floating_point_number(value);} catch(...) {}};
    value_setter_map_["window_pos.x"]=[this] (const string&value)
    {try {window_pos.x=integer(value);} catch(...) {}};
    value_setter_map_["window_pos.y"]=[this] (const string&value)
    {try {window_pos.y=integer(value);} catch(...) {}};
    value_setter_map_["window_size.cx"]=[this] (const string&value)
    {try {window_size.cx=integer(value);} catch(...) {}};
    value_setter_map_["window_size.cy"]=[this] (const string&value)
    {try {window_size.cy=integer(value);} catch(...) {}};
}

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

void properties::load(const vector<string>&expressions)
{
    for(const string&expression:expressions)
    {
        string name,value;
        decompose(parse(expression),&name,&value);
        const auto setter=value_setter_map_.find(name);
        if(setter!=value_setter_map_.end()) setter->second(value);
    }
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

context::context():error("") {}

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
    this->BeginPaint=nullptr;
    this->BitBlt=nullptr;
    this->CreateCompatibleBitmap=nullptr;
    this->CreateCompatibleDC=nullptr;
    this->CreateFont=nullptr;
    this->CreatePatternBrush=nullptr;
    this->CreatePen=nullptr;
    this->CreateSolidBrush=nullptr;
    this->CreateWindowEx=nullptr;
    this->DefWindowProc=nullptr;
    this->DeleteDC=nullptr;
    this->DeleteObject=nullptr;
    this->DestroyWindow=nullptr;
    this->DispatchMessage=nullptr;
    this->DrawText=nullptr;
    this->Ellipse=nullptr;
    this->EndPaint=nullptr;
    this->FillRect=nullptr;
    this->GetClientRect=nullptr;
    this->GetCursorPos=nullptr;
    this->GetDC=nullptr;
    this->GetForegroundWindow=nullptr;
    this->GetKeyState=nullptr;
    this->GetLastError=nullptr;
    this->GetMessage=nullptr;
    this->GetObject=nullptr;
    this->GetSystemMetrics=nullptr;
    this->GetWindowPlacement=nullptr;
    this->IsIconic=nullptr;
    this->IsWindow=nullptr;
    this->IsWindowVisible=nullptr;
    this->IsZoomed=nullptr;
    this->LoadBitmap=nullptr;
    this->LoadCursor=nullptr;
    this->MessageBox=nullptr;
    this->PostMessage=nullptr;
    this->PostQuitMessage=nullptr;
    this->RedrawWindow=nullptr;
    this->ReleaseDC=nullptr;
    this->RegisterClassEx=nullptr;
    this->ReleaseCapture=nullptr;
    this->ScreenToClient=nullptr;
    this->SelectObject=nullptr;
    this->SendMessageW=nullptr;
    this->SetBkMode=nullptr;
    this->SetBrushOrgEx=nullptr;
    this->SetCapture=nullptr;
    this->SetCursor=nullptr;
    this->SetForegroundWindow=nullptr;
    this->SetLayeredWindowAttributes=nullptr;
    this->SetStretchBltMode=nullptr;
    this->SetTextColor=nullptr;
    this->ShellExecute=nullptr;
    this->ShowWindow=nullptr;
    this->StretchBlt=nullptr;
    this->input_file=nullptr;
    this->output_file=nullptr;
    this->timeBeginPeriod=nullptr;
    this->timeEndPeriod=nullptr;
    this->timeGetDevCaps=nullptr;
    this->timeKillEvent=nullptr;
    this->timeSetEvent=nullptr;
}

void port::import()
{
    this->BeginPaint=&::BeginPaint;
    this->BitBlt=&::BitBlt;
    this->CreateCompatibleBitmap=&::CreateCompatibleBitmap;
    this->CreateCompatibleDC=&::CreateCompatibleDC;
    this->CreateFont=&::CreateFont;
    this->CreatePatternBrush=&::CreatePatternBrush;
    this->CreatePen=&::CreatePen;
    this->CreateSolidBrush=&::CreateSolidBrush;
    this->CreateWindowEx=&::CreateWindowEx;
    this->DefWindowProc=&::DefWindowProc;
    this->DeleteDC=&::DeleteDC;
    this->DeleteObject=&::DeleteObject;
    this->DestroyWindow=&::DestroyWindow;
    this->DispatchMessage=&::DispatchMessage;
    this->DrawText=&::DrawText;
    this->Ellipse=&::Ellipse;
    this->EndPaint=&::EndPaint;
    this->FillRect=&::FillRect;
    this->GetClientRect=&::GetClientRect;
    this->GetCursorPos=&::GetCursorPos;
    this->GetDC=&::GetDC;
    this->GetForegroundWindow=&::GetForegroundWindow;
    this->GetKeyState=&::GetKeyState;
    this->GetLastError=&::GetLastError;
    this->GetMessage=&::GetMessage;
    this->GetObject=&::GetObject;
    this->GetSystemMetrics=&::GetSystemMetrics;
    this->GetWindowPlacement=&::GetWindowPlacement;
    this->IsIconic=&::IsIconic;
    this->IsWindow=&::IsWindow;
    this->IsWindowVisible=&::IsWindowVisible;
    this->IsZoomed=&::IsZoomed;
    this->LoadBitmap=&::LoadBitmap;
    this->LoadCursor=&::LoadCursor;
    this->MessageBox=&::MessageBox;
    this->PostMessage=&::PostMessage;
    this->PostQuitMessage=&::PostQuitMessage;
    this->RedrawWindow=&::RedrawWindow;
    this->ReleaseDC=&::ReleaseDC;
    this->RegisterClassEx=&::RegisterClassEx;
    this->ReleaseCapture=&::ReleaseCapture;
    this->ScreenToClient=&::ScreenToClient;
    this->SelectObject=&::SelectObject;
    this->SendMessageW=&::SendMessageW;
    this->SetBkMode=&::SetBkMode;
    this->SetBrushOrgEx=&::SetBrushOrgEx;
    this->SetCapture=&::SetCapture;
    this->SetCursor=&::SetCursor;
    this->SetForegroundWindow=&::SetForegroundWindow;
    this->SetLayeredWindowAttributes=&::SetLayeredWindowAttributes;
    this->SetStretchBltMode=&::SetStretchBltMode;
    this->SetTextColor=&::SetTextColor;
    this->ShellExecute=&::ShellExecute;
    this->ShowWindow=&::ShowWindow;
    this->StretchBlt=&::StretchBlt;
    this->input_file=&nm::input_file;
    this->output_file=&nm::output_file;
    this->timeBeginPeriod=&::timeBeginPeriod;
    this->timeEndPeriod=&::timeEndPeriod;
    this->timeGetDevCaps=&::timeGetDevCaps;
    this->timeKillEvent=&::timeKillEvent;
    this->timeSetEvent=&::timeSetEvent;
}

bool control_mode()
{
    return
        (!ct.ps.control_mode_alt||pt.GetKeyState(VK_MENU)<0)&&
        (!ct.ps.control_mode_ctrl||pt.GetKeyState(VK_CONTROL)<0)&&
        (!ct.ps.control_mode_shift||pt.GetKeyState(VK_SHIFT)<0);
}

int execute
(
    HINSTANCE instance,
    HINSTANCE previousInstance,
    LPSTR commandLine,
    int commandShow
)
{
    int result=0;
    try
    {
        ct.initialize(instance);
        load_properties(commandLine);
        ct.ps.adjust();
        Window::registerClass();
        const auto main_window=make_shared<MainWindow>();
        pt.ShowWindow(main_window->handle(),commandShow);
        const auto timePeriod=nm::timeBeginPeriod(minimum_time_period());
        const auto timer=make_shared<Timer>
        (1000/ct.ps.fps,timePeriod->value(),main_window->handle());
        MSG msg;
        while(nm::GetMessage(&msg,NULL,0,0)) nm::DispatchMessage(&msg);
        save_properties();
        result=msg.wParam;
    } catch(const runtime_error&error)
    {
        pt.MessageBox
        (
            NULL,
            TEXT(describe
            (
                "An unexpected error occured.\n",
                error.what()
            ).c_str()),
            TEXT(APPLICATION_NAME),
            MB_ICONERROR|MB_OK
        );
        result=1;
    }
    return result;
}

void load_properties(const string&commandLine)
{
    const auto psIS=pt.input_file(PS_FILE_NAME,ios::in);
    if(*psIS) ct.ps.load(getlines(*psIS));
    ct.ps.load(tokenize(commandLine," "));
}

UINT minimum_time_period()
{
    TIMECAPS timeCaps;
    nm::timeGetDevCaps(&timeCaps,sizeof(TIMECAPS));
    return timeCaps.wPeriodMin;
}

void save_properties()
{
    const auto psOS=pt.output_file(PS_FILE_NAME,ios::out|ios::trunc);
    if(*psOS) putlines(*psOS,ct.ps.lines());
}

const string PS_FILE_NAME=describe(APPLICATION_NAME,".",PS_FILE_EXTENSION);
context ct;
port pt;

}
