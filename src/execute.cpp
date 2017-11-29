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

Context::Context():error("") {}

void Context::initialize(HINSTANCE instance)
{
    backBrush1=nm::CreateSolidBrush(ps->backColor1);
    backBrush2=nm::CreateSolidBrush(ps->backColor2);
    blackPen=nm::CreatePen(PS_SOLID,0,BLACK_COLOR);
    blackBrush=nm::CreateSolidBrush(BLACK_COLOR);
    componentBrush1=nm::CreateSolidBrush(ps->componentColor1);
    componentBrush2=nm::CreateSolidBrush(ps->componentColor2);
    componentPen1=nm::CreatePen(PS_SOLID,0,ps->componentColor1);
    componentPen2=nm::CreatePen(PS_SOLID,0,ps->componentColor2);
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
    opaqueBrush=nm::CreateSolidBrush(OPAQUE_COLOR);
    target=NULL;
    whiteBrush=nm::CreateSolidBrush(WHITE_COLOR);
}

void Port::import()
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
    this->GetPixel=&::GetPixel;
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

Properties::Properties()
{
    setter_map_["alpha"]=[this] (const string&value)
    {integer(&alpha,value);};
    setter_map_["backColor1"]=[this] (const string&value)
    {integer(&backColor1,value);};
    setter_map_["backColor2"]=[this] (const string&value)
    {integer(&backColor2,value);};
    setter_map_["componentColor1"]=[this] (const string&value)
    {integer(&componentColor1,value);};
    setter_map_["componentColor2"]=[this] (const string&value)
    {integer(&componentColor2,value);};
    setter_map_["controlModeAlt"]=[this] (const string&value)
    {integer(&controlModeAlt,value);};
    setter_map_["controlModeCtrl"]=[this] (const string&value)
    {integer(&controlModeCtrl,value);};
    setter_map_["controlModeShift"]=[this] (const string&value)
    {integer(&controlModeShift,value);};
    setter_map_["fps"]=[this] (const string&value)
    {integer(&fps,value);};
    setter_map_["halftone"]=[this] (const string&value)
    {integer(&halftone,value);};
    setter_map_["hole"]=[this] (const string&value)
    {integer(&hole,value);};
    setter_map_["psFileName"]=[this] (const string&value)
    {psFileName=value;};
    setter_map_["scale"]=[this] (const string&value)
    {integer(&scale,value);};
    setter_map_["viewBase.x"]=[this] (const string&value)
    {floating_point_number(&viewBase.x,value);};
    setter_map_["viewBase.y"]=[this] (const string&value)
    {floating_point_number(&viewBase.y,value);};
    setter_map_["windowPos.x"]=[this] (const string&value)
    {integer(&windowPos.x,value);};
    setter_map_["windowPos.y"]=[this] (const string&value)
    {integer(&windowPos.y,value);};
    setter_map_["windowSize.cx"]=[this] (const string&value)
    {integer(&windowSize.cx,value);};
    setter_map_["windowSize.cy"]=[this] (const string&value)
    {integer(&windowSize.cy,value);};
}

void Properties::adjust()
{
    fps=min(max(fps,(UINT)1),(UINT)60);
    hole=min(max(hole,(LONG)0),MAXIMUM_HOLE);
    scale=min(max(scale,(LONG)0),MAXIMUM_SCALE);
    const SIZE desktopSize=desktop_size();
    windowSize.cx=
        min(max(windowSize.cx,MINIMUM_WINDOW_WIDTH),desktopSize.cx);
    windowSize.cy=
        min(max(windowSize.cy,MINIMUM_WINDOW_HEIGHT),desktopSize.cy);
    windowPos.x=min(max(windowPos.x,-windowSize.cx+1),desktopSize.cx-1);
    windowPos.y=min(max(windowPos.y,-windowSize.cy+1),desktopSize.cy-1);
}

void Properties::initialize()
{
    alpha=DEFAULT_ALPHA;
    backColor1=DEFAULT_BACK_COLOR1;
    backColor2=DEFAULT_BACK_COLOR2;
    componentColor1=DEFAULT_COMPONENT_COLOR1;
    componentColor2=DEFAULT_COMPONENT_COLOR2;
    controlModeAlt=DEFAULT_CONTROL_MODE_ALT;
    controlModeCtrl=DEFAULT_CONTROL_MODE_CTRL;
    controlModeShift=DEFAULT_CONTROL_MODE_SHIFT;
    fps=DEFAULT_FPS;
    halftone=DEFAULT_HALFTONE;
    hole=DEFAULT_HOLE;
    psFileName=
        describe(DEFAULT_PS_FILE_BASE,'.',DEFAULT_PS_FILE_EXTENSION);
    scale=DEFAULT_SCALE;
    viewBase=POINT_DOUBLE({0,0});
    const SIZE desktopSize=desktop_size();
    windowSize=desktopSize/DEFAULT_WINDOW_SIZE_DIVISOR;
    windowPos=point(desktopSize-windowSize)/2;
}

vector<string> Properties::lines() const
{
    vector<string> result;
    result.push_back(describe("alpha","=",(UINT)alpha));
    result.push_back(describe
    ("backColor1","=",hex,showbase,backColor1));
    result.push_back(describe
    ("backColor2","=",hex,showbase,backColor2));
    result.push_back(describe
    ("componentColor1","=",hex,showbase,componentColor1));
    result.push_back(describe
    ("componentColor2","=",hex,showbase,componentColor2));
    result.push_back(describe
    ("controlModeAlt","=",controlModeAlt));
    result.push_back(describe
    ("controlModeCtrl","=",controlModeCtrl));
    result.push_back(describe
    ("controlModeShift","=",controlModeShift));
    result.push_back(describe("fps","=",fps));
    result.push_back(describe("halftone","=",halftone));
    result.push_back(describe("hole","=",hole));
    result.push_back(describe("scale","=",scale));
    result.push_back(describe("viewBase.x","=",viewBase.x));
    result.push_back(describe("viewBase.y","=",viewBase.y));
    result.push_back(describe("windowPos.x","=",windowPos.x));
    result.push_back(describe("windowPos.y","=",windowPos.y));
    result.push_back(describe("windowSize.cx","=",windowSize.cx));
    result.push_back(describe("windowSize.cy","=",windowSize.cy));
    return result;
}

void Properties::load(const vector<string>&expressions)
{
    for(const string&expression:expressions)
    {
        string name,value;
        decompose(parse(expression),&name,&value);
        const auto setter=setter_map_.find(name);
        if(setter!=setter_map_.end()) setter->second(value);
    }
}

pair<string,string> Properties::parse(const string&expression)
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

bool control_mode()
{
    return
        (!ps->controlModeAlt||pt->GetKeyState(VK_MENU)<0)&&
        (!ps->controlModeCtrl||pt->GetKeyState(VK_CONTROL)<0)&&
        (!ps->controlModeShift||pt->GetKeyState(VK_SHIFT)<0);
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
        ps->initialize();
        ct->initialize(instance);
        load_properties(commandLine);
        ps->adjust();
        Window::registerClass();
        const auto main_window=make_shared<MainWindow>();
        pt->ShowWindow(main_window->handle(),commandShow);
        const auto timePeriod=nm::timeBeginPeriod(minimum_time_period());
        const auto timer=make_shared<Timer>
        (1000/ps->fps,timePeriod->value(),main_window->handle());
        MSG msg;
        while(nm::GetMessage(&msg,NULL,0,0)) nm::DispatchMessage(&msg);
        save_properties();
        result=msg.wParam;
    } catch(const runtime_error&error)
    {
        pt->MessageBox
        (
            NULL,
            describe
            (
                "An unexpected error occured.\n",
                error.what()
            ).c_str(),
            APPLICATION_NAME,
            MB_ICONERROR|MB_OK
        );
        result=1;
    }
    return result;
}

void load_properties(const string&commandLine)
{
    ps->load(tokenize(commandLine," "));
    const auto psIS=pt->input_file(ps->psFileName,ios::in);
    if(*psIS) ps->load(get_lines(*psIS));
}

UINT minimum_time_period()
{
    TIMECAPS timeCaps;
    nm::timeGetDevCaps(&timeCaps,sizeof(TIMECAPS));
    return timeCaps.wPeriodMin;
}

void save_properties()
{
    const auto psOS=pt->output_file(ps->psFileName,ios::out|ios::trunc);
    if(*psOS) put_lines(*psOS,ps->lines());
}

shared_ptr<Port> pt;
shared_ptr<Properties> ps;
shared_ptr<Context> ct;

}
