#ifndef NAGARAMI_H
#define NAGARAMI_H

#include<functional>
#include<istream>
#include<map>
#include<memory>
#include<ostream>
#include<sstream>
#include<stdexcept>
#include<string>
#include<vector>
#include<windows.h>
#include<commctrl.h>

#define SQUARE(x) ((x)*(x))
#define WM_USERTIMER (WM_USER+1)

namespace nagarami
{

using namespace std;

//---- constexpr definition ----

constexpr char PROPERTIES_FILE_EXTENSION[]="ps";
constexpr char HELP_URL[]                 =
    "https://github.com/Gonbee2017/Nagarami/blob/master/README.md";

constexpr LONG  UNIT_LENGTH             =16;
constexpr LONG  HALF_UNIT_LENGTH        =UNIT_LENGTH/2;
constexpr SIZE  UNIT_SIZE               =SIZE({UNIT_LENGTH,UNIT_LENGTH});
constexpr RECT  UNIT_RECT               =
    RECT({0,0,UNIT_SIZE.cx,UNIT_SIZE.cy});
constexpr LONG  FRAME_LENGTH            =UNIT_LENGTH;
constexpr POINT CLIENT_POS=POINT({FRAME_LENGTH,FRAME_LENGTH});
constexpr LONG  MINIMUM_WINDOW_WIDTH    =UNIT_LENGTH*9+FRAME_LENGTH*2;
constexpr LONG  MINIMUM_WINDOW_HEIGHT   =UNIT_LENGTH*4+FRAME_LENGTH*2;
constexpr SIZE  MINIMUM_WINDOW_SIZE     =
    SIZE({MINIMUM_WINDOW_WIDTH,MINIMUM_WINDOW_HEIGHT});
constexpr int   SCALE_DIVISOR           =10;
constexpr int   ALPHA_DIVISOR           =20;
constexpr int   MAXIMUM_SCALE           =500;
constexpr int   MAXIMUM_HOLE            =UNIT_LENGTH*20;
constexpr LONG  SLIDER_BAR_WIDTH        =UNIT_LENGTH/4;
constexpr LONG  HALF_SLIDER_BAR_WIDTH   =SLIDER_BAR_WIDTH/2;
constexpr LONG  SLIDER_TEXT_WIDTH       =UNIT_LENGTH*2;
constexpr SIZE  SLIDER_TEXT_SIZE        =
    SIZE({SLIDER_TEXT_WIDTH,UNIT_LENGTH});

constexpr COLORREF ALMOST_BLACK_COLOR=RGB(  1,  1,  1);
constexpr COLORREF BLACK_COLOR       =RGB(  0,  0,  0);
constexpr COLORREF WHITE_COLOR       =RGB(255,255,255);

constexpr POINT ALPHA_SLIDER_CELL_POS     =POINT({ 0,-2});
constexpr POINT CLOSE_BUTTON_CELL_POS     =POINT({-1, 0});
constexpr POINT FIT_BUTTON_CELL_POS       =POINT({ 2, 0});
constexpr POINT FOREGROUND_BUTTON_CELL_POS=POINT({ 1, 0});
constexpr POINT HALFTONE_BUTTON_CELL_POS  =POINT({ 3, 0});
constexpr POINT HELP_BUTTON_CELL_POS      =POINT({-4, 0});
constexpr POINT HOLE_SLIDER_CELL_POS      =POINT({ 0,-1});
constexpr POINT LOCK_BUTTON_CELL_POS      =POINT({ 0, 0});
constexpr POINT MAXIMIZE_BUTTON_CELL_POS  =POINT({-2, 0});
constexpr POINT MINIMIZE_BUTTON_CELL_POS  =POINT({-3, 0});
constexpr POINT RESET_BUTTON_CELL_POS     =POINT({ 4, 0});
constexpr POINT SCALE_SLIDER_CELL_POS     =POINT({ 0,-3});

constexpr wchar_t ALPHA_SLIDER_HINT[]     =L"透過率";
constexpr wchar_t CLOSE_BUTTON_HINT[]     =L"閉じる";
constexpr wchar_t FIT_BUTTON_HINT[]       =L"余白をカット";
constexpr wchar_t FOREGROUND_BUTTON_HINT[]=L"ターゲットを前面に表示";
constexpr wchar_t HALFTONE_BUTTON_HINT[]  =L"高画質モード";
constexpr wchar_t HELP_BUTTON_HINT[]      =L"ヘルプ";
constexpr wchar_t HOLE_SLIDER_HINT[]      =L"穴の大きさ";
constexpr wchar_t LOCK_BUTTON_HINT[]      =L"ターゲットをロック";
constexpr wchar_t MAXIMIZE_BUTTON_HINT[]  =L"最大化";
constexpr wchar_t MINIMIZE_BUTTON_HINT[]  =L"最小化";
constexpr wchar_t RESET_BUTTON_HINT[]     =L"設定をリセット";
constexpr wchar_t SCALE_SLIDER_HINT[]     =L"倍率";

constexpr BYTE     DEFAULT_ALPHA              =255;
constexpr COLORREF DEFAULT_BACK_COLOR1        =RGB( 85, 86, 88);
constexpr COLORREF DEFAULT_BACK_COLOR2        =RGB( 55, 59, 62);
constexpr COLORREF DEFAULT_COMPONENT_COLOR1   =RGB(195,229,231);
constexpr COLORREF DEFAULT_COMPONENT_COLOR2   =RGB( 43,144,162);
constexpr bool     DEFAULT_CONTROL_MODE_ALT   =false;
constexpr bool     DEFAULT_CONTROL_MODE_CTRL  =true;
constexpr bool     DEFAULT_CONTROL_MODE_SHIFT =false;
constexpr UINT     DEFAULT_FPS                =30;
constexpr bool     DEFAULT_HALFTONE           =false;
constexpr LONG     DEFAULT_HOLE               =UNIT_LENGTH*10;
constexpr LONG     DEFAULT_SCALE              =100;
constexpr LONG     DEFAULT_WINDOW_SIZE_DIVISOR=4;

//---- helper declaration ----

class Initializer {public:Initializer(function<void()> initialize);};

class Finalizer
{
public:
    Finalizer(function<void()> finalize);
    virtual ~Finalizer();
protected:function<void()> finalize_;
};

class api_error:public runtime_error
{public:api_error(const string&name);};

struct POINT_DOUBLE {double x,y;};

template<class OBJECT,class RESULT,class...ARGUMENTS>
    function<RESULT(ARGUMENTS...arguments)> bind_object
(RESULT(OBJECT::*member)(ARGUMENTS...arguments),OBJECT*object);
template<class...ARGUMENTS> string describe(ARGUMENTS&&...arguments);
template<class LEAD,class...TRAILER> void describe_to_with
(ostream&os,const string&separator,LEAD&lead,TRAILER&&...trailer);
template<class ARGUMENT> void describe_to_with
(ostream&os,const string&separator,ARGUMENT&argument);
template<class...ARGUMENTS> string describe_with
(const string&separator,ARGUMENTS&&...arguments);

bool contain(const POINT&center,const LONG&squaredRadius,const POINT&pos);
bool contain(const RECT&rect,const POINT&pos);
POINT coordinates(LPARAM lParam);
POINT cursor_pos(HWND window);
void describe_to_with
(ostream&os,const string&separator,const char*argument);
void describe_to_with(ostream&os,const string&separator);
SIZE desktop_size();
double fpn(const string&str);
vector<string> getlines(istream&in);
LONG height(const RECT&rect);
long integer(const string&str);
POINT operator*(const POINT&lhs,const LONG&rhs);
POINT_DOUBLE operator*(const POINT_DOUBLE&lhs,const LONG&rhs);
SIZE operator*(const SIZE&lhs,const LONG&rhs);
POINT operator+(const POINT&lhs,const LONG&rhs);
POINT operator+(const POINT&lhs,const POINT&rhs);
POINT_DOUBLE operator+(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs);
POINT&operator+=(POINT&lhs,const POINT&rhs);
POINT_DOUBLE&operator+=(POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs);
POINT operator/(const POINT&lhs,const LONG&rhs);
POINT_DOUBLE operator/(const POINT_DOUBLE&lhs,const LONG&rhs);
SIZE operator/(const SIZE&lhs,const LONG&rhs);
POINT operator-(const POINT&point);
SIZE operator-(const SIZE&lhs,const LONG&rhs);
SIZE operator-(const SIZE&lhs,const SIZE&rhs);
POINT point(const POINT_DOUBLE&pointDouble);
POINT point(const SIZE&size);
POINT_DOUBLE point_double(const POINT&point);
POINT pos(const RECT&rect);
void putlines(ostream&out,const vector<string>&lines);
RECT rect(const POINT&pos,const SIZE&size);
SIZE size(LPARAM lParam);
SIZE size(const POINT&pos);
SIZE size(const RECT&rect);
POINT sliding_offset(HWND window,const POINT&base);
LONG squared_distance(const POINT&p,const POINT&q);
vector<string> tokenize(const string&str,const string&dels);
LONG width(const RECT&rect);

//---- systemobject declaration ----

class DeleteDC:public Finalizer
{
public:
    DeleteDC(HDC handle);
    HDC handle();
protected:HDC handle_;
};

class DeleteObject:public Finalizer
{
public:
    DeleteObject(HGDIOBJ handle);
    HGDIOBJ handle();
protected:HGDIOBJ handle_;
};

class EndPaint:public Finalizer
{
public:
    EndPaint(HWND window,PAINTSTRUCT*paintStruct,HDC dc);
    HDC handle();
protected:HDC handle_;
};

class ReleaseDC:public Finalizer
{
public:
    ReleaseDC(HWND window,HDC handle);
    HDC handle();
protected:HDC handle_;
};

class TimeEndPeriod:public Finalizer
{public:TimeEndPeriod(UINT resolution);};

class TimeKillEvent:public Finalizer {public:TimeKillEvent(UINT timer);};

class Timer
{
public:Timer(UINT delay,HWND dest);
protected:
    static void CALLBACK timeProcedure
    (
        UINT timerID,
        UINT message,
        DWORD_PTR user,
        DWORD_PTR reserved1,
        DWORD_PTR reserved2
    );
    UINT delay_;
    HWND dest_;
    shared_ptr<TimeKillEvent> kill_;
    static UINT resolution_;
};

class Buffer
{
public:
    HBITMAP bitmap();
    static shared_ptr<Buffer> create(const SIZE&size,HDC destDC);
    HDC dc();
    static shared_ptr<Buffer> load
    (HINSTANCE instance,LPCTSTR name,HDC destDC);
    const SIZE&size();
protected:
    Buffer
    (
        const shared_ptr<DeleteObject>&bitmap,
        const shared_ptr<DeleteDC>&dc,
        const SIZE&size
    );
    shared_ptr<DeleteObject> bitmap_;
    shared_ptr<DeleteDC> dc_;
    SIZE size_;
};

shared_ptr<TimeEndPeriod> timeBeginPeriod(UINT resolution);
void timeGetDevCaps(LPTIMECAPS caps,UINT sizeOfCaps);
shared_ptr<TimeKillEvent> timeSetEvent
(
    UINT delay,
    UINT resolution,
    LPTIMECALLBACK time_procedure,
    DWORD user,
    UINT event
);

//---- component declaration ----

class Component
{
public:
    virtual void activate(const POINT&cursorPos)=0;
    bool active();
    virtual void deactivate(const POINT&cursorPos)=0;
    virtual bool hitTest(const POINT&cursorPos)=0;
    virtual void paint(HDC dc)=0;
    virtual void relocate(const SIZE&containerSize);
    virtual void update(const POINT&cursorPos)=0;
protected:
    Component
    (
        const POINT&cellPos,
        HWND container,
        HWND toolTip,
        UINT toolId,
        const wstring&toolHint
    );
    virtual void relocateTool()=0;
    virtual void render()=0;
    void reposition(const SIZE&containerSize);
    bool active_;
    POINT cellPos_;
    POINT pos_;
    HWND toolTip_;
    wstring toolHint_;
    TOOLINFOW toolInfo_;
};

class Button:public Component
{
public:
    virtual void activate(const POINT&cursorPos) override;
    virtual bool hitTest(const POINT&cursorPos) override;
    virtual void paint(HDC dc) override;
    virtual void update(const POINT&cursorPos) override;
protected:
    Button
    (
        LPCTSTR maskBitmapName,
        HDC destDC,
        const POINT&cellPos,
        HWND container,
        HWND toolTip,
        UINT toolId,
        const wstring&toolHint
    );
    virtual void relocateTool() override;
    void render_(const bool&push);
    shared_ptr<Buffer> backBuffer_;
    shared_ptr<Buffer> foreBuffer_;
    shared_ptr<Buffer> iconMaskBuffer_;
    shared_ptr<Buffer> maskBuffer_;
};

class PushButton:public Button
{
public:
    PushButton
    (
        LPCTSTR maskBitmapName,
        HDC destDC,
        const POINT&cellPos,
        HWND container,
        HWND toolTip,
        UINT toolId,
        const wstring&toolHint
    );
    virtual void deactivate(const POINT&cursorPos) override;
    function<void()> click;
protected:virtual void render() override;
};

class RadioButton:public Button
{
public:
    RadioButton
    (
        const bool&initialValue,
        LPCTSTR maskBitmapName,
        HDC destDC,
        const POINT&cellPos,
        HWND container,
        HWND toolTip,
        UINT toolId,
        const wstring&toolHint
    );
    virtual void deactivate(const POINT&cursorPos) override;
    bool value();
    void value(const bool&value_);
    function<void()> change;
protected:
    virtual void render() override;
    bool value_;
};

class Slider:public Component
{
public:
    Slider
    (
        const int&minimum,
        const int&maximum,
        const int&initialValue,
        function<string(const int&value)> getText,
        LPCTSTR knobMaskBitmapName,
        HDC destDC,
        const POINT&cellPos,
        HWND container,
        HWND toolTip,
        UINT toolId,
        const wstring&toolHint
    );
    virtual void activate(const POINT&cursorPos) override;
    virtual void deactivate(const POINT&cursorPos) override;
    virtual bool hitTest(const POINT&cursorPos) override;
    virtual void paint(HDC dc) override;
    virtual void relocate(const SIZE&containerSize) override;
    virtual void update(const POINT&cursorPos) override;
    int value();
    void value(const int&value_);
    function<void()> change;
protected:
    bool hitTestBar(const POINT&cursorPos);
    bool hitTestKnob(const POINT&cursorPos);
    virtual void relocateTool() override;
    virtual void render() override;
    void renderBar();
    void renderKnob();
    void renderText();
    void updateKnobX();
    shared_ptr<Buffer> barBuffer_;
    function<string(const int&value)> getText_;
    LONG length_;
    shared_ptr<Buffer> knobBackBuffer_;
    shared_ptr<Buffer> knobForeBuffer_;
    shared_ptr<Buffer> knobIconMaskBuffer_;
    shared_ptr<Buffer> knobMaskBuffer_;
    LONG knobX_;
    int maximum_;
    int minimum_;
    LONG pinch_;
    string text_;
    shared_ptr<Buffer> textBuffer_;
    shared_ptr<Buffer> textMaskBuffer_;
    int value_;
};

//---- window declaration ----

class Window
{
public:
    HWND handle();
    void move(const POINT&pos,const SIZE&size);
protected:
    Window();
    LRESULT onReceive
    (HWND handle,UINT message,WPARAM wParam,LPARAM lParam);
    static LRESULT CALLBACK windowProcedure
    (HWND handle,UINT message,WPARAM wParam,LPARAM lParam);
    HWND handle_;
    map<UINT,function<LRESULT(UINT message,WPARAM wParam,LPARAM lParam)>>
        handlerMap_;
    static map<HWND,Window*> windowMap_;
};

class MainWindow:public Window
{
public:
    MainWindow();
    void onAlphaSliderChange();
    void onCloseButtonClick();
    LRESULT onCreate
    (UINT message,WPARAM wParam,LPARAM lParam);
    LRESULT onDestroy
    (UINT message,WPARAM wParam,LPARAM lParam);
    void onFitButtonClick();
    void onForegroundButtonClick();
    LRESULT onGetMinMaxInfo
    (UINT message,WPARAM wParam,LPARAM lParam);
    void onHalftoneButtonChange();
    void onHelpButtonClick();
    void onHoleSliderChange();
    LRESULT onLButtonDown
    (UINT message,WPARAM wParam,LPARAM lParam);
    LRESULT onLButtonUp
    (UINT message,WPARAM wParam,LPARAM lParam);
    void onMaximizeButtonChange();
    void onMinimizeButtonClick();
    LRESULT onMouseMove
    (UINT message,WPARAM wParam,LPARAM lParam);
    LRESULT onMove
    (UINT message,WPARAM wParam,LPARAM lParam);
    LRESULT onNCHitTest
    (UINT message,WPARAM wParam,LPARAM lParam);
    LRESULT onNCMouseMove
    (UINT message,WPARAM wParam,LPARAM lParam);
    LRESULT onNCRButtonDown
    (UINT message,WPARAM wParam,LPARAM lParam);
    LRESULT onPaint
    (UINT message,WPARAM wParam,LPARAM lParam);
    LRESULT onRButtonUp
    (UINT message,WPARAM wParam,LPARAM lParam);
    void onResetButtonClick();
    void onScaleSliderChange();
    LRESULT onSize
    (UINT message,WPARAM wParam,LPARAM lParam);
    LRESULT onUserTimer
    (UINT message,WPARAM wParam,LPARAM lParam);
protected:
    void initializeBackBrush();
    void initializeBuffers();
    void initializeComponents();
    Component*activeComponent_;
    shared_ptr<Slider> alphaSlider_;
    shared_ptr<DeleteObject> backBrush_;
    shared_ptr<Buffer> buffer_;
    shared_ptr<PushButton> closeButton_;
    vector<Component*> components_;
    shared_ptr<PushButton> fitButton_;
    shared_ptr<PushButton> foregroundButton_;
    shared_ptr<RadioButton> halftoneButton_;
    shared_ptr<PushButton> helpButton_;
    shared_ptr<Slider> holeSlider_;
    shared_ptr<RadioButton> lockButton_;
    shared_ptr<RadioButton> maximizeButton_;
    shared_ptr<PushButton> minimizeButton_;
    shared_ptr<PushButton> resetButton_;
    shared_ptr<Slider> scaleSlider_;
    shared_ptr<Timer> timer_;
    HWND toolTip_;
    shared_ptr<Buffer> viewBuffer_;
    bool viewSliding_;
    POINT viewSlidingBase_;
};

//---- execute declaration ----

struct properties
{
    template<class INPUT> void load(INPUT begin,INPUT end);

    properties();
    vector<string> lines() const;
    BYTE alpha;
    COLORREF back_color1;
    COLORREF back_color2;
    COLORREF component_color1;
    COLORREF component_color2;
    bool control_mode_alt;
    bool control_mode_ctrl;
    bool control_mode_shift;
    UINT fps;
    bool halftone;
    LONG hole;
    LONG scale;
    POINT_DOUBLE view_base;
    POINT window_pos;
    SIZE window_size;
protected:
    static pair<string,string> parse(const string&expression);
    void set(const string&name,const string&value);
};

struct context
{
    context();
    shared_ptr<DeleteObject> almost_black_brush;
    shared_ptr<DeleteObject> back_brush1;
    shared_ptr<DeleteObject> back_brush2;
    shared_ptr<DeleteObject> black_brush;
    shared_ptr<DeleteObject> black_pen;
    shared_ptr<DeleteObject> component_brush1;
    shared_ptr<DeleteObject> component_brush2;
    shared_ptr<DeleteObject> component_pen1;
    shared_ptr<DeleteObject> component_pen2;
    shared_ptr<runtime_error> error;
    shared_ptr<DeleteObject> font;
    HINSTANCE instance;
    properties ps;
    HWND target;
    shared_ptr<DeleteObject> white_brush;
};

bool control_mode();
context&ct();
int execute
(
    HINSTANCE instance,
    HINSTANCE previousInstance,
    LPSTR commandLine,
    int commandShow
);

//---- apiwrapper declaration ----

shared_ptr<EndPaint> BeginPaint(HWND window,PAINTSTRUCT*paintStruct);
void BitBlt
(
    HDC destDC,
    int destX,
    int destY,
    int width,
    int height,
    HDC srcDC,
    int srcX,
    int srcY,
    DWORD rop
);
shared_ptr<DeleteObject> CreateCompatibleBitmap
(HDC destDC,int width,int height);
shared_ptr<DeleteDC> CreateCompatibleDC(HDC dc);
shared_ptr<DeleteObject> CreateFont
(
    int height,
    int width,
    int escapement,
    int orientation,
    int weight,
    DWORD italic,
    DWORD underline,
    DWORD strikeOut,
    DWORD charSet,
    DWORD outputPrecision,
    DWORD clipPrecision,
    DWORD quality,
    DWORD pitchAndFamily,
    LPCTSTR face
);
shared_ptr<DeleteObject> CreatePatternBrush(HBITMAP bitmap);
shared_ptr<DeleteObject> CreatePen(int style,int width,COLORREF color);
shared_ptr<DeleteObject> CreateSolidBrush(COLORREF color);
int DrawText(HDC dc,LPCTSTR str,int count,LPRECT rect,UINT format);
void Ellipse(HDC dc,int left,int top,int right,int bottom);
void FillRect(HDC dc,CONST RECT*rect,HBRUSH brush);
shared_ptr<ReleaseDC> GetDC(HWND window);
void GetClientRect(HWND window,LPRECT rect);
void GetCursorPos(LPPOINT point);
bool GetMessage(LPMSG msg,HWND window,UINT first,UINT last);
int GetObject(HGDIOBJ object,int sizeOfBuffer,LPVOID buffer);
int GetSystemMetrics(int index);
void GetWindowPlacement(HWND window,WINDOWPLACEMENT*placement);
void InvalidateRect(HWND window,CONST RECT*rect,BOOL erase);
shared_ptr<DeleteObject> LoadBitmap(HINSTANCE instance,LPCTSTR name);
HCURSOR LoadCursor(HINSTANCE instance,LPCTSTR name);
ATOM RegisterClassEx(CONST WNDCLASSEX*windowClass);
void ScreenToClient(HWND window,LPPOINT point);
int SetBkMode(HDC dc,int mode);
void SetBrushOrgEx(HDC dc,int x,int y,LPPOINT oldPoint);
void SetForegroundWindow(HWND window);
void SetLayeredWindowAttributes
(HWND window,COLORREF key,BYTE alpha,DWORD flags);
int SetStretchBltMode(HDC dc,int mode);
COLORREF SetTextColor(HDC dc,COLORREF color);
void MoveWindow
(
    HWND window,
    int x,
    int y,
    int width,
    int height,
    BOOL repaint
);
void StretchBlt
(
    HDC destDC,
    int destX,
    int destY,
    int destWidth,
    int destHeight,
    HDC srcDC,
    int srcX,
    int srcY,
    int srcWidth,
    int srcHeight,
    DWORD rop
);
void UpdateWindow(HWND window);

//---- helper template definition ----

template<class OBJECT,class RESULT,class...ARGUMENTS>
    function<RESULT(ARGUMENTS...arguments)> bind_object
(RESULT(OBJECT::*member)(ARGUMENTS...arguments),OBJECT*object)
{
    return [object,member] (ARGUMENTS...arguments)->RESULT
    {return (object->*member)(arguments...);};
}

template<class...ARGUMENTS> string describe(ARGUMENTS&&...arguments)
{return describe_with("",arguments...);}

template<class LEAD,class... TRAILER> void describe_to_with
(ostream&os,const string&separator,LEAD&lead,TRAILER&&...trailer)
{
    describe_to_with(os,separator,lead);
    if(!separator.empty()) os<<separator;
    describe_to_with(os,separator,trailer...);
}

template<class ARGUMENT> void describe_to_with
(ostream&os,const string&separator,ARGUMENT&argument) {os<<argument;}

template<class...ARGUMENTS> string describe_with
(const string&separator,ARGUMENTS&&...arguments)
{
    ostringstream oss;
    describe_to_with(oss,separator,arguments...);
    return oss.str();
}

//---- execute template definition ----

template<class INPUT> void properties::load(INPUT begin,INPUT end)
{
    for(INPUT it=begin;it!=end;it++)
    {
        const auto nameAndValue=parse(*it);
        set(nameAndValue.first,nameAndValue.second);
    }
}

}

#endif
