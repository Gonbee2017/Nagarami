#include<algorithm>
#include<cmath>
#include"nagarami.h"
#include"resource.h"
#include<string>
#include<windows.h>

namespace nm
{

HWND Window::handle() {return handle_;}

void Window::registerClass()
{
    WNDCLASSEX clazz;
    clazz.cbClsExtra=0;
    clazz.cbSize=sizeof(WNDCLASSEX);
    clazz.cbWndExtra=0;
    clazz.lpfnWndProc=&procedure;
    clazz.lpszClassName=APPLICATION_NAME;
    clazz.lpszMenuName=NULL;
    clazz.hCursor=NULL;
    clazz.hIcon=NULL;
    clazz.hIconSm=NULL;
    clazz.hInstance=ct->instance;
    clazz.hbrBackground=NULL;
    clazz.style=CS_HREDRAW|CS_VREDRAW;
    nm::RegisterClassEx(&clazz);
}

LRESULT Window::onReceive
(HWND handle,UINT message,WPARAM wParam,LPARAM lParam)
{
    LRESULT result;
    if(message==WM_NCCREATE) handle_=handle;
    try
    {
        const auto handler=handlerMap_.find(message);
        if(handler!=handlerMap_.end())
            result=handler->second(message,wParam,lParam);
        else result=pt->DefWindowProc(handle,message,wParam,lParam);
    } catch(const runtime_error&error)
    {
        ct->error=error;
        if(message==WM_NCCREATE) result=FALSE;
        else if(message==WM_CREATE) result=-1;
        else if(message==WM_DESTROY) result=0;
        else if(message==WM_NCDESTROY) result=0;
        else
        {
            result=pt->DefWindowProc(handle,message,wParam,lParam);
            pt->DestroyWindow(handle);
        }
    }
    return result;
}

LRESULT CALLBACK Window::procedure
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
    } else result=pt->DefWindowProc(handle,message,wParam,lParam);
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
    nm::CreateWindowEx
    (
        WS_EX_LAYERED|WS_EX_TOPMOST,
        APPLICATION_NAME,
        APPLICATION_NAME,
        WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_POPUP,
        ps->windowPos.x,
        ps->windowPos.y,
        ps->windowSize.cx,
        ps->windowSize.cy,
        NULL,
        NULL,
        ct->instance,
        static_cast<Window*>(this)
    );
}

void MainWindow::onAlphaSliderChange()
{ps->alpha=(ALPHA_DIVISOR-alphaSlider_->value())*255/ALPHA_DIVISOR;}

void MainWindow::onCloseButtonClick()
{nm::PostMessage(handle_,WM_CLOSE,0,0);}

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
    nm::GetWindowPlacement(handle_,&placement);
    ps->windowPos=pos(placement.rcNormalPosition);
    ps->windowSize=size(placement.rcNormalPosition);
    if(toolTip_!=NULL) pt->DestroyWindow(toolTip_);
    if(!*ct->error.what()) pt->PostQuitMessage(0);
    return 0;
}

void MainWindow::onForegroundButtonClick()
{if(ct->target!=NULL) nm::SetForegroundWindow(ct->target);}

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
    if(halftoneButton_->value())
    {
        nm::SetStretchBltMode(buffer_->dc(),HALFTONE);
        nm::SetBrushOrgEx(buffer_->dc(),0,0,NULL);
    } else nm::SetStretchBltMode(buffer_->dc(),COLORONCOLOR);
    ps->halftone=halftoneButton_->value();
}

void MainWindow::onHelpButtonClick()
{nm::ShellExecute(handle_,"open",HELP_URL,NULL,NULL,SW_SHOWNORMAL);}

void MainWindow::onHoleSliderChange()
{ps->hole=UNIT_LENGTH*holeSlider_->value();}

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
            pt->SetCapture(handle_);
            nm::RedrawWindow
            (handle_,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
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
        nm::ReleaseCapture();
        nm::RedrawWindow(handle_,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
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
        pt->ShowWindow(handle_,command);
    }
}

void MainWindow::onMinimizeButtonClick()
{pt->ShowWindow(handle_,SW_MINIMIZE);}

LRESULT MainWindow::onMouseMove
(UINT message,WPARAM wParam,LPARAM lParam)
{
    const POINT cursorPos=coordinates(lParam);
    LPCTSTR cursorName=IDC_HAND;
    if(viewSliding_||activeComponent_)
    {
        if(viewSliding_) cursorName=IDC_ARROW;
        else if(activeComponent_) activeComponent_->update(cursorPos);
        nm::RedrawWindow
        (handle_,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
    }
    pt->SetCursor(nm::LoadCursor(NULL,cursorName));
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
    if(!pt->IsIconic(handle_)) ps->windowPos=coordinates(lParam);
    return 0;
}

LRESULT MainWindow::onNCHitTest
(UINT message,WPARAM wParam,LPARAM lParam)
{
    LRESULT result=HTCAPTION;
    if(pt->GetKeyState(VK_LBUTTON)>=0&&pt->GetKeyState(VK_RBUTTON)>=0)
    {
        POINT cursorPos=coordinates(lParam);
        nm::ScreenToClient(handle_,&cursorPos);
        if(!pt->IsZoomed(handle_))
        {
            const RECT captionRect=rect
            (
                POINT({FRAME_LENGTH,FRAME_LENGTH}),
                ps->windowSize-SIZE({FRAME_LENGTH*2,FRAME_LENGTH*2})
            );
            if(cursorPos.x<captionRect.left)
            {
                if(cursorPos.y<captionRect.top)
                    result=HTTOPLEFT;
                else if(cursorPos.y>=captionRect.bottom)
                    result=HTBOTTOMLEFT;
                else result=HTLEFT;
            } else if(cursorPos.x>=captionRect.right)
            {
                if(cursorPos.y<captionRect.top)
                    result=HTTOPRIGHT;
                else if(cursorPos.y>=captionRect.bottom)
                    result=HTBOTTOMRIGHT;
                else result=HTRIGHT;
            } else if(cursorPos.y<captionRect.top)
                result=HTTOP;
            else if(cursorPos.y>=captionRect.bottom)
                result=HTBOTTOM;
        }
        if(result==HTCAPTION&&control_mode())
        {
            for(Component*component:components_)
            {
                if(component->hitTest(cursorPos))
                {
                    result=HTCLIENT;
                    break;
                }
            }
        }
    }
    return result;
}

LRESULT MainWindow::onNCMouseMove
(UINT message,WPARAM wParam,LPARAM lParam)
{
    if(ps->hole>0)
        nm::RedrawWindow(handle_,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
    return 0;
}

LRESULT MainWindow::onNCRButtonDown
(UINT message,WPARAM wParam,LPARAM lParam)
{
    if(ct->target!=NULL)
    {
        viewSliding_=true;
        viewSlidingBase_=cursor_pos(handle_);
        pt->SetCapture(handle_);
    }
    return 0;
}

LRESULT MainWindow::onPaint
(UINT message,WPARAM wParam,LPARAM lParam)
{
    const auto dc=nm::BeginPaint(handle_,&paintStruct_);
    paintBack();
    paintBuffer();
    paintHole();
    paintComponent();
    paintWindow();
    BYTE lwaAlpha=0;
    DWORD lwaFlags=0;
    if(ps->hole>0) lwaFlags|=LWA_COLORKEY;
    if(!control_mode()||alphaSlider_->active())
    {
        lwaAlpha=ps->alpha;
        lwaFlags|=LWA_ALPHA;
    }
    nm::SetLayeredWindowAttributes(handle_,BLACK_COLOR,lwaAlpha,lwaFlags);
    return 0;
}

LRESULT MainWindow::onRButtonUp
(UINT message,WPARAM wParam,LPARAM lParam)
{
    nm::ReleaseCapture();
    viewSliding_=false;
    ps->viewBase+=point_double(cursor_pos(handle_)-viewSlidingBase_);
    return 0;
}

void MainWindow::onResetButtonClick()
{
    ps->viewBase=POINT_DOUBLE({0,0});
    scaleSlider_->value(DEFAULT_SCALE/SCALE_DIVISOR);
    holeSlider_->value(DEFAULT_HOLE/UNIT_LENGTH);
    alphaSlider_->value
    (ALPHA_DIVISOR-(int)round(DEFAULT_ALPHA*ALPHA_DIVISOR/(double)255));
    halftoneButton_->value(DEFAULT_HALFTONE);
    nm::RedrawWindow(handle_,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
}

void MainWindow::onScaleSliderChange()
{
    LONG oldScale=ps->scale;
    LONG newScale=scaleSlider_->value()*SCALE_DIVISOR;
    ps->scale=newScale;
    oldScale=max(oldScale,(LONG)1);
    newScale=max(newScale,(LONG)1);
    const double ratio=(double)newScale/oldScale;
    if(ps->viewBase.x<0) ps->viewBase.x*=ratio;
    if(ps->viewBase.y<0) ps->viewBase.y*=ratio;
}

LRESULT MainWindow::onSize
(UINT message,WPARAM wParam,LPARAM lParam)
{
    if(wParam!=SIZE_MINIMIZED)
    {
        ps->windowSize=size(lParam);
        for(Component*component:components_)
            component->relocate(ps->windowSize);
        const bool maximized=wParam==SIZE_MAXIMIZED;
        if(maximized!=maximizeButton_->value())
            maximizeButton_->value(maximized);
        nm::RedrawWindow(handle_,NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
    }
    return 0;
}

LRESULT MainWindow::onUserTimer
(UINT message,WPARAM wParam,LPARAM lParam)
{
    if(ct->target!=NULL&&pt->IsWindow(ct->target)==FALSE)
        ct->target=NULL;
    if(ct->target==NULL&&lockButton_->value()) lockButton_->value(false);
    if(!lockButton_->value())
    {
        HWND fore=pt->GetForegroundWindow();
        if(fore!=handle_) ct->target=fore;
    }
    if(!pt->IsIconic(handle_))
        nm::RedrawWindow(handle_,NULL,NULL,RDW_INVALIDATE);
    updateToolTip(cursor_pos(handle_));
    return 0;
}

void MainWindow::paintBack()
{
    nm::FillRect
    (
        buffer_->dc(),
        &paintStruct_.rcPaint,
        (HBRUSH)backBrush_->handle()
    );
}

void MainWindow::paintBuffer()
{
    if
    (
        ct->target!=NULL&&
        pt->IsWindow(ct->target)&&
        pt->IsWindowVisible(ct->target)&&
        !pt->IsIconic(ct->target)&&
        ps->scale>0
    )
    {
        RECT targetRect;
        nm::GetClientRect(ct->target,&targetRect);
        const SIZE targetSize=size(targetRect);
        const SIZE viewSize=targetSize*ps->scale/100;
        POINT viewPos=point(ps->viewBase);
        if(viewSliding_)
            viewPos+=
                cursor_pos(handle_)-
                viewSlidingBase_;
        const POINT viewEndPos=viewPos+point(viewSize);
        paintDestPos_=POINT(
        {
            max(paintStruct_.rcPaint.left,viewPos.x),
            max(paintStruct_.rcPaint.top,viewPos.y)
        });
        paintDestSize_=SIZE(
        {
            min(paintStruct_.rcPaint.right,viewEndPos.x),
            min(paintStruct_.rcPaint.bottom,viewEndPos.y)
        })-size(paintDestPos_);
        paintSrcPos_=POINT(
        {
            max(-viewPos.x,(LONG)0),
            max(-viewPos.y,(LONG)0)
        })*100/ps->scale;
        paintSrcSize_=paintDestSize_*100/ps->scale;
        if(paintDestSize_.cx>0&&paintDestSize_.cy>0&&paintSrcSize_.cx>0&&paintSrcSize_.cy>0)
        {
            paintTarget();
            paintOpacity();
        }
    }
}

void MainWindow::paintComponent()
{
    if(control_mode())
    {
        for(Component*component:components_)
            component->paint(buffer_->dc());
    }
}

void MainWindow::paintHole()
{
    if((!control_mode()||holeSlider_->active())&&ps->hole>0)
    {
        POINT center;
        if(!control_mode()) center=cursor_pos(handle_);
        else center=point(size(paintStruct_.rcPaint)/2);
        pt->SelectObject
        (buffer_->dc(),ct->blackPen->handle());
        pt->SelectObject
        (buffer_->dc(),ct->blackBrush->handle());
        const LONG radius=ps->hole/2;
        nm::Ellipse
        (
            buffer_->dc(),
            center.x-radius,
            center.y-radius,
            center.x+radius,
            center.y+radius
        );
    }
}

void MainWindow::paintOpacity()
{
    if(ps->hole>0)
    {
        nm::BitBlt
        (
            buffer_->dc(),
            paintDestPos_.x,
            paintDestPos_.y,
            paintDestSize_.cx,
            paintDestSize_.cy,
            opaqueBuffer_->dc(),
            0,
            0,
            SRCPAINT
        );
    }
}

void MainWindow::paintTarget()
{
    if(ps->scale==100)
    {
        nm::BitBlt
        (
            buffer_->dc(),
            paintDestPos_.x,
            paintDestPos_.y,
            paintDestSize_.cx,
            paintDestSize_.cy,
            nm::GetDC(ct->target)->handle(),
            paintSrcPos_.x,
            paintSrcPos_.y,
            SRCCOPY
        );
    } else
    {
        nm::StretchBlt
        (
            buffer_->dc(),
            paintDestPos_.x,
            paintDestPos_.y,
            paintDestSize_.cx,
            paintDestSize_.cy,
            nm::GetDC(ct->target)->handle(),
            paintSrcPos_.x,
            paintSrcPos_.y,
            paintSrcSize_.cx,
            paintSrcSize_.cy,
            SRCCOPY
        );
    }
}

void MainWindow::paintWindow()
{
    const SIZE paintSize=size(paintStruct_.rcPaint);
    nm::BitBlt
    (
        paintStruct_.hdc,
        paintStruct_.rcPaint.left,
        paintStruct_.rcPaint.top,
        paintSize.cx,
        paintSize.cy,
        buffer_->dc(),
        0,
        0,
        SRCCOPY
    );
}

void MainWindow::initializeBackBrush()
{
    auto dc=nm::GetDC(handle_);
    auto maskBuffer=Buffer::load
    (ct->instance,MAKEINTRESOURCE(IDB_BACK),dc->handle());
    auto foreBuffer=Buffer::create(maskBuffer->size(),dc->handle());
    auto backBuffer=Buffer::create(maskBuffer->size(),dc->handle());
    RECT backRect=rect(POINT({0,0}),maskBuffer->size());
    nm::FillRect
    (foreBuffer->dc(),&backRect,(HBRUSH)ct->backBrush1->handle());
    nm::BitBlt
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
    nm::BitBlt
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
    nm::FillRect
    (backBuffer->dc(),&backRect,(HBRUSH)ct->backBrush2->handle());
    nm::BitBlt
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
    nm::BitBlt
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
    backBrush_=nm::CreatePatternBrush(backBuffer->bitmap());
}

void MainWindow::initializeBuffers()
{
    const auto dc=nm::GetDC(handle_);
    const SIZE desktopSize=desktop_size();
    buffer_=Buffer::create(desktopSize,dc->handle());
    opaqueBuffer_=Buffer::create(desktopSize,dc->handle());
    const RECT desktopRect=rect(POINT({0,0}),desktopSize);
    nm::FillRect
    (opaqueBuffer_->dc(),&desktopRect,(HBRUSH)ct->opaqueBrush->handle());
}

void MainWindow::initializeComponents()
{
    toolTip_=nm::CreateWindowEx
    (
        0,
        TOOLTIPS_CLASS,
        NULL,
        TTS_ALWAYSTIP|TTS_BALLOON,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        handle_,
        NULL,
        ct->instance,
        NULL
    );
    alphaSlider_=make_shared<Slider>
    (
        0,
        ALPHA_DIVISOR,
        ALPHA_DIVISOR-(int)round(ps->alpha*ALPHA_DIVISOR/(double)255),
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
        ps->halftone,
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
        ps->hole/UNIT_LENGTH,
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
        ps->scale/SCALE_DIVISOR,
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

void MainWindow::updateToolTip(const POINT&cursorPos)
{
    Component*toolActiveComponent=nullptr;
    if
    (
        pt->GetKeyState(VK_LBUTTON)>=0&&
        pt->GetKeyState(VK_RBUTTON)>=0&&
        control_mode()&&
        !viewSliding_
    )
    {
        for(Component*component:components_)
        {
            if(component->hitTestTool(cursorPos))
            {
                toolActiveComponent=component;
                break;
            }
        }
    }
    for(Component*component:components_)
    {
        if(component==toolActiveComponent) component->activateTool();
        else component->deactivateTool();
    }
}

}
