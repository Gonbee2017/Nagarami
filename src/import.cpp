#include"nagarami.h"
#include<fstream>
#include<windows.h>

namespace nm
{

void import()
{
    pt().BeginPaint=&::BeginPaint;
    pt().BitBlt=&::BitBlt;
    pt().CreateCompatibleBitmap=&::CreateCompatibleBitmap;
    pt().CreateCompatibleDC=&::CreateCompatibleDC;
    pt().CreateFont=&::CreateFont;
    pt().CreatePatternBrush=&::CreatePatternBrush;
    pt().CreatePen=&::CreatePen;
    pt().CreateSolidBrush=&::CreateSolidBrush;
    pt().CreateWindowEx=&::CreateWindowEx;
    pt().DefWindowProc=&::DefWindowProc;
    pt().DeleteDC=&::DeleteDC;
    pt().DeleteObject=&::DeleteObject;
    pt().DestroyWindow=&::DestroyWindow;
    pt().DispatchMessage=&::DispatchMessage;
    pt().DrawText=&::DrawText;
    pt().Ellipse=&::Ellipse;
    pt().EndPaint=&::EndPaint;
    pt().FillRect=&::FillRect;
    pt().GetClientRect=&::GetClientRect;
    pt().GetCursorPos=&::GetCursorPos;
    pt().GetDC=&::GetDC;
    pt().GetForegroundWindow=&::GetForegroundWindow;
    pt().GetKeyState=&::GetKeyState;
    pt().GetLastError=&::GetLastError;
    pt().GetMessage=&::GetMessage;
    pt().GetObject=&::GetObject;
    pt().GetSystemMetrics=&::GetSystemMetrics;
    pt().GetWindowPlacement=&::GetWindowPlacement;
    pt().IsIconic=&::IsIconic;
    pt().IsWindow=&::IsWindow;
    pt().IsWindowVisible=&::IsWindowVisible;
    pt().IsZoomed=&::IsZoomed;
    pt().LoadBitmap=&::LoadBitmap;
    pt().LoadCursor=&::LoadCursor;
    pt().MessageBox=&::MessageBox;
    pt().PostQuitMessage=&::PostQuitMessage;
    pt().RedrawWindow=&::RedrawWindow;
    pt().ReleaseDC=&::ReleaseDC;
    pt().RegisterClassEx=&::RegisterClassEx;
    pt().ReleaseCapture=&::ReleaseCapture;
    pt().ScreenToClient=&::ScreenToClient;
    pt().SelectObject=&::SelectObject;
    pt().SendMessageW=&::SendMessageW;
    pt().SetBkMode=&::SetBkMode;
    pt().SetBrushOrgEx=&::SetBrushOrgEx;
    pt().SetCapture=&::SetCapture;
    pt().SetCursor=&::SetCursor;
    pt().SetForegroundWindow=&::SetForegroundWindow;
    pt().SetLayeredWindowAttributes=&::SetLayeredWindowAttributes;
    pt().SetStretchBltMode=&::SetStretchBltMode;
    pt().SetTextColor=&::SetTextColor;
    pt().ShellExecute=&::ShellExecute;
    pt().ShowWindow=&::ShowWindow;
    pt().StretchBlt=&::StretchBlt;
    pt().input_file=&nm::input_file;
    pt().output_file=&nm::output_file;
    pt().timeBeginPeriod=&::timeBeginPeriod;
    pt().timeEndPeriod=&::timeEndPeriod;
    pt().timeGetDevCaps=&::timeGetDevCaps;
    pt().timeKillEvent=&::timeKillEvent;
    pt().timeSetEvent=&::timeSetEvent;
}

shared_ptr<istream> input_file
(const string&name,const ios_base::openmode&mode)
{return make_shared<ifstream>(name,mode);}

shared_ptr<ostream> output_file
(const string&name,const ios_base::openmode&mode)
{return make_shared<ofstream>(name,mode);}

}
