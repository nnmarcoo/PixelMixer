#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/splitter.h>

//todo window size changes but the panel does not
//todo make the vertical splitter sash a custom color
//todo make the images paths relative

const std::string ICON_PATH = "C:/Users/marco/Documents/GitHub/PixelMixer/PixelMixer/Images/icon2.ico";

enum IDs
{
    exitB_ID = 2
};

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize) // constructor
{
    // Layout setup

    // Store the mem address of the created wxStatusBar object
    wxStatusBar* statusBar = wxFrameBase::CreateStatusBar();

    // Create the vertical and horizontal splitters to organize the frame
    auto* hSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);
    auto* vSplitter = new wxSplitterWindow(hSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);

    // Create the panels to color the split regions
    auto* configPanel = new wxPanel(vSplitter);
    auto* viewportPanel = new wxPanel(vSplitter);
    auto* headerPanel = new wxPanel(hSplitter);

    // Set the panel colors
    viewportPanel->SetBackgroundColour("#36393e");
      configPanel->SetBackgroundColour("#282b30");
      headerPanel->SetBackgroundColour("#2c2f33");

    // Configure the vertical splitter
        vSplitter->SetMinimumPaneSize(200);
        vSplitter->SetSashGravity(0);
        vSplitter->SplitVertically(configPanel, viewportPanel);
        vSplitter->SetSashPosition(100);

    // configure the horizontal splitter
        hSplitter->SplitHorizontally(headerPanel, vSplitter);
        hSplitter->SetSashPosition(30);

    // Configure the main window
    wxTopLevelWindowBase::SetMinSize(wxGetDisplaySize() * 0.3);
               statusBar->SetBackgroundColour("#2c2f33");
                          SetWindowStyle(wxFRAME_NO_TASKBAR);
                          SetIcon(wxIcon(ICON_PATH, wxBITMAP_TYPE_ICO));
                          SetClientSize(wxGetDisplaySize() * 0.8);
    CenterOnScreen();
    // Layout setup end
}

