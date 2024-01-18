#pragma once
#include <wx/wx.h>
#include <wx/splitter.h>

class MainFrame final : public wxFrame {
public:
    explicit MainFrame(const wxString& title);
    int configpanelwidth_;

private:
    wxIcon logoicon_ = wxIcon("res/images/logo.ico", wxBITMAP_TYPE_ICO); // dup also in HeaderPanel
    wxStaticText* statustext_;
    
    // https://stackoverflow.com/questions/41179437/wxwidgets-wxborder-none-and-wxresize-border-makes-white-area
#ifdef _WIN32
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
#endif // _WIN32
};