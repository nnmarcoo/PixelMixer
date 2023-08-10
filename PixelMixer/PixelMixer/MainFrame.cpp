#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/splitter.h>

const std::string ICON_PATH = "C:/Users/marco/Documents/GitHub/PixelMixer/PixelMixer/Images/icon2.ico"; // not sure how to make relative yet

enum IDs
{
    exitB_ID = 2
};

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize) // constructor
{
    wxStatusBar* statusBar = wxFrameBase::CreateStatusBar();

    auto* HorizontalSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);
    auto* verticalSplitter = new wxSplitterWindow(HorizontalSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);
    
    auto* configPanel = new wxPanel(verticalSplitter);
    auto* viewportPanel = new wxPanel(verticalSplitter);
    auto* customHeaderPanel = new wxPanel(HorizontalSplitter);
    
    configPanel->SetBackgroundColour("#282b30");
    viewportPanel->SetBackgroundColour("#36393e");
    customHeaderPanel->SetBackgroundColour("#2c2f33");

    verticalSplitter->SetMinimumPaneSize(200);
    verticalSplitter->SetSashGravity(0);
    verticalSplitter->SplitVertically(configPanel, viewportPanel);
    verticalSplitter->SetSashPosition(100);

    HorizontalSplitter->SplitHorizontally(customHeaderPanel, verticalSplitter);
    HorizontalSplitter->SetSashPosition(30);

    wxTopLevelWindowBase::SetMinSize(wxGetDisplaySize() * 0.3);
               statusBar->SetBackgroundColour("#2c2f33");
                          SetWindowStyle(wxFRAME_NO_TASKBAR);
                          SetIcon(wxIcon(ICON_PATH, wxBITMAP_TYPE_ICO));
                          SetClientSize(wxGetDisplaySize() * 0.8);
    CenterOnScreen();
}

