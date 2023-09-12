#include "MainFrame.h"

#include "ConfigPanel.h"
#include "HeaderPanel.h"
#include "ViewportPanel.h"

//todo make the vertical splitter sash a custom color

// File -> Open, save, export, import, settings

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize) {
    SetClientSize(wxGetDisplaySize() * 0.8);
    SetIcon(logoicon_);
    wxTopLevelWindowBase::SetMinSize(wxSize((GetSize().GetWidth() / 5) * 2, (GetSize().GetHeight() / 5) * 2)); // Min size is double the config column
    configpanelwidth_ = GetSize().GetWidth() / 5;
    
    wxStatusBar* statusBar = wxFrameBase::CreateStatusBar();
                 statusBar->SetForegroundColour("#bfbfbf");
                 statusBar->SetBackgroundColour("#2c2f33");
    statustext_ = new wxStaticText( statusBar, wxID_ANY,wxT("Version 1.5.0-alpha"), wxPoint(5, 5), wxDefaultSize, 0); // Set colored StatusBar text

    // Create splitters to organize the frame
    auto* hSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);
    auto* vSplitter = new wxSplitterWindow(hSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);
    
    auto* configPanel = new ConfigPanel(vSplitter);
    auto* headerPanel = new HeaderPanel(hSplitter);
    auto* viewportPanel = new ViewportPanel(vSplitter, &headerPanel->isDragging_);
    headerPanel->BindViewport(viewportPanel);
    configPanel->BindViewport(viewportPanel);

    vSplitter->SplitVertically(configPanel, viewportPanel); // Split the left(config) and right(viewport)
    vSplitter->SetSashPosition(configpanelwidth_);
    hSplitter->SplitHorizontally(headerPanel, vSplitter); // Split the top(header) and bottom(main region)
    hSplitter->SetSashPosition(30);
    
    SetWindowStyle(wxSYSTEM_MENU | wxRESIZE_BORDER| wxCLIP_CHILDREN); // Must be after status bar declaration
    CenterOnScreen();
}