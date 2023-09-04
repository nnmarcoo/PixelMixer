#pragma once
#include <wx/wx.h>
#include <wx/display.h>

#include "Button.h"
#include "ViewportPanel.h"

class HeaderPanel : public wxPanel {
public:
    HeaderPanel(wxWindow* parent);
    bool isDragging_;
    
private:
    DECLARE_EVENT_TABLE()
    wxIcon logoicon_ = wxIcon("res/images/icon2.ico", wxBITMAP_TYPE_ICO); // dup also in MainFrame
    wxIcon dropdownicon_ = wxIcon("res/images/dropdown.ico", wxBITMAP_TYPE_ICO);
    
    wxPoint dragStart_;
    void OnLeftDown(wxMouseEvent& e);
    void OnLeftUp(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnDoubleClick(wxMouseEvent& e);
    
    void OnExitButtonClick(wxCommandEvent& e);
    void OnMaximizeButtonClick(wxCommandEvent& e);
    void OnMinimizeButtonClick(wxCommandEvent& e);
    void OnIconButtonClick(wxCommandEvent& e);
    void OnDropDownButtonClick(wxCommandEvent& e);

    Button* exitbutton_;
    Button* maximizebutton_;
    Button* minimizebutton_;
    Button* iconbutton_;
    Button* dropdownbutton_;

    void ToggleMaximize() const;

    wxFrame* mainframe_ = dynamic_cast<wxFrame*>(wxGetTopLevelParent(this));
};
