#include "MainFrame.h"
#include "HeaderPanel.h"

//todo make the vertical splitter sash a custom color

// File -> Open, save, export, import, settings

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize) { // constructor
    // Layout setup
    
    wxStatusBar* statusBar = wxFrameBase::CreateStatusBar();
                 statusBar->SetForegroundColour("#bfbfbf");
                 statusBar->SetBackgroundColour("#2c2f33");
    // Set colored StatusBar text
    statusText = new wxStaticText( statusBar, wxID_ANY,wxT("Version 1.1.2-alpha"), wxPoint(5, 5), wxDefaultSize, 0);
    
    SetClientSize(wxGetDisplaySize() * 0.8);
    SetIcon(icon_);
    wxTopLevelWindowBase::SetMinSize(wxSize((GetSize().GetWidth() / 5) * 2, (GetSize().GetHeight() / 5) * 2)); // Min size is double the config column

    // Create the vertical and horizontal splitters to organize the frame
    auto* hSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);
    auto* vSplitter = new wxSplitterWindow(hSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);

    // Create the panels to color the split regions
    auto* configPanel = new wxPanel(vSplitter);
    auto* viewportPanel = new wxPanel(vSplitter);
    auto* headerPanel = new HeaderPanel(hSplitter);

    // Set the panel colors
    viewportPanel->SetBackgroundColour("#36393e");
      configPanel->SetBackgroundColour("#282b30");

    // Configure the vertical splitter
        vSplitter->SplitVertically(configPanel, viewportPanel); // Split the left(config) and right(viewport)
        vSplitter->SetSashPosition(GetSize().GetWidth() / 5);

    // configure the horizontal splitter
        hSplitter->SplitHorizontally(headerPanel, vSplitter); // Split the top(header) and bottom(main region)
        hSplitter->SetSashPosition(30);
    
    SetWindowStyle(wxSYSTEM_MENU | wxRESIZE_BORDER| wxCLIP_CHILDREN); // Must be after status bar declaration
    CenterOnScreen();
}
