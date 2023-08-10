#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/splitter.h>

//todo make the vertical splitter sash a custom color

enum IDs
{
    header_ID = 2
};

MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize) // constructor
{
    // Layout setup
    
    // Store the mem address of the created wxStatusBar object
    wxStatusBar* statusBar = wxFrameBase::CreateStatusBar();

    // Configure the main window
    statusBar->SetBackgroundColour("#2c2f33");
    SetClientSize(wxGetDisplaySize() * 0.8);
    SetIcon(wxIcon(icon_path_, wxBITMAP_TYPE_ICO));
    wxTopLevelWindowBase::SetMinSize(wxSize((GetSize().GetWidth() / 5) * 2, (GetSize().GetHeight() / 5) * 2)); // Min size is double the config column

    // Create the vertical and horizontal splitters to organize the frame
    auto* hSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);
    auto* vSplitter = new wxSplitterWindow(hSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);

    // Create the panels to color the split regions
    auto* configPanel = new wxPanel(vSplitter);
    auto* viewportPanel = new wxPanel(vSplitter);
    auto* headerPanel = new wxPanel(hSplitter, header_ID);

    // Set the panel colors
    viewportPanel->SetBackgroundColour("#36393e");
      configPanel->SetBackgroundColour("#282b30");
      headerPanel->SetBackgroundColour("#2c2f33");

    // Configure the vertical splitter
        vSplitter->SplitVertically(configPanel, viewportPanel); // Split the left(config) and right(viewport)
        vSplitter->SetSashPosition(GetSize().GetWidth() / 5);

    // configure the horizontal splitter
        hSplitter->SplitHorizontally(headerPanel, vSplitter); // Split the top(header) and bottom(main region)
        hSplitter->SetSashPosition(FromDIP(30)); // is DIP necessary?
    
    SetWindowStyle(wxSYSTEM_MENU | wxRESIZE_BORDER| wxCLIP_CHILDREN); // Must be after status bar declaration
    CenterOnScreen();
    // Layout setup end

    //Bind header to event handlers so the window can be dragged and maximized
    headerPanel->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnHeaderLeftDown, this, header_ID);
    headerPanel->Bind(wxEVT_LEFT_UP, &MainFrame::OnHeaderLeftUp, this, header_ID);
    headerPanel->Bind(wxEVT_MOTION, &MainFrame::OnMouseMove, this, header_ID);
    headerPanel->Bind(wxEVT_LEFT_DCLICK, &MainFrame::OnTitleBarDoubleClick, this, header_ID);
    
}

void MainFrame::OnHeaderLeftDown(const wxMouseEvent& e)
{
    isDragging_ = true;
    dragStart_ = e.GetPosition();
}

void MainFrame::OnHeaderLeftUp(wxMouseEvent& e)
{
    isDragging_ = false;
}

void MainFrame::OnMouseMove(wxMouseEvent& e)
{
    if (isDragging_) {
        const wxPoint newPos = e.GetPosition() - dragStart_;
        SetPosition(GetPosition() + newPos);
    }
}

void MainFrame::OnTitleBarDoubleClick(wxMouseEvent& e)
{
    if (IsMaximized()) {
        Restore();
    } else {
        Maximize();
    }
}

