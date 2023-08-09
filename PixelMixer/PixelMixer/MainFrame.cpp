#include "MainFrame.h"
#include <wx/wx.h>

enum IDs
{
    exitB_ID = 2
};

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title) // constructor
{
    //SetWindowStyle(wxFRAME_NO_TASKBAR);
    wxWindowBase::SetBackgroundColour("#23272a"); // should this be in App.cpp
    
    SetClientSize(wxGetDisplaySize() * 0.8);
    Center();

    wxStatusBar* statusBar = wxFrameBase::CreateStatusBar();
    statusBar->SetBackgroundColour("#2c2f33");
}