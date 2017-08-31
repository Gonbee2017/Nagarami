#include<algorithm>
#include<cmath>
#include"nagarami.h"
#include<memory>
#include<windows.h>
#include<commctrl.h>

namespace nagarami
{

bool Component::active() {return active_;}

Component::Component
(HWND container,HWND tool,UINT toolId,const string&toolHint):
    tool_(tool),
    toolHint_(toolHint),
    active_(false),
    move([] (const SIZE&) {})
{
    toolInfo_.cbSize=sizeof(TOOLINFO);
    toolInfo_.uFlags=TTF_SUBCLASS;
    toolInfo_.hwnd=container;
    toolInfo_.uId=toolId;
    toolInfo_.rect=RECT({0,0,0,0});
    toolInfo_.hinst=NULL;
    toolInfo_.lpszText=(LPTSTR)TEXT(toolHint_.c_str());
    if(SendMessage(tool_,TTM_ADDTOOL,0,(LPARAM)&toolInfo_)==FALSE)
        throw make_shared<runtime_error>(describe
        ("SendMessage(TTM_ADDTOOL) is failed."));
}

void Button::activate(const POINT&cursorPos)
{
    active_=true;
    render();
}

bool Button::hitTest(const POINT&cursorPos)
{
    return
        contain(rect(pos,UNIT_SIZE),cursorPos)&&
        contain
        (pos+HALF_UNIT_LENGTH,SQUARED_HALF_UNIT_LENGTH,cursorPos);
}

void Button::moveTool()
{
    toolInfo_.rect=rect(pos,UNIT_SIZE);
    SendMessage(tool_,TTM_NEWTOOLRECT,0,(LPARAM)&toolInfo_);
}

void Button::paint(HDC dc)
{
    nagarami::BitBlt
    (
        dc,
        pos.x,
        pos.y,
        UNIT_LENGTH,
        UNIT_LENGTH,
        maskBuffer_->dc(),
        0,
        0,
        SRCAND
    );
    nagarami::BitBlt
    (
        dc,
        pos.x,
        pos.y,
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
    HWND container,
    HWND tool,
    UINT toolId,
    const string&toolHint
):
    Component(container,tool,toolId,toolHint),
    iconMaskBuffer_(Buffer::load(ct().instance,maskBitmapName,destDC)),
    foreBuffer_(Buffer::create(UNIT_SIZE,destDC)),
    backBuffer_(Buffer::create(UNIT_SIZE,destDC)),
    maskBuffer_(Buffer::create(UNIT_SIZE,destDC)),
    click([] () {})
{
    nagarami::FillRect
    (maskBuffer_->dc(),&UNIT_RECT,(HBRUSH)ct().white_brush->handle());
    SelectObject(maskBuffer_->dc(),ct().black_pen->handle());
    SelectObject(maskBuffer_->dc(),ct().black_brush->handle());
    nagarami::Ellipse(maskBuffer_->dc(),0,0,UNIT_LENGTH,UNIT_LENGTH);
}

void Button::render_(const bool&push)
{
    HPEN backPen;
    HBRUSH foreBrush,backBrush;
    if(push)
    {
        foreBrush=(HBRUSH)ct().component_brush2->handle();
        backPen=(HPEN)ct().component_pen2->handle();
        backBrush=(HBRUSH)ct().component_brush1->handle();
    } else
    {
        foreBrush=(HBRUSH)ct().component_brush1->handle();
        backPen=(HPEN)ct().component_pen1->handle();
        backBrush=(HBRUSH)ct().component_brush2->handle();
    }
    nagarami::FillRect(foreBuffer_->dc(),&UNIT_RECT,foreBrush);
    nagarami::BitBlt
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
    nagarami::BitBlt
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
    nagarami::FillRect
    (backBuffer_->dc(),&UNIT_RECT,(HBRUSH)ct().black_brush->handle());
    SelectObject(backBuffer_->dc(),backPen);
    SelectObject(backBuffer_->dc(),backBrush);
    nagarami::Ellipse(backBuffer_->dc(),0,0,UNIT_LENGTH,UNIT_LENGTH);
    nagarami::BitBlt
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
    nagarami::BitBlt
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
    HWND container,
    HWND tool,
    UINT toolId,
    const string&toolHint
):Button(maskBitmapName,destDC,container,tool,toolId,toolHint) {render();}

void PushButton::deactivate(const POINT&cursorPos)
{
    if(control_mode()&&hitTest(cursorPos)) click();
    active_=false;
    render();
}

void PushButton::render() {render_(active_);}

RadioButton::RadioButton
(
    const bool&initialValue,
    LPCTSTR maskBitmapName,
    HDC destDC,
    HWND container,
    HWND tool,
    UINT toolId,
    const string&toolHint
):
    Button(maskBitmapName,destDC,container,tool,toolId,toolHint),
    value_(initialValue) {render();}

bool RadioButton::value() {return value_;}

void RadioButton::value(const bool&value_)
{
    this->value_=value_;
    render();
}

void RadioButton::deactivate(const POINT&cursorPos)
{
    if(control_mode()&&hitTest(cursorPos))
    {
        value_=!value_;
        click();
    }
    active_=false;
    render();
}

void RadioButton::render() {render_(active_||value_);}

Slider::Slider
(
    const int&minimum,
    const int&maximum,
    const int&initialValue,
    function<string(const int&value)> getText,
    const LONG&initialLength,
    LPCTSTR knobMaskBitmapName,
    HDC destDC,
    HWND container,
    HWND tool,
    UINT toolId,
    const string&toolHint
):
    Component(container,tool,toolId,toolHint),
    minimum_(minimum),
    maximum_(maximum),
    value_(initialValue),
    getText_(getText),
    length_(initialLength),
    knobIconMaskBuffer_
    (Buffer::load(ct().instance,knobMaskBitmapName,destDC)),
    knobForeBuffer_(Buffer::create(UNIT_SIZE,destDC)),
    knobBackBuffer_(Buffer::create(UNIT_SIZE,destDC)),
    knobMaskBuffer_(Buffer::create(UNIT_SIZE,destDC)),
    barBuffer_
    (Buffer::create(SIZE({desktop_size().cx,SLIDER_BAR_WIDTH}),destDC)),
    textBuffer_(Buffer::create(SLIDER_TEXT_SIZE,destDC)),
    textMaskBuffer_(Buffer::create(SLIDER_TEXT_SIZE,destDC)),
    change([] () {})
{
    updateKnobX();
    nagarami::FillRect
    (knobMaskBuffer_->dc(),&UNIT_RECT,(HBRUSH)ct().white_brush->handle());
    SelectObject(knobMaskBuffer_->dc(),ct().black_pen->handle());
    SelectObject(knobMaskBuffer_->dc(),ct().black_brush->handle());
    nagarami::Ellipse(knobMaskBuffer_->dc(),0,0,UNIT_LENGTH,UNIT_LENGTH);
    SelectObject(textBuffer_->dc(),ct().font->handle());
    nagarami::SetBkMode(textBuffer_->dc(),TRANSPARENT);
    SelectObject(textMaskBuffer_->dc(),ct().font->handle());
    nagarami::SetBkMode(textMaskBuffer_->dc(),TRANSPARENT);
    render();
}

void Slider::activate(const POINT&cursorPos)
{
    if(hitTestKnob(cursorPos)) pinch_=cursorPos.x-pos.x-knobX_;
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

bool Slider::hitTest(const POINT&cursorPos)
{return hitTestKnob(cursorPos)||hitTestBar(cursorPos);}

void Slider::length(const LONG&length_)
{
    this->length_=length_;
    updateKnobX();
    renderBar();
}

void Slider::moveTool()
{
    toolInfo_.rect=rect(pos,SIZE({length_,UNIT_LENGTH}));
    SendMessage(tool_,TTM_NEWTOOLRECT,0,(LPARAM)&toolInfo_);
}

void Slider::paint(HDC dc)
{
    const LONG barLength=length_-SLIDER_TEXT_WIDTH-UNIT_LENGTH;
    nagarami::BitBlt
    (
        dc,
        pos.x+HALF_UNIT_LENGTH,
        pos.y+HALF_UNIT_LENGTH-SLIDER_HALF_BAR_WIDTH,
        barLength,
        SLIDER_BAR_WIDTH,
        barBuffer_->dc(),
        0,
        0,
        SRCCOPY
    );
    nagarami::BitBlt
    (
        dc,
        pos.x+knobX_,
        pos.y,
        UNIT_LENGTH,
        UNIT_LENGTH,
        knobMaskBuffer_->dc(),
        0,
        0,
        SRCAND
    );
    nagarami::BitBlt
    (
        dc,
        pos.x+knobX_,
        pos.y,
        UNIT_LENGTH,
        UNIT_LENGTH,
        knobBackBuffer_->dc(),
        0,
        0,
        SRCPAINT
    );
    const LONG textX=pos.x+length_-SLIDER_TEXT_WIDTH;
    nagarami::BitBlt
    (
        dc,
        textX,
        pos.y,
        SLIDER_TEXT_WIDTH,
        UNIT_LENGTH,
        textMaskBuffer_->dc(),
        0,
        0,
        SRCAND
    );
    nagarami::BitBlt
    (
        dc,
        textX,
        pos.y,
        SLIDER_TEXT_WIDTH,
        UNIT_LENGTH,
        textBuffer_->dc(),
        0,
        0,
        SRCPAINT
    );
}

void Slider::update(const POINT&cursorPos)
{
    if(GetKeyState(VK_LBUTTON)<0)
    {
        const int range=maximum_-minimum_;
        const LONG x=cursorPos.x-pos.x-pinch_;
        const LONG barLength=length_-SLIDER_TEXT_WIDTH-UNIT_LENGTH;
        value_=minimum_+(int)round(range*x/(double)barLength);
        value_=min(max(value_,minimum_),maximum_);
        change();
        updateKnobX();
        renderText();
    }
}

int Slider::value() {return value_;}

void Slider::value(const int&value_)
{
    this->value_=value_;
    change();
    updateKnobX();
    renderText();
}

bool Slider::hitTestBar(const POINT&cursorPos)
{
    const POINT barPos=POINT(
    {
        pos.x+HALF_UNIT_LENGTH,
        pos.y+HALF_UNIT_LENGTH-SLIDER_HALF_BAR_WIDTH
    });
    const SIZE barSize=
        SIZE({length_-SLIDER_TEXT_WIDTH-UNIT_LENGTH,SLIDER_BAR_WIDTH});
    return contain(rect(barPos,barSize),cursorPos);
}

bool Slider::hitTestKnob(const POINT&cursorPos)
{
    const POINT knobPos=POINT({pos.x+knobX_,pos.y});
    return
        contain(rect(knobPos,UNIT_SIZE),cursorPos)&&
        contain
        (knobPos+HALF_UNIT_LENGTH,SQUARED_HALF_UNIT_LENGTH,cursorPos);
}

void Slider::render()
{
    renderKnob();
    renderBar();
    renderText();
}

void Slider::renderBar()
{
    HBRUSH foreBrush,backBrush;
    if(active_)
    {
        foreBrush=(HBRUSH)ct().component_brush1->handle();
        backBrush=(HBRUSH)ct().component_brush2->handle();
    } 
    else
    {
        foreBrush=(HBRUSH)ct().component_brush2->handle();
        backBrush=(HBRUSH)ct().component_brush1->handle();
    }
    const LONG length=length_-SLIDER_TEXT_WIDTH-UNIT_LENGTH;
    RECT backRect=RECT({0,0,length,SLIDER_BAR_WIDTH});
    nagarami::FillRect(barBuffer_->dc(),&backRect,backBrush);
    RECT foreRect=RECT({1,1,length-1,SLIDER_BAR_WIDTH-1});
    nagarami::FillRect(barBuffer_->dc(),&foreRect,foreBrush);
}

void Slider::renderKnob()
{
    HPEN backPen;
    HBRUSH foreBrush,backBrush;
    if(active_)
    {
        foreBrush=(HBRUSH)ct().component_brush2->handle();
        backPen=(HPEN)ct().component_pen2->handle();
        backBrush=(HBRUSH)ct().component_brush1->handle();
    } else
    {
        foreBrush=(HBRUSH)ct().component_brush1->handle();
        backPen=(HPEN)ct().component_pen1->handle();
        backBrush=(HBRUSH)ct().component_brush2->handle();
    }
    nagarami::FillRect(knobForeBuffer_->dc(),&UNIT_RECT,foreBrush);
    nagarami::BitBlt
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
    nagarami::BitBlt
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
    nagarami::FillRect
    (knobBackBuffer_->dc(),&UNIT_RECT,(HBRUSH)ct().black_brush->handle());
    SelectObject(knobBackBuffer_->dc(),backPen);
    SelectObject(knobBackBuffer_->dc(),backBrush);
    nagarami::Ellipse(knobBackBuffer_->dc(),0,0,UNIT_LENGTH,UNIT_LENGTH);
    nagarami::BitBlt
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
    nagarami::BitBlt
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

void Slider::renderText()
{
    constexpr UINT FORMAT=DT_BOTTOM|DT_RIGHT|DT_SINGLELINE;
    text_=getText_(value_);
    RECT textRect=rect(POINT({0,0}),SLIDER_TEXT_SIZE);
    nagarami::FillRect
    (textBuffer_->dc(),&textRect,(HBRUSH)ct().black_brush->handle());
    nagarami::FillRect
    (textMaskBuffer_->dc(),&textRect,(HBRUSH)ct().white_brush->handle());
    COLORREF foreTextColor,backTextColor;
    if(active_)
    {
        foreTextColor=ct().ps.component_color1;
        backTextColor=ct().ps.component_color2;
    } else
    {
        foreTextColor=ct().ps.component_color2;
        backTextColor=ct().ps.component_color1;
    }
    nagarami::SetTextColor(textBuffer_->dc(),backTextColor);
    nagarami::SetTextColor(textMaskBuffer_->dc(),BLACK_COLOR);
    textRect.right--;
    nagarami::DrawText
    (
        textBuffer_->dc(),
        TEXT(text_.c_str()),
        text_.length(),
        &textRect,
        FORMAT
    );
    nagarami::DrawText
    (
        textMaskBuffer_->dc(),
        TEXT(text_.c_str()),
        text_.length(),
        &textRect,
        FORMAT
    );
    textRect.bottom-=2;
    nagarami::DrawText
    (
        textBuffer_->dc(),
        TEXT(text_.c_str()),
        text_.length(),
        &textRect,
        FORMAT
    );
    nagarami::DrawText
    (
        textMaskBuffer_->dc(),
        TEXT(text_.c_str()),
        text_.length(),
        &textRect,
        FORMAT
    );
    textRect.bottom++;
    textRect.right++;
    nagarami::DrawText
    (
        textBuffer_->dc(),
        TEXT(text_.c_str()),
        text_.length(),
        &textRect,
        FORMAT
    );
    nagarami::DrawText
    (
        textMaskBuffer_->dc(),
        TEXT(text_.c_str()),
        text_.length(),
        &textRect,
        FORMAT
    );
    textRect.right-=2;
    nagarami::DrawText
    (
        textBuffer_->dc(),
        TEXT(text_.c_str()),
        text_.length(),
        &textRect,
        FORMAT
    );
    nagarami::DrawText
    (
        textMaskBuffer_->dc(),
        TEXT(text_.c_str()),
        text_.length(),
        &textRect,
        FORMAT
    );
    nagarami::SetTextColor(textBuffer_->dc(),foreTextColor);
    textRect.right++;
    nagarami::DrawText
    (
        textBuffer_->dc(),
        TEXT(text_.c_str()),
        text_.length(),
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
