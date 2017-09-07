#ifndef NAGARAMI_H
#define NAGARAMI_H

#include<functional>
#include<ios>
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

namespace nm
{

using namespace std;

//---- constexpr definition ----

constexpr char APPLICATION_NAME[]         ="Nagarami";
constexpr char PROPERTIES_FILE_EXTENSION[]="ps";
constexpr char HELP_URL[]                 =
    "https://github.com/Gonbee2017/Nagarami/blob/master/README.md";

constexpr COLORREF ALMOST_BLACK_COLOR=RGB(  1,  1,  1);
constexpr COLORREF BLACK_COLOR       =RGB(  0,  0,  0);
constexpr COLORREF WHITE_COLOR       =RGB(255,255,255);

constexpr LONG  UNIT_LENGTH          =16;
constexpr LONG  HALF_UNIT_LENGTH     =UNIT_LENGTH/2;
constexpr SIZE  UNIT_SIZE            =SIZE({UNIT_LENGTH,UNIT_LENGTH});
constexpr RECT  UNIT_RECT            =
    RECT({0,0,UNIT_SIZE.cx,UNIT_SIZE.cy});
constexpr LONG  FRAME_LENGTH         =UNIT_LENGTH;
constexpr LONG  MINIMUM_WINDOW_WIDTH =UNIT_LENGTH*8+FRAME_LENGTH*2;
constexpr LONG  MINIMUM_WINDOW_HEIGHT=UNIT_LENGTH*4+FRAME_LENGTH*2;
constexpr SIZE  MINIMUM_WINDOW_SIZE  =
    SIZE({MINIMUM_WINDOW_WIDTH,MINIMUM_WINDOW_HEIGHT});
constexpr LONG  SLIDER_BAR_WIDTH     =UNIT_LENGTH/4;
constexpr LONG  HALF_SLIDER_BAR_WIDTH=SLIDER_BAR_WIDTH/2;
constexpr LONG  SLIDER_TEXT_WIDTH    =UNIT_LENGTH*2;
constexpr SIZE  SLIDER_TEXT_SIZE     =
    SIZE({SLIDER_TEXT_WIDTH,UNIT_LENGTH});
constexpr POINT ZERO_POS             =POINT({0,0});
constexpr RECT  ZERO_RECT            =RECT({0,0,0,0});
constexpr SIZE  ZERO_SIZE            =SIZE({0,0});

constexpr POINT ALPHA_SLIDER_CELL_POS     =POINT({ 0,-1});
constexpr POINT CLOSE_BUTTON_CELL_POS     =POINT({-1, 0});
constexpr POINT FOREGROUND_BUTTON_CELL_POS=POINT({ 1, 0});
constexpr POINT HALFTONE_BUTTON_CELL_POS  =POINT({ 2, 0});
constexpr POINT HELP_BUTTON_CELL_POS      =POINT({-4, 0});
constexpr POINT HOLE_SLIDER_CELL_POS      =POINT({ 0,-2});
constexpr POINT LOCK_BUTTON_CELL_POS      =POINT({ 0, 0});
constexpr POINT MAXIMIZE_BUTTON_CELL_POS  =POINT({-2, 0});
constexpr POINT MINIMIZE_BUTTON_CELL_POS  =POINT({-3, 0});
constexpr POINT RESET_BUTTON_CELL_POS     =POINT({ 3, 0});
constexpr POINT SCALE_SLIDER_CELL_POS     =POINT({ 0,-3});

constexpr int  ALPHA_DIVISOR=20;
constexpr LONG MAXIMUM_HOLE =UNIT_LENGTH*20;
constexpr LONG MAXIMUM_SCALE=500;
constexpr int  SCALE_DIVISOR=10;

constexpr BYTE     DEFAULT_ALPHA              =255;
constexpr COLORREF DEFAULT_BACK_COLOR1        =RGB( 85, 86, 88);
constexpr COLORREF DEFAULT_BACK_COLOR2        =RGB( 55, 59, 62);
constexpr COLORREF DEFAULT_COMPONENT_COLOR1   =RGB(195,229,231);
constexpr COLORREF DEFAULT_COMPONENT_COLOR2   =RGB( 19,122,127);
constexpr bool     DEFAULT_CONTROL_MODE_ALT   =false;
constexpr bool     DEFAULT_CONTROL_MODE_CTRL  =true;
constexpr bool     DEFAULT_CONTROL_MODE_SHIFT =false;
constexpr UINT     DEFAULT_FPS                =30;
constexpr bool     DEFAULT_HALFTONE           =false;
constexpr LONG     DEFAULT_HOLE               =UNIT_LENGTH*0;
constexpr LONG     DEFAULT_SCALE              =100;
constexpr LONG     DEFAULT_WINDOW_SIZE_DIVISOR=4;

constexpr wchar_t ALPHA_SLIDER_TOOL_TEXT[]     =L"透過率";
constexpr wchar_t CLOSE_BUTTON_TOOL_TEXT[]     =L"閉じる";
constexpr wchar_t FOREGROUND_BUTTON_TOOL_TEXT[]=L"ターゲットを前面に表示";
constexpr wchar_t HALFTONE_BUTTON_TOOL_TEXT[]  =L"高画質モード";
constexpr wchar_t HELP_BUTTON_TOOL_TEXT[]      =L"ヘルプ";
constexpr wchar_t HOLE_SLIDER_TOOL_TEXT[]      =L"穴のサイズ";
constexpr wchar_t LOCK_BUTTON_TOOL_TEXT[]      =L"ターゲットをロック";
constexpr wchar_t MAXIMIZE_BUTTON_TOOL_TEXT[]  =L"最大化";
constexpr wchar_t MINIMIZE_BUTTON_TOOL_TEXT[]  =L"最小化";
constexpr wchar_t RESET_BUTTON_TOOL_TEXT[]     =L"設定をリセット";
constexpr wchar_t SCALE_SLIDER_TOOL_TEXT[]     =L"倍率";

//---- forward declaration ----

class Buffer;
class Button;
class Component;
class DeleteDC;
class DeleteObject;
class EndPaint;
class Finalizer;
class Initializer;
class MainWindow;
struct POINT_DOUBLE {double x,y;};
class PushButton;
class RadioButton;
class ReleaseDC;
class Slider;
class TimeEndPeriod;
class TimeKillEvent;
class Timer;
class Window;
class api_error;
struct context;
struct port;
struct properties;

//---- apiwrapper declaration ----

shared_ptr<EndPaint> BeginPaint(HWND window,PAINTSTRUCT*paint);
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
HWND CreateWindowEx
(
    DWORD exStyle,
    LPCTSTR className,
    LPCTSTR windowName,
    DWORD style,
    int x,
    int y,
    int width,
    int height,
    HWND parent,
    HMENU menu,
    HINSTANCE instance,
    LPVOID param
);
void DispatchMessage(CONST MSG*message);
int DrawText(HDC dc,LPCTSTR str,int count,LPRECT rect,UINT format);
void Ellipse(HDC dc,int left,int top,int right,int bottom);
void FillRect(HDC dc,CONST RECT*rect,HBRUSH brush);
void GetClientRect(HWND window,LPRECT rect);
void GetCursorPos(LPPOINT point);
shared_ptr<ReleaseDC> GetDC(HWND window);
bool GetMessage(LPMSG msg,HWND window,UINT first,UINT last);
int GetObject(HGDIOBJ object,int sizeOfBuffer,LPVOID buffer);
int GetSystemMetrics(int index);
void GetWindowPlacement(HWND window,WINDOWPLACEMENT*placement);
shared_ptr<DeleteObject> LoadBitmap(HINSTANCE instance,LPCTSTR name);
HCURSOR LoadCursor(HINSTANCE instance,LPCTSTR name);
void RedrawWindow(HWND window,CONST RECT*rect,HRGN region,UINT flags);
ATOM RegisterClassEx(CONST WNDCLASSEX*windowClass);
void ReleaseCapture();
void ScreenToClient(HWND window,LPPOINT point);
int SetBkMode(HDC dc,int mode);
void SetBrushOrgEx(HDC dc,int x,int y,LPPOINT oldPoint);
void SetForegroundWindow(HWND window);
void SetLayeredWindowAttributes
(HWND window,COLORREF key,BYTE alpha,DWORD flags);
int SetStretchBltMode(HDC dc,int mode);
COLORREF SetTextColor(HDC dc,COLORREF color);
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

//---- component declaration ----

class Component
{
public:
    virtual void activate(const POINT&cursorPos)=0;
    void activateTool();
    bool active();
    virtual void deactivate(const POINT&cursorPos)=0;
    void deactivateTool();
    virtual bool hitTest(const POINT&cursorPos)=0;
    virtual bool hitTestTool(const POINT&cursorPos)=0;
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
        const wstring&toolText
    );
    virtual void relocateTool()=0;
    virtual void render()=0;
    void reposition(const SIZE&containerSize);
    bool active_;
    POINT cellPos_;
    POINT pos_;
    bool toolActive_;
    HWND toolTip_;
    wstring toolText_;
    TOOLINFOW toolInfo_;
};

class Button:public Component
{
public:
    virtual void activate(const POINT&cursorPos) override;
    virtual bool hitTest(const POINT&cursorPos) override;
    virtual bool hitTestTool(const POINT&cursorPos) override;
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
        const wstring&toolText
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
        const wstring&toolText
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
        const wstring&toolText
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
        const function<string(const int&value)>&getText,
        LPCTSTR knobMaskBitmapName,
        HDC destDC,
        const POINT&cellPos,
        HWND container,
        HWND toolTip,
        UINT toolId,
        const wstring&toolText
    );
    virtual void activate(const POINT&cursorPos) override;
    virtual void deactivate(const POINT&cursorPos) override;
    virtual bool hitTest(const POINT&cursorPos) override;
    virtual bool hitTestTool(const POINT&cursorPos) override;
    int maximum();
    int minimum();
    virtual void paint(HDC dc) override;
    virtual void relocate(const SIZE&containerSize) override;
    virtual void update(const POINT&cursorPos) override;
    int value();
    void value(const int&value_);
    function<void()> change;
protected:
    bool hitTestBar(const POINT&cursorPos);
    bool hitTestKnob(const POINT&cursorPos);
    bool hitTestText(const POINT&cursorPos);
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

//---- execute declaration ----

struct properties
{
    template<class INPUT> void load(INPUT begin,INPUT end);

    void adjust();
    void initialize();
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
    void initialize(HINSTANCE instance);
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

struct port
{
    void clear();
    void import();
    function<HDC(HWND window,PAINTSTRUCT*paint)> BeginPaint;
    function<BOOL
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
    )> BitBlt;
    function<HBITMAP(HDC destDC,int width,int height)>
        CreateCompatibleBitmap;
    function<HDC(HDC dc)> CreateCompatibleDC;
    function<HFONT
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
    )> CreateFont;
    function<HBRUSH(HBITMAP bitmap)> CreatePatternBrush;
    function<HPEN(int style,int width,COLORREF color)> CreatePen;
    function<HBRUSH(COLORREF color)> CreateSolidBrush;
    function<HWND
    (
        DWORD exStyle,
        LPCTSTR className,
        LPCTSTR windowName,
        DWORD style,
        int x,
        int y,
        int width,
        int height,
        HWND parent,
        HMENU menu,
        HINSTANCE instance,
        LPVOID param
    )> CreateWindowEx;
    function<LRESULT(HWND window,UINT message,WPARAM wParam,LPARAM lParam)>
        DefWindowProc;
    function<BOOL(HDC dc)> DeleteDC;
    function<BOOL(HGDIOBJ object)> DeleteObject;
    function<BOOL(HWND window)> DestroyWindow;
    function<LRESULT(CONST MSG*message)> DispatchMessage;
    function<int(HDC dc,LPCTSTR str,int count,LPRECT rect,UINT format)>
        DrawText;
    function<BOOL(HDC dc,int left,int top,int right,int bottom)> Ellipse;
    function<BOOL(HWND window,CONST PAINTSTRUCT*paint)> EndPaint;
    function<BOOL(HDC dc,CONST RECT*rect,HBRUSH brush)> FillRect;
    function<BOOL(HWND window,LPRECT rect)> GetClientRect;
    function<BOOL(LPPOINT point)> GetCursorPos;
    function<HDC(HWND window)> GetDC;
    function<HWND()> GetForegroundWindow;
    function<SHORT(int key)> GetKeyState;
    function<DWORD()> GetLastError;
    function<int(LPMSG msg,HWND window,UINT first,UINT last)> GetMessage;
    function<int(HGDIOBJ object,int sizeOfBuffer,LPVOID buffer)> GetObject;
    function<int(int index)> GetSystemMetrics;
    function<BOOL(HWND window,WINDOWPLACEMENT*placement)>
        GetWindowPlacement;
    function<BOOL(HWND window)> IsIconic;
    function<BOOL(HWND window)> IsWindow;
    function<BOOL(HWND window)> IsWindowVisible;
    function<BOOL(HWND window)> IsZoomed;
    function<HBITMAP(HINSTANCE instance,LPCTSTR name)> LoadBitmap;
    function<HCURSOR(HINSTANCE instance,LPCTSTR name)> LoadCursor;
    function<int(HWND owner,LPCTSTR text,LPCTSTR caption,UINT type)>
        MessageBox;
    function<VOID(int exitCode)> PostQuitMessage;
    function<BOOL(HWND window,CONST RECT*rect,HRGN region,UINT flags)>
        RedrawWindow;
    function<ATOM(CONST WNDCLASSEX*windowClass)> RegisterClassEx;
    function<BOOL()> ReleaseCapture;
    function<int(HWND window,HDC dc)> ReleaseDC;
    function<BOOL(HWND window,LPPOINT point)> ScreenToClient;
    function<HGDIOBJ(HDC dc,HGDIOBJ object)> SelectObject;
    function<LRESULT(HWND window,UINT message,WPARAM wParam,LPARAM lParam)>
        SendMessageW;
    function<int(HDC dc,int mode)> SetBkMode;
    function<BOOL(HDC dc,int x,int y,LPPOINT oldPoint)> SetBrushOrgEx;
    function<HWND(HWND window)> SetCapture;
    function<HCURSOR(HCURSOR cursor)> SetCursor;
    function<BOOL(HWND window)> SetForegroundWindow;
    function<BOOL(HWND window,COLORREF key,BYTE alpha,DWORD flags)>
        SetLayeredWindowAttributes;
    function<int(HDC dc,int mode)> SetStretchBltMode;
    function<COLORREF(HDC dc,COLORREF color)> SetTextColor;
    function<HINSTANCE
    (
        HWND parent,
        LPCTSTR verb,
        LPCTSTR file,
        LPCTSTR parameters,
        LPCTSTR directory,
        INT showCommand
    )> ShellExecute;
    function<BOOL(HWND window,int commandShow)> ShowWindow;
    function<BOOL
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
    )> StretchBlt;
    function<shared_ptr<istream>
    (const string&name,const ios_base::openmode&mode)> input_file;
    function<shared_ptr<ostream>
    (const string&name,const ios_base::openmode&mode)> output_file;
    function<MMRESULT(UINT period)> timeBeginPeriod;
    function<MMRESULT(UINT period)> timeEndPeriod;
    function<MMRESULT(LPTIMECAPS caps,UINT sizeOfCaps)> timeGetDevCaps;
    function<MMRESULT(UINT timerID)> timeKillEvent;
    function<MMRESULT
    (
        UINT delay,
        UINT resolution,
        LPTIMECALLBACK procedure,
        DWORD user,
        UINT event
    )> timeSetEvent;
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
port&pt();

//---- helper declaration ----

class Finalizer
{
public:
    Finalizer(const function<void()>&finalize);
    virtual ~Finalizer();
protected:function<void()> finalize_;
};

class Initializer {public:Initializer(const function<void()>&initialize);};

class api_error:public runtime_error
{public:api_error(const string&name);};

template<class OBJECT,class RESULT,class...ARGUMENTS>
    function<RESULT(ARGUMENTS...arguments)> bind_object
(RESULT(OBJECT::*member)(ARGUMENTS...arguments) const,const OBJECT*object);
template<class OBJECT,class RESULT,class...ARGUMENTS>
    function<RESULT(ARGUMENTS...arguments)> bind_object
(RESULT(OBJECT::*member)(ARGUMENTS...arguments),OBJECT*object);
template<class...ARGUMENTS> string describe(ARGUMENTS&&...arguments);
template<class LEAD,class...TRAILER> void describe_to_with
(ostream&os,const string&separator,LEAD&lead,TRAILER&&...trailer);
template<class ARGUMENT> void describe_to_with
(ostream&os,const string&separator,const ARGUMENT&argument);
template<class ARGUMENT> void describe_to_with
(ostream&os,const string&separator,ARGUMENT&argument);
template<class ARGUMENT> void describe_to_with
(ostream&os,const string&separator,const ARGUMENT*argument);
template<class ARGUMENT> void describe_to_with
(ostream&os,const string&separator,ARGUMENT*argument);
template<class...ARGUMENTS> string describe_with
(const string&separator,ARGUMENTS&&...arguments);

string chomp(const string&str,const char&ch);
bool contain(const POINT&center,const LONG&squaredRadius,const POINT&pos);
bool contain(const RECT&rect,const POINT&pos);
POINT coordinates(LPARAM lParam);
POINT cursor_pos(HWND window);
void describe_to_with(ostream&os,const string&separator);
void describe_to_with
(ostream&os,const string&separator,const char*str);
void describe_to_with
(ostream&os,const string&separator,LPPOINT point);
SIZE desktop_size();
double floating_point_number(const string&str);
vector<string> getlines(istream&is);
LONG height(const RECT&rect);
shared_ptr<istream> input_file
(const string&name,const ios_base::openmode&mode);
long integer(const string&str);
bool operator!=(const POINT&lhs,const POINT&rhs);
bool operator!=(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs);
bool operator!=(const RECT&lhs,const RECT&rhs);
bool operator!=(const SIZE&lhs,const SIZE&rhs);
POINT operator*(const POINT&lhs,const LONG&rhs);
SIZE operator*(const SIZE&lhs,const LONG&rhs);
POINT operator+(const POINT&lhs,const LONG&rhs);
POINT operator+(const POINT&lhs,const POINT&rhs);
POINT_DOUBLE operator+(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs);
POINT&operator+=(POINT&lhs,const POINT&rhs);
POINT_DOUBLE&operator+=(POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs);
POINT operator-(const POINT&point);
POINT operator-(const POINT&lhs,const POINT&rhs);
SIZE operator-(const SIZE&lhs,const SIZE&rhs);
POINT operator/(const POINT&lhs,const LONG&rhs);
SIZE operator/(const SIZE&lhs,const LONG&rhs);
ostream&operator<<(ostream&os,const POINT&point);
ostream&operator<<(ostream&os,const POINT_DOUBLE&point);
ostream&operator<<(ostream&os,const RECT&rect);
ostream&operator<<(ostream&os,const SIZE&size);
bool operator==(const POINT&lhs,const POINT&rhs);
bool operator==(const POINT_DOUBLE&lhs,const POINT_DOUBLE&rhs);
bool operator==(const RECT&lhs,const RECT&rhs);
bool operator==(const SIZE&lhs,const SIZE&rhs);
shared_ptr<ostream> output_file
(const string&name,const ios_base::openmode&mode);
POINT point(const POINT_DOUBLE&pointDouble);
POINT point(const SIZE&size);
POINT_DOUBLE point_double(const POINT&point);
POINT pos(const RECT&rect);
void putlines(ostream&os,const vector<string>&lines);
RECT rect(const POINT&pos,const SIZE&size);
SIZE size(LPARAM lParam);
SIZE size(const POINT&pos);
SIZE size(const RECT&rect);
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

class EndPaint:public Finalizer
{
public:
    EndPaint(HWND window,PAINTSTRUCT*paint,HDC dc);
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

class TimeEndPeriod:public Finalizer {public:TimeEndPeriod(UINT period);};

class TimeKillEvent:public Finalizer {public:TimeKillEvent(UINT timerID);};

class Timer
{
public:Timer(UINT delay,HWND dest);
protected:
    static void CALLBACK procedure
    (
        UINT timerID,
        UINT message,
        DWORD_PTR user,
        DWORD_PTR reserved1,
        DWORD_PTR reserved2
    );
    UINT delay_;
    HWND dest_;
    shared_ptr<TimeKillEvent> killEvent_;
    static UINT period_;
};

shared_ptr<TimeEndPeriod> timeBeginPeriod(UINT period);
void timeGetDevCaps(LPTIMECAPS caps,UINT sizeOfCaps);
shared_ptr<TimeKillEvent> timeSetEvent
(
    UINT delay,
    UINT resolution,
    LPTIMECALLBACK procedure,
    DWORD user,
    UINT event
);

//---- window declaration ----

class Window
{
public:HWND handle();
protected:
    Window();
    LRESULT onReceive
    (HWND handle,UINT message,WPARAM wParam,LPARAM lParam);
    static LRESULT CALLBACK procedure
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
    LRESULT onMouseWheel
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
    void updateToolTip(const POINT&cursorPos);
    Component*activeComponent_;
    shared_ptr<Slider> alphaSlider_;
    shared_ptr<DeleteObject> backBrush_;
    shared_ptr<Buffer> buffer_;
    shared_ptr<PushButton> closeButton_;
    vector<Component*> components_;
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

//---- helper template definition ----

template<class OBJECT,class RESULT,class...ARGUMENTS>
    function<RESULT(ARGUMENTS...arguments)> bind_object
(RESULT(OBJECT::*member)(ARGUMENTS...arguments) const,const OBJECT*object)
{
    return [object,member] (ARGUMENTS&&...arguments)->RESULT
    {return (object->*member)(arguments...);};
}

template<class OBJECT,class RESULT,class...ARGUMENTS>
    function<RESULT(ARGUMENTS...arguments)> bind_object
(RESULT(OBJECT::*member)(ARGUMENTS...arguments),OBJECT*object)
{
    return [object,member] (ARGUMENTS&&...arguments)->RESULT
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
(ostream&os,const string&separator,const ARGUMENT&argument) {os<<argument;}

template<class ARGUMENT> void describe_to_with
(ostream&os,const string&separator,ARGUMENT&argument) {os<<argument;}

template<class ARGUMENT> void describe_to_with
(ostream&os,const string&separator,const ARGUMENT*argument)
{
    ostringstream oss;
    oss<<hex<<showbase<<(unsigned int)argument;
    os<<oss.str();
}

template<class ARGUMENT> void describe_to_with
(ostream&os,const string&separator,ARGUMENT*argument)
{
    ostringstream oss;
    oss<<hex<<showbase<<(unsigned int)argument;
    os<<oss.str();
}

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
