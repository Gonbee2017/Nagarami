#include<algorithm>
#include<cmath>
#include"nagarami.h"
#include<memory>
#include<windows.h>
#include<commctrl.h>

namespace nm
{

void Component::activateTool()
{
    if(!toolActive_) pt->SendMessageW(toolTip_,TTM_ACTIVATE,TRUE,0);
    toolActive_=true;
}

bool Component::active() const {return active_;}

void Component::deactivateTool()
{
    if(toolActive_) pt->SendMessageW(toolTip_,TTM_ACTIVATE,FALSE,0);
    toolActive_=false;
}

void Component::relocate(const SIZE&containerSize)
{
    reposition(containerSize);
    relocateTool();
}

Component::Component
(
    const POINT&cellPos,
    HWND container,
    HWND toolTip,
    UINT toolId,
    LPCWSTR toolText
):
    active_(false),
    cellPos_(cellPos),
    toolActive_(true),
    toolTip_(toolTip)
{
    fill(&toolInfo_,0);
    toolInfo_.cbSize=TTTOOLINFOW_V2_SIZE;
    toolInfo_.uFlags=TTF_SUBCLASS;
    toolInfo_.hwnd=container;
    toolInfo_.uId=toolId;
    toolInfo_.lpszText=(LPWSTR)toolText;
    if(pt->SendMessageW(toolTip_,TTM_ADDTOOLW,0,(LPARAM)&toolInfo_)==FALSE)
        throw api_error("SendMessageW with TTM_ADDTOOLW",0);
    deactivateTool();
}

void Component::reposition(const SIZE&containerSize)
{
    if(cellPos_.x>=0) pos_.x=FRAME_LENGTH+UNIT_LENGTH*cellPos_.x;
    else pos_.x=containerSize.cx-FRAME_LENGTH+UNIT_LENGTH*cellPos_.x;
    if(cellPos_.y>=0) pos_.y=FRAME_LENGTH+UNIT_LENGTH*cellPos_.y;
    else pos_.y=containerSize.cy-FRAME_LENGTH+UNIT_LENGTH*cellPos_.y;
}

void Button::activate(const POINT&cursorPos)
{
    active_=true;
    render();
}

bool Button::hitTest(const POINT&cursorPos) const
{
    const POINT offset=cursorPos-pos_;
    return
        contain(UNIT_RECT,offset)&&
        nm::GetPixel(maskBuffer_->dc(),offset.x,offset.y)==BLACK_COLOR;
}

bool Button::hitTestTool(const POINT&cursorPos) const
{return hitTest(cursorPos);}

void Button::paint(HDC dc) const
{
    nm::BitBlt
    (
        dc,
        pos_.x,
        pos_.y,
        UNIT_LENGTH,
        UNIT_LENGTH,
        maskBuffer_->dc(),
        0,
        0,
        SRCAND
    );
    nm::BitBlt
    (
        dc,
        pos_.x,
        pos_.y,
        UNIT_LENGTH,
        UNIT_LENGTH,
        backBuffer_->dc(),
        0,
        0,
        SRCPAINT
    );
}

void Button::update(const POINT&cursorPos) {}

Button::Button
(
    LPCTSTR maskBitmapName,
    HDC destDC,
    const POINT&cellPos,
    HWND container,
    HWND toolTip,
    UINT toolId,
    LPCWSTR toolText
):
    Component(cellPos,container,toolTip,toolId,toolText),
    backBuffer_(Buffer::create(UNIT_SIZE,destDC)),
    foreBuffer_(Buffer::create(UNIT_SIZE,destDC)),
    iconMaskBuffer_(Buffer::load(ct->instance,maskBitmapName,destDC)),
    maskBuffer_(Buffer::create(UNIT_SIZE,destDC))
{
    nm::FillRect
    (maskBuffer_->dc(),&UNIT_RECT,(HBRUSH)ct->whiteBrush->handle());
    pt->SelectObject(maskBuffer_->dc(),ct->blackPen->handle());
    pt->SelectObject(maskBuffer_->dc(),ct->blackBrush->handle());
    nm::Ellipse(maskBuffer_->dc(),0,0,UNIT_LENGTH,UNIT_LENGTH);
}

void Button::relocateTool()
{
    toolInfo_.rect=rect(pos_,UNIT_SIZE);
    pt->SendMessageW(toolTip_,TTM_NEWTOOLRECTW,0,(LPARAM)&toolInfo_);
}

void Button::renderButton(const bool&pushed) const
{
    HPEN backPen;
    HBRUSH foreBrush,backBrush;
    if(pushed)
    {
        foreBrush=(HBRUSH)ct->componentBrush2->handle();
        backPen=(HPEN)ct->componentPen2->handle();
        backBrush=(HBRUSH)ct->componentBrush1->handle();
    } else
    {
        foreBrush=(HBRUSH)ct->componentBrush1->handle();
        backPen=(HPEN)ct->componentPen1->handle();
        backBrush=(HBRUSH)ct->componentBrush2->handle();
    }
    nm::FillRect(foreBuffer_->dc(),&UNIT_RECT,foreBrush);
    nm::BitBlt
    (
        backBuffer_->dc(),
        0,
        0,
        UNIT_LENGTH,
        UNIT_LENGTH,
        iconMaskBuffer_->dc(),
        0,
        0,
        NOTSRCCOPY
    );
    nm::BitBlt
    (
        foreBuffer_->dc(),
        0,
        0,
        UNIT_LENGTH,
        UNIT_LENGTH,
        backBuffer_->dc(),
        0,
        0,
        SRCAND
    );
    nm::FillRect
    (backBuffer_->dc(),&UNIT_RECT,(HBRUSH)ct->blackBrush->handle());
    pt->SelectObject(backBuffer_->dc(),backPen);
    pt->SelectObject(backBuffer_->dc(),backBrush);
    nm::Ellipse(backBuffer_->dc(),0,0,UNIT_LENGTH,UNIT_LENGTH);
    nm::BitBlt
    (
        backBuffer_->dc(),
        0,
        0,
        UNIT_LENGTH,
        UNIT_LENGTH,
        iconMaskBuffer_->dc(),
        0,
        0,
        SRCAND
    );
    nm::BitBlt
    (
        backBuffer_->dc(),
        0,
        0,
        UNIT_LENGTH,
        UNIT_LENGTH,
        foreBuffer_->dc(),
        0,
        0,
        SRCPAINT
    );
}

PushButton::PushButton
(
    LPCTSTR maskBitmapName,
    HDC destDC,
    const POINT&cellPos,
    HWND container,
    HWND toolTip,
    UINT toolId,
    LPCWSTR toolText
):Button(maskBitmapName,destDC,cellPos,container,toolTip,toolId,toolText)
{render();}

void PushButton::deactivate(const POINT&cursorPos)
{
    if(control_mode()&&hitTest(cursorPos)&&click) click();
    active_=false;
    render();
}

void PushButton::render() const {renderButton(active_);}

RadioButton::RadioButton
(
    const bool&initialValue,
    LPCTSTR maskBitmapName,
    HDC destDC,
    const POINT&cellPos,
    HWND container,
    HWND toolTip,
    UINT toolId,
    LPCWSTR toolText
):
    Button
    (maskBitmapName,destDC,cellPos,container,toolTip,toolId,toolText),
    value_(initialValue) {render();}

bool RadioButton::value() const {return value_;}

void RadioButton::value(const bool&value_)
{
    this->value_=value_;
    if(change) change();
    render();
}

void RadioButton::deactivate(const POINT&cursorPos)
{
    if(control_mode()&&hitTest(cursorPos))
    {
        value_=!value_;
        if(change) change();
    }
    active_=false;
    render();
}

void RadioButton::render() const {renderButton(active_||value_);}

Slider::Slider
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
    LPCWSTR toolText
):
    Component(cellPos,container,toolTip,toolId,toolText),
    barBuffer_
    (Buffer::create(SIZE({desktop_width(),SLIDER_BAR_WIDTH}),destDC)),
    getText_(getText),
    knobBackBuffer_(Buffer::create(UNIT_SIZE,destDC)),
    knobForeBuffer_(Buffer::create(UNIT_SIZE,destDC)),
    knobIconMaskBuffer_
    (Buffer::load(ct->instance,knobMaskBitmapName,destDC)),
    knobMaskBuffer_(Buffer::create(UNIT_SIZE,destDC)),
    maximum_(maximum),
    minimum_(minimum),
    textBuffer_(Buffer::create(SLIDER_TEXT_SIZE,destDC)),
    textMaskBuffer_(Buffer::create(SLIDER_TEXT_SIZE,destDC)),
    value_(initialValue)
{
    nm::FillRect
    (knobMaskBuffer_->dc(),&UNIT_RECT,(HBRUSH)ct->whiteBrush->handle());
    pt->SelectObject(knobMaskBuffer_->dc(),ct->blackPen->handle());
    pt->SelectObject(knobMaskBuffer_->dc(),ct->blackBrush->handle());
    nm::Ellipse(knobMaskBuffer_->dc(),0,0,UNIT_LENGTH,UNIT_LENGTH);
    pt->SelectObject(textBuffer_->dc(),ct->font->handle());
    nm::SetBkMode(textBuffer_->dc(),TRANSPARENT);
    pt->SelectObject(textMaskBuffer_->dc(),ct->font->handle());
    nm::SetBkMode(textMaskBuffer_->dc(),TRANSPARENT);
    renderKnob();
    renderText();
}

void Slider::activate(const POINT&cursorPos)
{
    if(hitTestKnob(cursorPos)) pinch_=cursorPos.x-pos_.x-knobX_;
    else pinch_=HALF_UNIT_LENGTH;
    update(cursorPos);
    active_=true;
    render();
}

void Slider::deactivate(const POINT&cursorPos)
{
    active_=false;
    render();
}

bool Slider::hitTest(const POINT&cursorPos) const
{return hitTestKnob(cursorPos)||hitTestBar(cursorPos);}

bool Slider::hitTestTool(const POINT&cursorPos) const
{return hitTest(cursorPos)||hitTestText(cursorPos);}

int Slider::maximum() const {return maximum_;}

int Slider::minimum() const {return minimum_;}

void Slider::paint(HDC dc) const
{
    const POINT barPos(
    {
        pos_.x+HALF_UNIT_LENGTH,
        pos_.y+HALF_UNIT_LENGTH-HALF_SLIDER_BAR_WIDTH
    });
    const LONG barLength=length_-SLIDER_TEXT_WIDTH-UNIT_LENGTH;
    nm::BitBlt
    (
        dc,
        barPos.x,
        barPos.y,
        barLength,
        SLIDER_BAR_WIDTH,
        barBuffer_->dc(),
        0,
        0,
        SRCCOPY
    );
    nm::BitBlt
    (
        dc,
        pos_.x+knobX_,
        pos_.y,
        UNIT_LENGTH,
        UNIT_LENGTH,
        knobMaskBuffer_->dc(),
        0,
        0,
        SRCAND
    );
    nm::BitBlt
    (
        dc,
        pos_.x+knobX_,
        pos_.y,
        UNIT_LENGTH,
        UNIT_LENGTH,
        knobBackBuffer_->dc(),
        0,
        0,
        SRCPAINT
    );
    const LONG textX=pos_.x+length_-SLIDER_TEXT_WIDTH;
    nm::BitBlt
    (
        dc,
        textX,
        pos_.y,
        SLIDER_TEXT_WIDTH,
        UNIT_LENGTH,
        textMaskBuffer_->dc(),
        0,
        0,
        SRCAND
    );
    nm::BitBlt
    (
        dc,
        textX,
        pos_.y,
        SLIDER_TEXT_WIDTH,
        UNIT_LENGTH,
        textBuffer_->dc(),
        0,
        0,
        SRCPAINT
    );
}

void Slider::relocate(const SIZE&containerSize)
{
    Component::reposition(containerSize);
    length_=containerSize.cx-pos_.x-FRAME_LENGTH;
    updateKnobX();
    renderBar();
    relocateTool();
}

void Slider::update(const POINT&cursorPos)
{
    if(control_mode())
    {
        const int range=maximum_-minimum_;
        const LONG x=cursorPos.x-pos_.x-pinch_;
        const LONG barLength=length_-SLIDER_TEXT_WIDTH-UNIT_LENGTH;
        value_=minimum_+(int)round(range*x/(double)barLength);
        value_=min(max(value_,minimum_),maximum_);
        if(change) change();
        updateKnobX();
        renderText();
    }
}

int Slider::value() const {return value_;}

void Slider::value(const int&value_)
{
    this->value_=value_;
    if(change) change();
    updateKnobX();
    renderText();
}

bool Slider::hitTestBar(const POINT&cursorPos) const
{
    const POINT barPos(
    {
        pos_.x+HALF_UNIT_LENGTH,
        pos_.y+HALF_UNIT_LENGTH-HALF_SLIDER_BAR_WIDTH
    });
    const SIZE barSize=
        SIZE({length_-SLIDER_TEXT_WIDTH-UNIT_LENGTH,SLIDER_BAR_WIDTH});
    return contain(rect(barPos,barSize),cursorPos);
}

bool Slider::hitTestKnob(const POINT&cursorPos) const
{
    const POINT knobPos({pos_.x+knobX_,pos_.y});
    const POINT offset=cursorPos-knobPos;
    return
        contain(UNIT_RECT,offset)&&
        nm::GetPixel(knobMaskBuffer_->dc(),offset.x,offset.y)==BLACK_COLOR;
}

bool Slider::hitTestText(const POINT&cursorPos) const
{
    const POINT textPos({pos_.x+length_-SLIDER_TEXT_WIDTH,pos_.y});
    const RECT textRect({0,0,SLIDER_TEXT_WIDTH,UNIT_LENGTH});
    const POINT offset=cursorPos-textPos;
    return
        contain(textRect,offset)&&
        nm::GetPixel(textMaskBuffer_->dc(),offset.x,offset.y)==BLACK_COLOR;
}

void Slider::relocateTool()
{
    toolInfo_.rect=rect(pos_,SIZE({length_,UNIT_LENGTH}));
    pt->SendMessageW(toolTip_,TTM_NEWTOOLRECTW,0,(LPARAM)&toolInfo_);
}

void Slider::render() const
{
    renderBar();
    renderKnob();
    renderText();
}

void Slider::renderBar() const
{
    HBRUSH foreBrush,backBrush;
    if(active_)
    {
        foreBrush=(HBRUSH)ct->componentBrush1->handle();
        backBrush=(HBRUSH)ct->componentBrush2->handle();
    } 
    else
    {
        foreBrush=(HBRUSH)ct->componentBrush2->handle();
        backBrush=(HBRUSH)ct->componentBrush1->handle();
    }
    const LONG length=length_-SLIDER_TEXT_WIDTH-UNIT_LENGTH;
    RECT backRect({0,0,length,SLIDER_BAR_WIDTH});
    nm::FillRect(barBuffer_->dc(),&backRect,backBrush);
    RECT foreRect({1,1,length-1,SLIDER_BAR_WIDTH-1});
    nm::FillRect(barBuffer_->dc(),&foreRect,foreBrush);
}

void Slider::renderKnob() const
{
    HPEN backPen;
    HBRUSH foreBrush,backBrush;
    if(active_)
    {
        foreBrush=(HBRUSH)ct->componentBrush2->handle();
        backPen=(HPEN)ct->componentPen2->handle();
        backBrush=(HBRUSH)ct->componentBrush1->handle();
    } else
    {
        foreBrush=(HBRUSH)ct->componentBrush1->handle();
        backPen=(HPEN)ct->componentPen1->handle();
        backBrush=(HBRUSH)ct->componentBrush2->handle();
    }
    nm::FillRect(knobForeBuffer_->dc(),&UNIT_RECT,foreBrush);
    nm::BitBlt
    (
        knobBackBuffer_->dc(),
        0,
        0,
        UNIT_LENGTH,
        UNIT_LENGTH,
        knobIconMaskBuffer_->dc(),
        0,
        0,
        NOTSRCCOPY
    );
    nm::BitBlt
    (
        knobForeBuffer_->dc(),
        0,
        0,
        UNIT_LENGTH,
        UNIT_LENGTH,
        knobBackBuffer_->dc(),
        0,
        0,
        SRCAND
    );
    nm::FillRect
    (knobBackBuffer_->dc(),&UNIT_RECT,(HBRUSH)ct->blackBrush->handle());
    pt->SelectObject(knobBackBuffer_->dc(),backPen);
    pt->SelectObject(knobBackBuffer_->dc(),backBrush);
    nm::Ellipse(knobBackBuffer_->dc(),0,0,UNIT_LENGTH,UNIT_LENGTH);
    nm::BitBlt
    (
        knobBackBuffer_->dc(),
        0,
        0,
        UNIT_LENGTH,
        UNIT_LENGTH,
        knobIconMaskBuffer_->dc(),
        0,
        0,
        SRCAND
    );
    nm::BitBlt
    (
        knobBackBuffer_->dc(),
        0,
        0,
        UNIT_LENGTH,
        UNIT_LENGTH,
        knobForeBuffer_->dc(),
        0,
        0,
        SRCPAINT
    );
}

void Slider::renderText() const
{
    constexpr UINT FORMAT=DT_BOTTOM|DT_RIGHT|DT_SINGLELINE;
    const string text=getText_(value_);
    RECT textRect=rect(POINT({0,0}),SLIDER_TEXT_SIZE);
    nm::FillRect
    (textBuffer_->dc(),&textRect,(HBRUSH)ct->blackBrush->handle());
    nm::FillRect
    (textMaskBuffer_->dc(),&textRect,(HBRUSH)ct->whiteBrush->handle());
    COLORREF foreTextColor,backTextColor;
    if(active_)
    {
        foreTextColor=ps->componentColor1;
        backTextColor=ps->componentColor2;
    } else
    {
        foreTextColor=ps->componentColor2;
        backTextColor=ps->componentColor1;
    }
    nm::SetTextColor(textBuffer_->dc(),backTextColor);
    nm::SetTextColor(textMaskBuffer_->dc(),BLACK_COLOR);
    textRect.right--;
    nm::DrawText
    (
        textBuffer_->dc(),
        text.c_str(),
        text.length(),
        &textRect,
        FORMAT
    );
    nm::DrawText
    (
        textMaskBuffer_->dc(),
        text.c_str(),
        text.length(),
        &textRect,
        FORMAT
    );
    textRect.bottom-=2;
    nm::DrawText
    (
        textBuffer_->dc(),
        text.c_str(),
        text.length(),
        &textRect,
        FORMAT
    );
    nm::DrawText
    (
        textMaskBuffer_->dc(),
        text.c_str(),
        text.length(),
        &textRect,
        FORMAT
    );
    textRect.bottom++;
    textRect.right++;
    nm::DrawText
    (
        textBuffer_->dc(),
        text.c_str(),
        text.length(),
        &textRect,
        FORMAT
    );
    nm::DrawText
    (
        textMaskBuffer_->dc(),
        text.c_str(),
        text.length(),
        &textRect,
        FORMAT
    );
    textRect.right-=2;
    nm::DrawText
    (
        textBuffer_->dc(),
        text.c_str(),
        text.length(),
        &textRect,
        FORMAT
    );
    nm::DrawText
    (
        textMaskBuffer_->dc(),
        text.c_str(),
        text.length(),
        &textRect,
        FORMAT
    );
    nm::SetTextColor(textBuffer_->dc(),foreTextColor);
    textRect.right++;
    nm::DrawText
    (
        textBuffer_->dc(),
        text.c_str(),
        text.length(),
        &textRect,
        FORMAT
    );
}

void Slider::updateKnobX()
{
    const LONG barLength=length_-SLIDER_TEXT_WIDTH-UNIT_LENGTH;
    const int offset=value_-minimum_;
    const int range=maximum_-minimum_;
    knobX_=barLength*offset/range;
}

}
