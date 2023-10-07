#include "MainFrame.h"
#include "Palette.h"

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
                 statusBar->SetForegroundColour(Palette::text);
                 statusBar->SetBackgroundColour(Palette::border);
    statustext_ = new wxStaticText( statusBar, wxID_ANY,wxT("Version 1.6.6-alpha"), wxPoint(5, 5), wxDefaultSize, 0); // Set colored StatusBar text

    // Create splitters to organize the frame
    auto* hsplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);
    auto* vsplitter = new wxSplitterWindow(hsplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);
    
    auto* headerPanel = new HeaderPanel(hsplitter);
    auto* viewportPanel = new ViewportPanel(vsplitter, &headerPanel->isDragging_);
    auto* configPanel = new ConfigPanel(vsplitter, viewportPanel);
    headerPanel->BindViewport(viewportPanel);

    vsplitter->SplitVertically(configPanel, viewportPanel); // Split the left(config) and right(viewport)
    vsplitter->SetSashPosition(configpanelwidth_);
    hsplitter->SplitHorizontally(headerPanel, vsplitter); // Split the top(header) and bottom(main region)
    hsplitter->SetSashPosition(30);
    
    SetWindowStyle(wxSYSTEM_MENU | wxRESIZE_BORDER| wxCLIP_CHILDREN); // Must be after status bar declaration
    CenterOnScreen();
}