#include "HeaderPanel.h"
#include "MainFrame.h"

enum IDs {
    header_ID = 2,
    exitButton_ID = 3,
    maximizeButton_ID = 4,
    minimizeButton_ID = 5,
    iconButton_ID = 6,
    fileImportButton_ID = 7
};

BEGIN_EVENT_TABLE(HeaderPanel, wxPanel)
    // Empty header space controls
    EVT_LEFT_DOWN(HeaderPanel::OnHeaderLeftDown)
    EVT_LEFT_UP(HeaderPanel::OnHeaderLeftUp)
    EVT_MOTION(HeaderPanel::OnMouseMove)
    EVT_LEFT_DCLICK(HeaderPanel::OnHeaderDoubleClick)

    // Button controls
    EVT_BUTTON(exitButton_ID, HeaderPanel::OnExitButtonClick)
    EVT_BUTTON(maximizeButton_ID, HeaderPanel::OnMaximizeButtonClick)
    EVT_BUTTON(minimizeButton_ID, HeaderPanel::OnMinimizeButtonClick)
    EVT_BUTTON(iconButton_ID, HeaderPanel::OnIconButtonClick)

    // Button hover events // todo put into table?
END_EVENT_TABLE()


HeaderPanel::HeaderPanel(wxWindow *parent) : wxPanel(parent)
{
    wxWindowBase::SetBackgroundColour("#2c2f33");

    auto* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    
    auto* exitButton = new wxButton(this, exitButton_ID, "x", wxDefaultPosition, wxSize(50, 30), wxNO_BORDER);
    auto* maximizeButton = new wxButton(this, maximizeButton_ID, wxString(wxT("\U0001F5D6")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER);
    auto* minimizeButton = new wxButton(this, minimizeButton_ID, "_", wxDefaultPosition, wxSize(50, 30), wxNO_BORDER | wxBU_TOP);
    auto* iconButton = new wxButton(this, iconButton_ID, wxEmptyString, wxDefaultPosition, wxSize(20, 20));
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
    SetSizer(headerSizer);
    
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
}

// must put 'mainFrame->' in front of all controls for the parent frame. Harder to read here but cleans up MainFrame.cpp
void HeaderPanel::OnHeaderLeftDown(wxMouseEvent& e) {
    isDragging_ = true; //          todo WTF is the point of isDragging_ if I can just check the mouse state???
    dragStart_ = e.GetPosition();
}

void HeaderPanel::OnHeaderLeftUp(wxMouseEvent& e) {
    isDragging_ = false;

    const wxPoint mousePos = wxGetMousePosition(); // these two lines get the mouse position on the monitor it is on
    const wxPoint rMousePos = mousePos - wxDisplay(wxDisplay::GetFromPoint(mousePos)).GetGeometry().GetPosition();

    if (rMousePos.y == 0) { // todo implement docking for side of screen
        mainFrame->SetPosition(mainFrame->GetPosition() + wxPoint(0, 50));
        mainFrame->Maximize();
    }
}

void HeaderPanel::OnMouseMove(wxMouseEvent& e) {
    if (mainFrame->IsMaximized()) isDragging_ = false;
    if (isDragging_  && wxGetMouseState().LeftIsDown()) {
        const wxPoint newPos = e.GetPosition() - dragStart_;
        mainFrame->SetPosition(mainFrame->GetPosition() + newPos);
    }
}

void HeaderPanel::OnHeaderDoubleClick(wxMouseEvent& e) {
    if (mainFrame->IsMaximized()) 
        mainFrame->Restore();
    else 
        mainFrame->Maximize();
}

void HeaderPanel::OnExitButtonClick(wxCommandEvent& e) {
    mainFrame->Destroy();
}

void HeaderPanel::OnMaximizeButtonClick(wxCommandEvent& e) {
    if (mainFrame->IsMaximized())
        mainFrame->Restore();
    else
        mainFrame->Maximize();
}

void HeaderPanel::OnMinimizeButtonClick(wxCommandEvent& e) {
    mainFrame->Iconize(true);
}

void HeaderPanel::OnIconButtonClick(wxCommandEvent& e) {
    wxLaunchDefaultBrowser("https://github.com/nnmarcoo");
}
