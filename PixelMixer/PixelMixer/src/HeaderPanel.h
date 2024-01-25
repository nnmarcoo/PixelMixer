#pragma once
#include <wx/wx.h>
#include <wx/display.h>

#include "Button.h"
#include "MainFrame.h"
#include "ViewportPanel.h"

enum Header_IDs {
    header_ID = 2,
    exitButton_ID = 3,
    maximizeButton_ID = 4,
    minimizeButton_ID = 5,
    iconButton_ID = 6,
    dropdownButton_ID = 7,
    dropdownFrame_ID = 8,
    centermediaButton_ID = 9,
    importmediaButton_ID = 10,
    exitdropdownButton_ID = 11,
    settingsButton_ID = 12,
    helpButton_ID = 13,
    exportmediaButton_ID = 14,
    screenshotButton_ID = 15,
    resetzoomButton_ID = 16
};

class HeaderPanel : public wxPanel {
public:
    HeaderPanel(wxWindow* parent);
    bool isDragging_;
    void ToggleMaximize() const;
    void BindViewport(ViewportPanel* viewport) {viewport_ = viewport;}
    
private:
    DECLARE_EVENT_TABLE()
    wxIcon logoicon_ = wxIcon("res/images/logo.ico", wxBITMAP_TYPE_ICO); // dup also in MainFrame
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
    

    wxFrame* dropdown_;
    wxFrame* settings_;
    void CreateDropDown();
    void CreateSettings();
    void OnCenterMediaClick(wxCommandEvent& e);
    void OnImportMediaClick(wxCommandEvent& e);
    void OnExportMediaClick(wxCommandEvent& e);
    void OnScreenshotClick(wxCommandEvent& e);
    void OnSettingsClick(wxCommandEvent& e);
    void OnHelpClick(wxCommandEvent& e);
    void OnResetZoomClick(wxCommandEvent& e);

    MainFrame* mainframe_ = dynamic_cast<MainFrame*>(wxGetTopLevelParent(this));
    ViewportPanel* viewport_;
};