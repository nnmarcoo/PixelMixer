#include "HeaderPanel.h"
#include "MainFrame.h"

enum IDs {
    header_ID = 2,
    exitButton_ID = 3,
    maximizeButton_ID = 4,
    minimizeButton_ID = 5,
    iconButton_ID = 6,
    dropdown_ID = 7
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

//todo fix window drag bugs / errors
HeaderPanel::HeaderPanel(wxWindow* parent) : wxPanel(parent) {
    wxInitAllImageHandlers();
    wxWindowBase::SetBackgroundColour("#2c2f33");

    auto* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    
    exitbutton_ = new wxButton(this, exitButton_ID, wxString(wxT("\U0001F5D9")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER);
    maximizebutton_ = new wxButton(this, maximizeButton_ID, wxString(wxT("\U0001F5D6")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER);
    minimizebutton_ = new wxButton(this, minimizeButton_ID, wxString(wxT("\U0001F5D5")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER | wxBU_TOP);
    iconbutton_ = new wxButton(this, iconButton_ID, wxEmptyString, wxDefaultPosition, wxSize(20, 20));
          iconbutton_->SetBitmap(logoicon_);

    dropdownbutton_ = new wxButton(this, dropdown_ID, wxString(wxT("\U00002263")), wxDefaultPosition, wxSize(32, 30), wxNO_BORDER);
        dropdownbutton_->SetBitmap(dropdownicon_);

    headerSizer->Add(iconbutton_, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 5);
    headerSizer->Add(dropdownbutton_);
    

    wxButton* headerButtons[] = { dropdownbutton_, minimizebutton_ , maximizebutton_, exitbutton_ };
    
    headerSizer->AddStretchSpacer();
    for (wxButton* button : headerButtons) {
        button->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
        button->SetForegroundColour("#bfbfbf");
        button->SetBackgroundColour("#2c2f33");
        if (button->GetId() != dropdown_ID) //todo fix botch
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
    exitbutton_->Bind(wxEVT_ENTER_WINDOW, [=](wxMouseEvent& event) {
        ButtonHoverHandler(event, exitbutton_, "#ff3333"); // Hover color for exitButton
    });

    exitbutton_->Bind(wxEVT_LEAVE_WINDOW, [=](wxMouseEvent& event) {
        ButtonLeaveHandler(event, exitbutton_, "#2c2f33"); // Default color for exitButton
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
    isDragging_ = true;
    dragStart_ = e.GetPosition();
    CaptureMouse();
}

void HeaderPanel::OnHeaderLeftUp(wxMouseEvent& e) {
    isDragging_ = false;

    const wxPoint mousePos = wxGetMousePosition();
    const wxPoint rMousePos = mousePos - wxDisplay(wxDisplay::GetFromPoint(mousePos)).GetGeometry().GetPosition();

    if (rMousePos.y == 0 && !mainframe_->IsMaximized()) {
        mainframe_->SetPosition(mainframe_->GetPosition() + wxPoint(0, 50));
        ToggleMaximize();
    }
    if (HasCapture())
        ReleaseMouse();
}

void HeaderPanel::OnMouseMove(wxMouseEvent& e) {
    if (mainframe_->IsMaximized()) isDragging_ = false;
    if (isDragging_) {                                              //  && wxGetMouseState().LeftIsDown()
        const wxPoint newPos = e.GetPosition() - dragStart_;
        mainframe_->SetPosition(mainframe_->GetPosition() + newPos);
    }
}

void HeaderPanel::OnHeaderDoubleClick(wxMouseEvent& e) {
    ToggleMaximize();
}

void HeaderPanel::OnExitButtonClick(wxCommandEvent& e) {
    mainframe_->Destroy();
}

void HeaderPanel::OnMaximizeButtonClick(wxCommandEvent& e) {
    ToggleMaximize();
}

void HeaderPanel::OnMinimizeButtonClick(wxCommandEvent& e) {
    mainframe_->Iconize(true);
}

void HeaderPanel::OnIconButtonClick(wxCommandEvent& e) {
    auto* test = new wxFrame(this, wxID_ANY, "test", wxDefaultPosition, wxDefaultSize);
    test->Show();
    //wxLaunchDefaultBrowser("https://github.com/nnmarcoo");
}

void HeaderPanel::ToggleMaximize() const
{
    if (mainframe_->IsMaximized()) {
        mainframe_->Restore();
        maximizebutton_->SetLabel(wxString(wxT("\U0001F5D6")));
    }
    else { 
        mainframe_->Maximize();
        maximizebutton_->SetLabel(wxString(wxT("\U0001F5D7")));
    }
} 
