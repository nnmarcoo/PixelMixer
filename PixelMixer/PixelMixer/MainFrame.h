#pragma once
#include <wx/wx.h>

class MainFrame final : public wxFrame
{
public:
    explicit MainFrame(const wxString& title);

private:
    wxIcon icon_ = wxIcon("C:/Users/marco/Documents/GitHub/PixelMixer/PixelMixer/Images/icon2.ico", wxBITMAP_TYPE_ICO);
    wxDECLARE_EVENT_TABLE();
    
    bool isDragging_;
    wxPoint dragStart_;
    void OnHeaderLeftDown(const wxMouseEvent& e);
    void OnHeaderLeftUp(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnTitleBarDoubleClick(wxMouseEvent& e);
    void OnExitButtonClick(wxCommandEvent& e);
    void OnMaximizeButtonClick(wxCommandEvent& e);
    void OnMinimizeButtonClick(wxCommandEvent& e);
    void OnIconButtonClick(wxCommandEvent& e);
    
    // https://stackoverflow.com/questions/41179437/wxwidgets-wxborder-none-and-wxresize-border-makes-white-area
    WXLRESULT MSWWindowProc(WXUINT nMsg, WXWPARAM wParam, WXLPARAM lParam) override
    {
        switch (nMsg)
        {
        case WM_NCACTIVATE:
            {
                lParam = -1;
                break;
            }
        case WM_NCCALCSIZE:
            if (wParam)
            {
                const HWND hWnd = (HWND)this->GetHandle();
                WINDOWPLACEMENT wPos;
                wPos.length = sizeof(wPos);
                GetWindowPlacement(hWnd, &wPos);
                if (wPos.showCmd != SW_SHOWMAXIMIZED)
                {
                    RECT borderThickness;
                    SetRectEmpty(&borderThickness);
                    AdjustWindowRectEx(&borderThickness,
                        GetWindowLongPtr(hWnd, GWL_STYLE) & ~WS_CAPTION, FALSE, NULL);
                    borderThickness.left *= -1;
                    borderThickness.top *= -1;
                    NCCALCSIZE_PARAMS* sz = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
                    sz->rgrc[0].top += 1;
                    sz->rgrc[0].left += borderThickness.left;
                    sz->rgrc[0].right -= borderThickness.right;
                    sz->rgrc[0].bottom -= borderThickness.bottom;
                    return 0;
                }
            }
            break;
        default: ;
        }
        return wxFrame::MSWWindowProc(nMsg, wParam, lParam);
    }
};
