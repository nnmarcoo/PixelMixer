#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/splitter.h>

//todo make the vertical splitter sash a custom color
//todo if mouse leaves window before mouse up, it won't change the bool
//todo optimize button hover binds

enum IDs {
    header_ID = 2,
    exitButton_ID = 3,
    maximizeButton_ID = 4,
    minimizeButton_ID = 5,
    iconButton_ID = 6
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_BUTTON(exitButton_ID, MainFrame::OnExitButtonClick)
    EVT_BUTTON(maximizeButton_ID, MainFrame::OnMaximizeButtonClick)
    EVT_BUTTON(minimizeButton_ID, MainFrame::OnMinimizeButtonClick)
    EVT_BUTTON(iconButton_ID, MainFrame::OnIconButtonClick)
wxEND_EVENT_TABLE();



MainFrame::MainFrame(const wxString& title): wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize) { // constructor
    // Layout setup
    
    // Store the mem address of the created wxStatusBar object
    wxStatusBar* statusBar = wxFrameBase::CreateStatusBar();

    // Configure the main window
    statusBar->SetBackgroundColour("#2c2f33");
    SetClientSize(wxGetDisplaySize() * 0.8);
    SetIcon(icon_);
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
    // Layout setup end ==

    

    //Bind header to event handlers so the window can be dragged and maximized by empty header space
    headerPanel->Bind(wxEVT_LEFT_DOWN, &MainFrame::OnHeaderLeftDown, this, header_ID);
    headerPanel->Bind(wxEVT_LEFT_UP, &MainFrame::OnHeaderLeftUp, this, header_ID);
    headerPanel->Bind(wxEVT_MOTION, &MainFrame::OnMouseMove, this, header_ID);
    headerPanel->Bind(wxEVT_LEFT_DCLICK, &MainFrame::OnTitleBarDoubleClick, this, header_ID);

    // GUI button controls
    //Header buttons
    auto* exitButton = new wxButton(headerPanel, exitButton_ID, "x", wxDefaultPosition, wxSize(50, 30), wxNO_BORDER);
    auto* maximizeButton = new wxButton(headerPanel, maximizeButton_ID, wxString(wxT("\U0001F5D6")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER);
    auto* minimizeButton = new wxButton(headerPanel, minimizeButton_ID, "_", wxDefaultPosition, wxSize(50, 30), wxNO_BORDER | wxBU_TOP);
    auto* iconButton = new wxButton(headerPanel, iconButton_ID, wxEmptyString, wxDefaultPosition, wxSize(20, 20));
    iconButton->SetBitmap(icon_);

    wxButton* headerButtons[] = {minimizeButton , maximizeButton, exitButton};

    auto* headerSizer = new wxBoxSizer(wxHORIZONTAL);

    headerSizer->Add(iconButton, 0, wxALIGN_CENTER | wxLEFT, 5);
    
    headerSizer->AddStretchSpacer();
    for (wxButton* button : headerButtons) {
        button->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        button->SetForegroundColour("#bfbfbf");
        button->SetBackgroundColour("#2c2f33");
        headerSizer->Add(button);
    }
    headerPanel->SetSizer(headerSizer);

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
    isDragging_ = true; //          todo WTF is the point of isDragging_ if I can just check the mouse state???
    dragStart_ = e.GetPosition();
}

void MainFrame::OnHeaderLeftUp(wxMouseEvent& e) {
    isDragging_ = false;
    SetStatusText(wxString::Format(wxT("%d"), e.GetPosition().y));
    if (wxGetMousePosition().y == 0) { // todo implement docking for side of screen
        Center();
        Maximize();
    }
}

void MainFrame::OnMouseMove(wxMouseEvent& e) {
    if (IsMaximized()) isDragging_ = false;
    if (isDragging_  && wxGetMouseState().LeftIsDown()) {
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

void MainFrame::OnMaximizeButtonClick(wxCommandEvent& e) {
    if (IsMaximized())
        Restore();
    else
        Maximize();
}

void MainFrame::OnMinimizeButtonClick(wxCommandEvent& e) {
    Iconize(true);
}

void MainFrame::OnIconButtonClick(wxCommandEvent& e) {
    wxLaunchDefaultBrowser("https://github.com/nnmarcoo");
}
