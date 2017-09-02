#include"nagarami.h"
#include<fstream>
#include<functional>
#include<ios>
#include<iostream>
#include<map>
#include<memory>
#include"resource.h"
#include<stdexcept>
#include<string>
#include<utility>
#include<vector>
#include<windows.h>

namespace nagarami
{

properties::properties()
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
    result.push_back(describe_with("=","alpha",(UINT)alpha));
    result.push_back(describe_with("=","back_color1",back_color1));
    result.push_back(describe_with("=","back_color2",back_color2));
    result.push_back
    (describe_with("=","component_color1",component_color1));
    result.push_back
    (describe_with("=","component_color2",component_color2));
    result.push_back
    (describe_with("=","control_mode_alt",control_mode_alt));
    result.push_back
    (describe_with("=","control_mode_ctrl",control_mode_ctrl));
    result.push_back
    (describe_with("=","control_mode_shift",control_mode_shift));
    result.push_back(describe_with("=","fps",fps));
    result.push_back(describe_with("=","halftone",halftone));
    result.push_back(describe_with("=","hole",hole));
    result.push_back(describe_with("=","scale",scale));
    result.push_back(describe_with("=","view_base.x",view_base.x));
    result.push_back(describe_with("=","view_base.y",view_base.y));
    result.push_back(describe_with("=","window_pos.x",window_pos.x));
    result.push_back(describe_with("=","window_pos.y",window_pos.y));
    result.push_back(describe_with("=","window_size.cx",window_size.cx));
    result.push_back(describe_with("=","window_size.cy",window_size.cy));
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
            {alpha=integer(value);}
        },
        {
            "back_color1",
            [this] (const string&value)
            {back_color1=integer(value);}
        },
        {
            "back_color2",
            [this] (const string&value)
            {back_color2=integer(value);}
        },
        {
            "component_color1",
            [this] (const string&value)
            {component_color1=integer(value);}
        },
        {
            "component_color2",
            [this] (const string&value)
            {component_color2=integer(value);}
        },
        {
            "control_mode_alt",
            [this] (const string&value)
            {control_mode_alt=integer(value);}
        },
        {
            "control_mode_ctrl",
            [this] (const string&value)
            {control_mode_ctrl=integer(value);}
        },
        {
            "control_mode_shift",
            [this] (const string&value)
            {control_mode_shift=integer(value);}
        },
        {
            "fps",
            [this] (const string&value)
            {fps=integer(value);}
        },
        {
            "halftone",
            [this] (const string&value)
            {halftone=integer(value);}
        },
        {
            "hole",
            [this] (const string&value)
            {hole=integer(value);}
        },
        {
            "scale",
            [this] (const string&value)
            {scale=integer(value);}
        },
        {
            "view_base.x",
            [this] (const string&value)
            {view_base.x=fpn(value);}
        },
        {
            "view_base.y",
            [this] (const string&value)
            {view_base.y=fpn(value);}
        },
        {
            "window_pos.x",
            [this] (const string&value)
            {window_pos.x=integer(value);}
        },
        {
            "window_pos.y",
            [this] (const string&value)
            {window_pos.y=integer(value);}
        },
        {
            "window_size.cx",
            [this] (const string&value)
            {window_size.cx=integer(value);}
        },
        {
            "window_size.cy",
            [this] (const string&value)
            {window_size.cy=integer(value);}
        },
    };
    if(setPropertyMap.find(name)!=setPropertyMap.end())
        setPropertyMap.at(name)(value);
}

context::context()
{
    almost_black_brush=nagarami::CreateSolidBrush(ALMOST_BLACK_COLOR);
    back_brush1=nagarami::CreateSolidBrush(ps.back_color1);
    back_brush2=nagarami::CreateSolidBrush(ps.back_color2);
    black_pen=nagarami::CreatePen(PS_SOLID,0,BLACK_COLOR);
    black_brush=nagarami::CreateSolidBrush(BLACK_COLOR);
    component_pen1=nagarami::CreatePen(PS_SOLID,0,ps.component_color1);
    component_brush1=nagarami::CreateSolidBrush(ps.component_color1);
    component_pen2=nagarami::CreatePen(PS_SOLID,0,ps.component_color2);
    component_brush2=nagarami::CreateSolidBrush(ps.component_color2);
    font=nagarami::CreateFont
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
    target=NULL;
    white_brush=nagarami::CreateSolidBrush(WHITE_COLOR);
}

context&ct()
{
    static context context_;
    return context_;
}

bool control_mode()
{
    return
        (!ct().ps.control_mode_alt||GetKeyState(VK_MENU)<0)&&
        (!ct().ps.control_mode_ctrl||GetKeyState(VK_CONTROL)<0)&&
        (!ct().ps.control_mode_shift||GetKeyState(VK_SHIFT)<0);
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
        describe(VER_PRODUCTNAME,".",PROPERTIES_FILE_EXTENSION);
    int result=0;
    try
    {
        ct().instance=instance;
        ifstream psIn(psFile,ios::in);
        if(psIn)
        {
            auto lines=getlines(psIn);
            ct().ps.load(lines.begin(),lines.end());
        }
        auto expressions=tokenize(commandLine," ");
        ct().ps.load(expressions.begin(),expressions.end());
        auto main_window=make_shared<MainWindow>();
        ShowWindow(main_window->handle(),commandShow);
        nagarami::UpdateWindow(main_window->handle());
        auto timer=
            make_shared<Timer>(1000/ct().ps.fps,main_window->handle());
        MSG msg;
        while(nagarami::GetMessage(&msg,NULL,0,0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            if(ct().error.get()) throw ct().error;
        }
        result=msg.wParam;
        ofstream psOut(psFile,ios::out|ios::trunc);
        if(psOut) putlines(psOut,ct().ps.lines());
    } catch(const shared_ptr<runtime_error>&error)
    {
        const string message=describe
        ("Unexpected error occured.\n",error->what());
        cerr<<message<<endl;
        MessageBox
        (
            NULL,
            TEXT(message.c_str()),
            TEXT(VER_PRODUCTNAME),
            MB_ICONERROR|MB_OK
        );
        result=1;
    }
    return result;
}

}
