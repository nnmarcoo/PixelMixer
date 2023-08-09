#include "MainFrame.h"
#include <wx/wx.h>

const std::string ICON_PATH = "C:/Users/marco/Documents/GitHub/PixelMixer/PixelMixer/Images/icon2.ico";

enum IDs
{
    exitB_ID = 2
};

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title) // constructor
{
    SetIcon(wxIcon(ICON_PATH, wxBITMAP_TYPE_ICO));
    wxWindowBase::SetBackgroundColour("#23272a"); // should this be in App.cpp
    
    SetClientSize(wxGetDisplaySize() * 0.8);
    wxTopLevelWindowBase::SetMinSize(wxGetDisplaySize() * 0.3);
    Center();

    wxStatusBar* statusBar = wxFrameBase::CreateStatusBar();
    statusBar->SetBackgroundColour("#2c2f33");
}