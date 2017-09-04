#include<algorithm>
#include<cmath>
#include"nagarami.h"
#include"resource.h"
#include<string>
#include<windows.h>

namespace nagarami
{

HWND Window::handle() {return handle_;}

void Window::move(const POINT&pos,const SIZE&size)
{
    MINMAXINFO minMaxInfo;
    SendMessage(handle_,WM_GETMINMAXINFO,0,(LPARAM)&minMaxInfo);
    const LONG width=max(size.cx,minMaxInfo.ptMinTrackSize.x);
    const LONG height=max(size.cy,minMaxInfo.ptMinTrackSize.y);
    nagarami::MoveWindow
    (
        handle_,
        pos.x,
        pos.y,
        width,
        height,
        TRUE
    );
}

Window::Window()
{
    static const auto registerClass=make_shared<Initializer>([] ()
    {
        WNDCLASSEX clazz;
        clazz.cbClsExtra=0;
        clazz.cbSize=sizeof(WNDCLASSEX);
        clazz.cbWndExtra=0;
        clazz.lpfnWndProc=&windowProcedure;
        clazz.lpszClassName=TEXT(VER_PRODUCTNAME);
        clazz.lpszMenuName=NULL;
        clazz.hCursor=NULL;
        clazz.hIcon=NULL;
        clazz.hIconSm=NULL;
        clazz.hInstance=ct().instance;
        clazz.hbrBackground=NULL;
        clazz.style=CS_HREDRAW|CS_VREDRAW;
        nagarami::RegisterClassEx(&clazz);
    });
}

LRESULT Window::onReceive
(HWND handle,UINT message,WPARAM wParam,LPARAM lParam)
{
    LRESULT result;
    try
    {
        if(message==WM_NCCREATE) handle_=handle;
        if(handlerMap_.find(message)!=handlerMap_.end())
            result=handlerMap_.at(message)(message,wParam,lParam);
        else result=DefWindowProc(handle,message,wParam,lParam);
    } catch(const shared_ptr<runtime_error>&error)
    {
        ct().error=error;
        if(message==WM_NCCREATE) result=FALSE;
        else if(message==WM_CREATE) result=-1;
        else if(message==WM_DESTROY) result=0;
        else if(message==WM_NCDESTROY) result=0;
        {
            result=DefWindowProc(handle,message,wParam,lParam);
            DestroyWindow(handle);
        }
    }
    return result;
}

LRESULT CALLBACK Window::windowProcedure
(HWND handle,UINT message,WPARAM wParam,LPARAM lParam)
{
    LRESULT result;
    if(message==WM_NCCREATE)
    {
        CREATESTRUCT*createStruct=(CREATESTRUCT*)lParam;
        Window*window=
            reinterpret_cast<Window*>(createStruct->lpCreateParams);
        windowMap_.insert(make_pair(handle,window));
    }
    if(windowMap_.find(handle)!=windowMap_.end())
    {
        Window*window=windowMap_.at(handle);
        result=window->onReceive(handle,message,wParam,lParam);
        if(message==WM_NCDESTROY) windowMap_.erase(handle);
    } else result=DefWindowProc(handle,message,wParam,lParam);
    return result;
}

map<HWND,Window*> Window::windowMap_;

MainWindow::MainWindow():
    activeComponent_(nullptr),toolTip_(NULL),viewSliding_(false)
{
    handlerMap_[WM_CREATE]=bind_object(&onCreate,this);
    handlerMap_[WM_DESTROY]=bind_object(&onDestroy,this);
    handlerMap_[WM_GETMINMAXINFO]=bind_object(&onGetMinMaxInfo,this);
    handlerMap_[WM_LBUTTONDOWN]=bind_object(&onLButtonDown,this);
    handlerMap_[WM_LBUTTONUP]=bind_object(&onLButtonUp,this);
    handlerMap_[WM_MOUSEMOVE]=bind_object(&onMouseMove,this);
    handlerMap_[WM_MOUSEWHEEL]=bind_object(&onMouseWheel,this);
    handlerMap_[WM_MOVE]=bind_object(&onMove,this);
    handlerMap_[WM_NCHITTEST]=bind_object(&onNCHitTest,this);
    handlerMap_[WM_NCMOUSEMOVE]=bind_object(&onNCMouseMove,this);
    handlerMap_[WM_NCRBUTTONDOWN]=bind_object(&onNCRButtonDown,this);
    handlerMap_[WM_PAINT]=bind_object(&onPaint,this);
    handlerMap_[WM_RBUTTONUP]=bind_object(&onRButtonUp,this);
    handlerMap_[WM_SIZE]=bind_object(&onSize,this);
    handlerMap_[WM_USERTIMER]=bind_object(&onUserTimer,this);
    HWND handle=::CreateWindowEx
    (
        WS_EX_LAYERED|WS_EX_TOPMOST,
        TEXT(VER_PRODUCTNAME),
        TEXT(VER_PRODUCTNAME),
        WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_POPUP,
        ct().ps.window_pos.x,
        ct().ps.window_pos.y,
        ct().ps.window_size.cx,
        ct().ps.window_size.cy,
        NULL,
        NULL,
        ct().instance,
        static_cast<Window*>(this)
    );
    if(ct().error.get()) throw ct().error;
    if(handle==NULL) throw make_shared<api_error>("CreateWindowEx");
}

void MainWindow::onAlphaSliderChange()
{ct().ps.alpha=(ALPHA_DIVISOR-alphaSlider_->value())*255/ALPHA_DIVISOR;}

void MainWindow::onCloseButtonClick() {PostMessage(handle_,WM_CLOSE,0,0);}

LRESULT MainWindow::onCreate
(UINT message,WPARAM wParam,LPARAM lParam)
{
    initializeBuffers();
    initializeBackBrush();
    initializeComponents();
    return 0;
}

LRESULT MainWindow::onDestroy
(UINT message,WPARAM wParam,LPARAM lParam)
{
    WINDOWPLACEMENT placement;
    nagarami::GetWindowPlacement(handle_,&placement);
    ct().ps.window_pos=pos(placement.rcNormalPosition);
    ct().ps.window_size=size(placement.rcNormalPosition);
    if(toolTip_!=NULL) DestroyWindow(toolTip_);
    if(!ct().error.get()) PostQuitMessage(0);
    return 0;
}

void MainWindow::onForegroundButtonClick()
{if(ct().target!=NULL) nagarami::SetForegroundWindow(ct().target);}

LRESULT MainWindow::onGetMinMaxInfo
(UINT message,WPARAM wParam,LPARAM lParam)
{
    MINMAXINFO*minMaxInfo=(MINMAXINFO*)lParam;
    minMaxInfo->ptMaxTrackSize=point(desktop_size());
    minMaxInfo->ptMinTrackSize=point(MINIMUM_WINDOW_SIZE);
    return 0;
}

void MainWindow::onHalftoneButtonChange()
{
    HDC dc=viewBuffer_->dc();
    if(halftoneButton_->value())
    {
        nagarami::SetStretchBltMode(dc,HALFTONE);
        nagarami::SetBrushOrgEx(dc,0,0,NULL);
    } else nagarami::SetStretchBltMode(dc,COLORONCOLOR);
    ct().ps.halftone=halftoneButton_->value();
}

void MainWindow::onHelpButtonClick()
{
    int code=(int)ShellExecute
    (handle_,TEXT("open"),TEXT(HELP_URL),NULL,NULL,SW_SHOWNORMAL);
    if(code<=32)
        throw make_shared<runtime_error>(describe
        ("ShellExecute failed.(",code,")"));
}

void MainWindow::onHoleSliderChange()
{ct().ps.hole=UNIT_LENGTH*holeSlider_->value();}

LRESULT MainWindow::onLButtonDown
(UINT message,WPARAM wParam,LPARAM lParam)
{
    POINT cursorPos=cursor_pos(handle_);
    for(Component*component:components_)
    {
        if(component->hitTest(cursorPos))
        {
            component->activate(cursorPos);
            activeComponent_=component;
            SetCapture(handle_);
            nagarami::InvalidateRect(handle_,NULL,FALSE);
            nagarami::UpdateWindow(handle_);
            break;
        }
    }
    return 0;
}

LRESULT MainWindow::onLButtonUp
(UINT message,WPARAM wParam,LPARAM lParam)
{
    if(activeComponent_)
    {
        activeComponent_->deactivate(coordinates(lParam));
        activeComponent_=nullptr;
        ReleaseCapture();
        nagarami::InvalidateRect(handle_,NULL,FALSE);
        nagarami::UpdateWindow(handle_);
    }
    return 0;
}

void MainWindow::onMaximizeButtonChange()
{
    if(maximizeButton_->value()!=IsZoomed(handle_))
    {
        int command;
        if(maximizeButton_->value()) command=SW_MAXIMIZE;
        else command=SW_RESTORE;
        ShowWindow(handle_,command);
    }
}

void MainWindow::onMinimizeButtonClick()
{ShowWindow(handle_,SW_MINIMIZE);}

LRESULT MainWindow::onMouseMove
(UINT message,WPARAM wParam,LPARAM lParam)
{
    LPCTSTR cursorName=IDC_HAND;
    if(viewSliding_||activeComponent_)
    {
        if(viewSliding_) cursorName=IDC_ARROW;
        else if(activeComponent_)
            activeComponent_->update(coordinates(lParam));
        nagarami::InvalidateRect(handle_,NULL,FALSE);
        nagarami::UpdateWindow(handle_);
    }
    SetCursor(nagarami::LoadCursor(NULL,cursorName));
    return 0;
}


LRESULT MainWindow::onMouseWheel
(UINT message,WPARAM wParam,LPARAM lParam)
{
    const int offset=GET_WHEEL_DELTA_WPARAM(wParam)/WHEEL_DELTA;
    int value=scaleSlider_->value()+offset;
    value=min(max(value,scaleSlider_->minimum()),scaleSlider_->maximum());
    scaleSlider_->value(value);
    return 0;
}

LRESULT MainWindow::onMove
(UINT message,WPARAM wParam,LPARAM lParam)
{
    if(!IsIconic(handle_)) ct().ps.window_pos=coordinates(lParam);
    return 0;
}

LRESULT MainWindow::onNCHitTest
(UINT message,WPARAM wParam,LPARAM lParam)
{
    LRESULT result=HTCAPTION;
    Component*toolActiveComponent=nullptr;
    if(GetKeyState(VK_LBUTTON)>=0&&GetKeyState(VK_RBUTTON)>=0)
    {
        const RECT captionRect=
            rect(CLIENT_POS,ct().ps.window_size-FRAME_LENGTH*2);
        POINT cursorPos=coordinates(lParam);
        nagarami::ScreenToClient(handle_,&cursorPos);
        const bool zoomed=IsZoomed(handle_);
        if(!zoomed&&cursorPos.x<captionRect.left)
        {
            if(cursorPos.y<captionRect.top) result=HTTOPLEFT;
            else if(cursorPos.y>=captionRect.bottom) result=HTBOTTOMLEFT;
            else result=HTLEFT;
        } else if(!zoomed&&cursorPos.x>=captionRect.right)
        {
            if(cursorPos.y<captionRect.top) result=HTTOPRIGHT;
            else if(cursorPos.y>=captionRect.bottom) result=HTBOTTOMRIGHT;
            else result=HTRIGHT;
        } else if(!zoomed&&cursorPos.y<captionRect.top) result=HTTOP;
        else if(!zoomed&&cursorPos.y>=captionRect.bottom) result=HTBOTTOM;
        else if(control_mode())
        {
            for(Component*component:components_)
            {
                if(component->hitTest(cursorPos))
                {
                    result=HTCLIENT;
                    break;
                }
            }
            for(Component*component:components_)
            {
                if(component->hitTestTool(cursorPos))
                {
                    toolActiveComponent=component;
                    break;
                }
            }
        }
    }
    for(Component*component:components_)
    {
        if(component==toolActiveComponent) component->activateTool();
        else component->deactivateTool();
    }
    return result;
}

LRESULT MainWindow::onNCMouseMove
(UINT message,WPARAM wParam,LPARAM lParam)
{
    if(viewSliding_) SetCursor(LoadCursor(NULL,IDC_ARROW));
    nagarami::InvalidateRect(handle_,NULL,FALSE);
    nagarami::UpdateWindow(handle_);
    return 0;
}

LRESULT MainWindow::onNCRButtonDown
(UINT message,WPARAM wParam,LPARAM lParam)
{
    viewSliding_=true;
    viewSlidingBase_=cursor_pos(handle_);
    SetCapture(handle_);
    return 0;
}

LRESULT MainWindow::onPaint
(UINT message,WPARAM wParam,LPARAM lParam)
{
    PAINTSTRUCT paint;
    const auto dc=nagarami::BeginPaint(handle_,&paint);
    const POINT paintPos=pos(paint.rcPaint);
    const SIZE paintSize=size(paint.rcPaint);
    nagarami::FillRect
    (buffer_->dc(),&paint.rcPaint,(HBRUSH)backBrush_->handle());
    if(ct().target!=NULL&&!IsIconic(ct().target)&&ct().ps.scale>0)
    {
        RECT targetRect;
        nagarami::GetClientRect(ct().target,&targetRect);
        const SIZE targetSize=size(targetRect);
        const SIZE viewSize=targetSize*ct().ps.scale/100;
        POINT viewPos=point(ct().ps.view_base);
        if(viewSliding_)
            viewPos+=sliding_offset(handle_,viewSlidingBase_);
        const POINT viewEndPos=viewPos+point(viewSize);
        const POINT destPos=POINT(
        {
            max(paint.rcPaint.left,viewPos.x),
            max(paint.rcPaint.top,viewPos.y)
        });
        const SIZE destSize=SIZE(
        {
            min(paint.rcPaint.right,viewEndPos.x),
            min(paint.rcPaint.bottom,viewEndPos.y)
        })-size(destPos);
        const RECT destRect=nagarami::rect(destPos,destSize);
        POINT srcPos=-viewPos*100/ct().ps.scale;
        srcPos.x=max(srcPos.x,(LONG)0);
        srcPos.y=max(srcPos.y,(LONG)0);
        const SIZE srcSize=destSize*100/ct().ps.scale;
        if(destSize.cx>0&&destSize.cy>0&&srcSize.cx>0&&srcSize.cy>0)
        {
            auto targetDC=nagarami::GetDC(ct().target);
            nagarami::StretchBlt
            (
                viewBuffer_->dc(),
                0,
                0,
                destSize.cx,
                destSize.cy,
                targetDC->handle(),
                srcPos.x,
                srcPos.y,
                srcSize.cx,
                srcSize.cy,
                SRCCOPY
            );
            nagarami::FillRect
            (
                buffer_->dc(),
                &destRect,
                (HBRUSH)ct().almost_black_brush->handle()
            );
            nagarami::BitBlt
            (
                buffer_->dc(),
                destPos.x,
                destPos.y,
                destSize.cx,
                destSize.cy,
                viewBuffer_->dc(),
                0,
                0,
                SRCPAINT
            );
        }
    }
    if((!control_mode()||holeSlider_->active())&&ct().ps.hole>0)
    {
        POINT center;
        if(!control_mode()) center=cursor_pos(handle_);
        else center=point(paintSize/2);
        SelectObject(buffer_->dc(),ct().black_pen->handle());
        SelectObject(buffer_->dc(),ct().black_brush->handle());
        const LONG radius=ct().ps.hole/2;
        nagarami::Ellipse
        (
            buffer_->dc(),
            center.x-radius,
            center.y-radius,
            center.x+radius,
            center.y+radius
        );
    }
    if(control_mode())
    {
        for(Component*component:components_)
            component->paint(buffer_->dc());
    }
    nagarami::BitBlt
    (
        dc->handle(),
        paintPos.x,
        paintPos.y,
        paintSize.cx,
        paintSize.cy,
        buffer_->dc(),
        0,
        0,
        SRCCOPY
    );
    if(control_mode()&&!alphaSlider_->active())
        nagarami::SetLayeredWindowAttributes
        (handle_,BLACK_COLOR,0,LWA_COLORKEY);
    else
        nagarami::SetLayeredWindowAttributes
        (handle_,BLACK_COLOR,ct().ps.alpha,LWA_ALPHA|LWA_COLORKEY);
    return 0;
}

LRESULT MainWindow::onRButtonUp
(UINT message,WPARAM wParam,LPARAM lParam)
{
    ReleaseCapture();
    viewSliding_=false;
    ct().ps.view_base+=
        point_double(sliding_offset(handle_,viewSlidingBase_));
    return 0;
}

void MainWindow::onResetButtonClick()
{
    ct().ps.view_base=POINT_DOUBLE({0,0});
    halftoneButton_->value(DEFAULT_HALFTONE);
    scaleSlider_->value(DEFAULT_SCALE/SCALE_DIVISOR);
    alphaSlider_->value
    (ALPHA_DIVISOR-(int)round(DEFAULT_ALPHA*ALPHA_DIVISOR/(double)255));
    holeSlider_->value(DEFAULT_HOLE/UNIT_LENGTH);
    nagarami::InvalidateRect(handle_,NULL,FALSE);
    nagarami::UpdateWindow(handle_);
}

void MainWindow::onScaleSliderChange()
{
    LONG oldScale=ct().ps.scale;
    LONG newScale=scaleSlider_->value()*SCALE_DIVISOR;
    ct().ps.scale=newScale;
    oldScale=max(oldScale,(LONG)1);
    newScale=max(newScale,(LONG)1);
    if(ct().ps.view_base.x<0)
        ct().ps.view_base.x=ct().ps.view_base.x*newScale/oldScale;
    if(ct().ps.view_base.y<0)
        ct().ps.view_base.y=ct().ps.view_base.y*newScale/oldScale;
}

LRESULT MainWindow::onSize
(UINT message,WPARAM wParam,LPARAM lParam)
{
    if(wParam!=SIZE_MINIMIZED)
    {
        ct().ps.window_size=size(lParam);
        for(Component*component:components_)
            component->relocate(ct().ps.window_size);
        const bool maximized=wParam==SIZE_MAXIMIZED;
        if(maximized!=maximizeButton_->value())
            maximizeButton_->value(maximized);
        nagarami::InvalidateRect(handle_,NULL,FALSE);
        nagarami::UpdateWindow(handle_);
    }
    return 0;
}

LRESULT MainWindow::onUserTimer
(UINT message,WPARAM wParam,LPARAM lParam)
{
    if(ct().target!=NULL&&IsWindow(ct().target)==FALSE) ct().target=NULL;
    if(ct().target==NULL&&lockButton_->value()) lockButton_->value(false);
    if(!lockButton_->value())
    {
        HWND fore=GetForegroundWindow();
        if(fore!=handle_) ct().target=fore;
    }
    if(!IsIconic(handle_)) nagarami::InvalidateRect(handle_,NULL,FALSE);
    return 0;
}

void MainWindow::initializeBackBrush()
{
    auto dc=nagarami::GetDC(handle_);
    auto maskBuffer=Buffer::load
    (ct().instance,MAKEINTRESOURCE(IDB_BACK),dc->handle());
    auto foreBuffer=Buffer::create(maskBuffer->size(),dc->handle());
    auto backBuffer=Buffer::create(maskBuffer->size(),dc->handle());
    RECT backRect=rect(POINT({0,0}),maskBuffer->size());
    nagarami::FillRect
    (foreBuffer->dc(),&backRect,(HBRUSH)ct().back_brush1->handle());
    nagarami::BitBlt
    (
        backBuffer->dc(),
        0,
        0,
        maskBuffer->size().cx,
        maskBuffer->size().cy,
        maskBuffer->dc(),
        0,
        0,
        NOTSRCCOPY
    );
    nagarami::BitBlt
    (
        foreBuffer->dc(),
        0,
        0,
        maskBuffer->size().cx,
        maskBuffer->size().cy,
        backBuffer->dc(),
        0,
        0,
        SRCAND
    );
    nagarami::FillRect
    (backBuffer->dc(),&backRect,(HBRUSH)ct().back_brush2->handle());
    nagarami::BitBlt
    (
        backBuffer->dc(),
        0,
        0,
        maskBuffer->size().cx,
        maskBuffer->size().cy,
        maskBuffer->dc(),
        0,
        0,
        SRCAND
    );
    nagarami::BitBlt
    (
        backBuffer->dc(),
        0,
        0,
        maskBuffer->size().cx,
        maskBuffer->size().cy,
        foreBuffer->dc(),
        0,
        0,
        SRCPAINT
    );
    backBrush_=nagarami::CreatePatternBrush(backBuffer->bitmap());
}

void MainWindow::initializeBuffers()
{
    auto dc=nagarami::GetDC(handle_);
    SIZE desktopSize=desktop_size();
    buffer_=Buffer::create(desktopSize,dc->handle());
    viewBuffer_=Buffer::create(desktopSize,dc->handle());
}

void MainWindow::initializeComponents()
{
    toolTip_=::CreateWindowExW
    (
        0,
        TOOLTIPS_CLASSW,
        NULL,
        TTS_ALWAYSTIP|TTS_BALLOON,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        handle_,
        NULL,
        ct().instance,
        NULL
    );
    if(toolTip_==NULL) throw make_shared<api_error>("CreateWindowExW");
    nagarami::SetWindowPos
    (toolTip_,HWND_TOPMOST,0,0,0,0,SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
    alphaSlider_=make_shared<Slider>
    (
        0,
        ALPHA_DIVISOR,
        ALPHA_DIVISOR-(int)round(ct().ps.alpha*ALPHA_DIVISOR/(double)255),
        [] (const int&value)->string
        {return describe(value*100/ALPHA_DIVISOR,"%");},
        MAKEINTRESOURCE(IDB_ALPHA),
        buffer_->dc(),
        ALPHA_SLIDER_CELL_POS,
        handle_,
        toolTip_,
        components_.size(),
        ALPHA_SLIDER_TOOL_TEXT
    );
    alphaSlider_->change=bind_object(&onAlphaSliderChange,this);
    components_.push_back(alphaSlider_.get());
    closeButton_=make_shared<PushButton>
    (
        MAKEINTRESOURCE(IDB_CLOSE),
        buffer_->dc(),
        CLOSE_BUTTON_CELL_POS,
        handle_,
        toolTip_,
        components_.size(),
        CLOSE_BUTTON_TOOL_TEXT
    );
    closeButton_->click=bind_object(&onCloseButtonClick,this);
    components_.push_back(closeButton_.get());
    foregroundButton_=make_shared<PushButton>
    (
        MAKEINTRESOURCE(IDB_FOREGROUND),
        buffer_->dc(),
        FOREGROUND_BUTTON_CELL_POS,
        handle_,
        toolTip_,
        components_.size(),
        FOREGROUND_BUTTON_TOOL_TEXT
    );
    foregroundButton_->click=bind_object(&onForegroundButtonClick,this);
    components_.push_back(foregroundButton_.get());
    halftoneButton_=make_shared<RadioButton>
    (
        ct().ps.halftone,
        MAKEINTRESOURCE(IDB_HALFTONE),
        buffer_->dc(),
        HALFTONE_BUTTON_CELL_POS,
        handle_,
        toolTip_,
        components_.size(),
        HALFTONE_BUTTON_TOOL_TEXT
    );
    halftoneButton_->change=bind_object(&onHalftoneButtonChange,this);
    onHalftoneButtonChange();
    components_.push_back(halftoneButton_.get());
    helpButton_=make_shared<PushButton>
    (
        MAKEINTRESOURCE(IDB_HELP),
        buffer_->dc(),
        HELP_BUTTON_CELL_POS,
        handle_,
        toolTip_,
        components_.size(),
        HELP_BUTTON_TOOL_TEXT
    );
    helpButton_->click=bind_object(&onHelpButtonClick,this);
    components_.push_back(helpButton_.get());
    holeSlider_=make_shared<Slider>
    (
        0,
        MAXIMUM_HOLE/UNIT_LENGTH,
        ct().ps.hole/UNIT_LENGTH,
        [] (const int&value)->string
        {return describe(UNIT_LENGTH*value,"p");},
        MAKEINTRESOURCE(IDB_HOLE),
        buffer_->dc(),
        HOLE_SLIDER_CELL_POS,
        handle_,
        toolTip_,
        components_.size(),
        HOLE_SLIDER_TOOL_TEXT
    );
    holeSlider_->change=bind_object(&onHoleSliderChange,this);
    components_.push_back(holeSlider_.get());
    lockButton_=make_shared<RadioButton>
    (
        false,
        MAKEINTRESOURCE(IDB_LOCK),
        buffer_->dc(),
        LOCK_BUTTON_CELL_POS,
        handle_,
        toolTip_,
        components_.size(),
        LOCK_BUTTON_TOOL_TEXT
    );
    components_.push_back(lockButton_.get());
    maximizeButton_=make_shared<RadioButton>
    (
        false,
        MAKEINTRESOURCE(IDB_MAXIMIZE),
        buffer_->dc(),
        MAXIMIZE_BUTTON_CELL_POS,
        handle_,
        toolTip_,
        components_.size(),
        MAXIMIZE_BUTTON_TOOL_TEXT
    );
    maximizeButton_->change=bind_object(&onMaximizeButtonChange,this);
    components_.push_back(maximizeButton_.get());
    minimizeButton_=make_shared<PushButton>
    (
        MAKEINTRESOURCE(IDB_MINIMIZE),
        buffer_->dc(),
        MINIMIZE_BUTTON_CELL_POS,
        handle_,
        toolTip_,
        components_.size(),
        MINIMIZE_BUTTON_TOOL_TEXT
    );
    minimizeButton_->click=bind_object(&onMinimizeButtonClick,this);
    components_.push_back(minimizeButton_.get());
    resetButton_=make_shared<PushButton>
    (
        MAKEINTRESOURCE(IDB_RESET),
        buffer_->dc(),
        RESET_BUTTON_CELL_POS,
        handle_,
        toolTip_,
        components_.size(),
        RESET_BUTTON_TOOL_TEXT
    );
    resetButton_->click=bind_object(&onResetButtonClick,this);
    components_.push_back(resetButton_.get());
    scaleSlider_=make_shared<Slider>
    (
        0,
        MAXIMUM_SCALE/SCALE_DIVISOR,
        ct().ps.scale/SCALE_DIVISOR,
        [] (const int&value)->string
        {return describe(value*SCALE_DIVISOR,"%");},
        MAKEINTRESOURCE(IDB_SCALE),
        buffer_->dc(),
        SCALE_SLIDER_CELL_POS,
        handle_,
        toolTip_,
        components_.size(),
        SCALE_SLIDER_TOOL_TEXT
    );
    scaleSlider_->change=bind_object(&onScaleSliderChange,this);
    components_.push_back(scaleSlider_.get());
}

}
