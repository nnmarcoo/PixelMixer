#include "MainFrame.h"
#include <wx/wx.h>

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title) // constructor
{
    wxWindowBase::SetBackgroundColour("#23272a"); // should this be in App.cpp
    //SetWindowStyle(wxFRAME_NO_TASKBAR);
    
    SetClientSize(wxGetDisplaySize() * 0.8);
    Center();
    
}