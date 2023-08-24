#pragma once
#include <wx/wx.h>
#include <wx/display.h>

class HeaderPanel : public wxPanel {
public:
    HeaderPanel(wxWindow* parent);
    
private:
    DECLARE_EVENT_TABLE()
    wxIcon icon_ = wxIcon("C:/Users/marco/Documents/GitHub/PixelMixer/PixelMixer/Images/icon2.ico", wxBITMAP_TYPE_ICO); // duplicate
    
    bool isDragging_;
    wxPoint dragStart_;
    void OnHeaderLeftDown(wxMouseEvent& e);
    void OnHeaderLeftUp(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnHeaderDoubleClick(wxMouseEvent& e);
    
    void OnExitButtonClick(wxCommandEvent& e);
    void OnMaximizeButtonClick(wxCommandEvent& e);
    void OnMinimizeButtonClick(wxCommandEvent& e);
    void OnIconButtonClick(wxCommandEvent& e);

    wxFrame* mainFrame = dynamic_cast<wxFrame*>(wxGetTopLevelParent(this));
};
