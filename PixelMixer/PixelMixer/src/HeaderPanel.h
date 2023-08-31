#pragma once
#include <wx/wx.h>
#include <wx/display.h>

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
    void OnHeaderLeftDown(wxMouseEvent& e);
    void OnHeaderLeftUp(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnHeaderDoubleClick(wxMouseEvent& e);
    
    void OnExitButtonClick(wxCommandEvent& e);
    void OnMaximizeButtonClick(wxCommandEvent& e);
    void OnMinimizeButtonClick(wxCommandEvent& e);
    void OnIconButtonClick(wxCommandEvent& e);

    wxButton* exitbutton_;
    wxButton* maximizebutton_;
    wxButton* minimizebutton_;
    wxButton* iconbutton_;
    wxButton* dropdownbutton_;

    void ToggleMaximize() const;

    wxFrame* mainframe_ = dynamic_cast<wxFrame*>(wxGetTopLevelParent(this));
};
