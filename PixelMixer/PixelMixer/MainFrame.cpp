#include "MainFrame.h"
#include "HeaderPanel.h"

//todo make the vertical splitter sash a custom color
//todo if mouse leaves window before mouse up, it won't change the bool

// File -> Open, save, export, import, settings

enum IDs {
    header_ID = 2,
    exitButton_ID = 3,
    maximizeButton_ID = 4,
    minimizeButton_ID = 5,
    iconButton_ID = 6,
    fileImportButton_ID = 7
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
    statusBar->SetForegroundColour("#bfbfbf");
    statusBar->SetBackgroundColour("#2c2f33");
    statusText = new wxStaticText( statusBar, wxID_ANY,wxT("Version 1.1.2-alpha"), wxPoint(5, 5), wxDefaultSize, 0); // custom status bar color
    SetClientSize(wxGetDisplaySize() * 0.8);
    SetIcon(icon_);
    wxTopLevelWindowBase::SetMinSize(wxSize((GetSize().GetWidth() / 5) * 2, (GetSize().GetHeight() / 5) * 2)); // Min size is double the config column

    // Create the vertical and horizontal splitters to organize the frame
    auto* hSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);
    auto* vSplitter = new wxSplitterWindow(hSplitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_NOSASH);

    // Create the panels to color the split regions
    auto* configPanel = new wxPanel(vSplitter);
    auto* viewportPanel = new wxPanel(vSplitter);
    auto* headerPanel = new HeaderPanel(hSplitter, header_ID);

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
    // Layout setup end ==

    // GUI button controls
    //Header buttons
    auto* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    
    auto* exitButton = new wxButton(headerPanel, exitButton_ID, "x", wxDefaultPosition, wxSize(50, 30), wxNO_BORDER);
    auto* maximizeButton = new wxButton(headerPanel, maximizeButton_ID, wxString(wxT("\U0001F5D6")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER);
    auto* minimizeButton = new wxButton(headerPanel, minimizeButton_ID, "_", wxDefaultPosition, wxSize(50, 30), wxNO_BORDER | wxBU_TOP);
    auto* iconButton = new wxButton(headerPanel, iconButton_ID, wxEmptyString, wxDefaultPosition, wxSize(20, 20));
    iconButton->SetBitmap(icon_);

    wxButton* headerButtons[] = {minimizeButton , maximizeButton, exitButton};

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
    
    for (wxButton* button : headerButtons) { // Change the rest of the buttons
        if (button->GetId() == exitButton_ID) continue;
        
        button->Bind(wxEVT_ENTER_WINDOW, [=](wxMouseEvent& event) {
        ButtonHoverHandler(event, button, "#3c3f43");
        });

        button->Bind(wxEVT_LEAVE_WINDOW, [=](wxMouseEvent& event) {
            ButtonLeaveHandler(event, button, "#2c2f33");
        });
    }

    //Viewport
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
