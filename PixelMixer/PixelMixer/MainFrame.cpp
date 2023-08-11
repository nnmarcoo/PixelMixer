#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/sizer.h>

//todo make the vertical splitter sash a custom color
//todo if mouse leaves window before mouse up, it won't change the bool

enum IDs
{
    header_ID = 2,
    exitButton_ID = 3,
    maximizeButton_ID = 4,
    minimizeButton_ID = 5
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(exitButton_ID, MainFrame::OnExitButtonClick)
    EVT_BUTTON(maximizeButton_ID, MainFrame::OnMaximizeButtonClick)
    EVT_BUTTON(minimizeButton_ID, MainFrame::OnMinimizeButtonClick)
wxEND_EVENT_TABLE();



MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize) // constructor
{
    // Assuming you have a wxFrame named 'frame'
    
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
        hSplitter->SetSashPosition(30);
    
    SetWindowStyle(wxSYSTEM_MENU | wxRESIZE_BORDER| wxCLIP_CHILDREN); // Must be after status bar declaration
    CenterOnScreen();
    // Layout setup end

    //Bind header to event handlers so the window can be dragged and maximized
    headerPanel->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnHeaderLeftDown, this, header_ID);
    headerPanel->Bind(wxEVT_LEFT_UP, &MainFrame::OnHeaderLeftUp, this, header_ID);
    headerPanel->Bind(wxEVT_MOTION, &MainFrame::OnMouseMove, this, header_ID);
    headerPanel->Bind(wxEVT_LEFT_DCLICK, &MainFrame::OnTitleBarDoubleClick, this, header_ID);

    // GUI button controls
    
    // Exit button 
    auto* exitButton = new wxButton(headerPanel, exitButton_ID, "X", wxDefaultPosition, wxSize(50, 30), wxNO_BORDER);
    exitButton->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    exitButton->SetForegroundColour("#dbdbdb");
    exitButton->SetBackgroundColour("#2c2f33");
    

    // Maximize button
    auto* maximizeButton = new wxButton(headerPanel, maximizeButton_ID, wxString(wxT("\U0001F5D6")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER);
    maximizeButton->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    maximizeButton->SetForegroundColour("#dbdbdb");
    maximizeButton->SetBackgroundColour("#2c2f33");

    // Minimize button
    auto* minimizeButton = new wxButton(headerPanel, minimizeButton_ID, "_", wxDefaultPosition, wxSize(50, 30), wxNO_BORDER | wxBU_TOP);
    minimizeButton->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    minimizeButton->SetForegroundColour("#dbdbdb");
    minimizeButton->SetBackgroundColour("#2c2f33");
    

    // Bind button locations TODO REMOVE THESE BINDS AND REPLACE WITH BOXSIZER OR SOMETHING
    headerPanel->Bind(wxEVT_SIZE, [exitButton](wxSizeEvent& event) {
    wxSize newSize = event.GetSize();
    int buttonX = newSize.GetWidth() - 50;
    exitButton->Move(wxPoint(buttonX, 0));
    event.Skip();
    });

    headerPanel->Bind(wxEVT_SIZE, [maximizeButton](wxSizeEvent& event) {
    wxSize newSize = event.GetSize();
    int buttonX = newSize.GetWidth() - 100;
    maximizeButton->Move(wxPoint(buttonX, 0));
    event.Skip();
    });

    headerPanel->Bind(wxEVT_SIZE, [minimizeButton](wxSizeEvent& event) {
    wxSize newSize = event.GetSize();
    int buttonX = newSize.GetWidth() - 150;
    minimizeButton->Move(wxPoint(buttonX, 0));
    event.Skip();
    });

    // Bind button hover event

    // Define a common event handler function for hover and leave events
    auto ButtonHoverHandler = [](wxMouseEvent& event, wxButton* button, const wxString& hoverColor) {
        button->SetBackgroundColour(hoverColor);  // Change the background color on hover
        button->Refresh();
        event.Skip();
    };

    auto ButtonLeaveHandler = [](wxMouseEvent& event, wxButton* button, const wxString& defaultColor) {
        button->SetBackgroundColour(defaultColor);  // Change the background color back when leaving hover
        button->Refresh();
        event.Skip();
    };
    
    // Bind the common event handlers to each button
    exitButton->Bind(wxEVT_ENTER_WINDOW, [=](wxMouseEvent& event) {
        ButtonHoverHandler(event, exitButton, "#ff3333"); // Hover color for exitButton
    });

    exitButton->Bind(wxEVT_LEAVE_WINDOW, [=](wxMouseEvent& event) {
        ButtonLeaveHandler(event, exitButton, "#2c2f33"); // Default color for exitButton
    });

    maximizeButton->Bind(wxEVT_ENTER_WINDOW, [=](wxMouseEvent& event) {
        ButtonHoverHandler(event, maximizeButton, "#3c3f43"); // Hover color for maximizeButton
    });

    maximizeButton->Bind(wxEVT_LEAVE_WINDOW, [=](wxMouseEvent& event) {
        ButtonLeaveHandler(event, maximizeButton, "#2c2f33"); // Default color for maximizeButton
    });

    minimizeButton->Bind(wxEVT_ENTER_WINDOW, [=](wxMouseEvent& event) {
        ButtonHoverHandler(event, minimizeButton, "#3c3f43"); // Hover color for minimizeButton
    });

    minimizeButton->Bind(wxEVT_LEAVE_WINDOW, [=](wxMouseEvent& event) {
        ButtonLeaveHandler(event, minimizeButton, "#2c2f33"); // Default color for minimizeButton
    });
}

void MainFrame::OnHeaderLeftDown(const wxMouseEvent& e) {
    isDragging_ = true;
    dragStart_ = e.GetPosition();
}

void MainFrame::OnHeaderLeftUp(wxMouseEvent& e) {
    isDragging_ = false;
}

void MainFrame::OnMouseMove(wxMouseEvent& e) {
    if (IsMaximized()) isDragging_ = false;
    if (isDragging_)
    {
        const wxPoint newPos = e.GetPosition() - dragStart_;
        SetPosition(GetPosition() + newPos);
    }
}

void MainFrame::OnTitleBarDoubleClick(wxMouseEvent& e) {
    if (IsMaximized()) {
        Restore();
    }
    else {
        Maximize();
    }
}

void MainFrame::OnExitButtonClick(wxCommandEvent& e) {
    Destroy();
}

void MainFrame::OnMaximizeButtonClick(wxCommandEvent& e)
{
    if (IsMaximized())
        Restore();
    else
        Maximize();
}

void MainFrame::OnMinimizeButtonClick(wxCommandEvent& e)
{
    Iconize(true);
}


