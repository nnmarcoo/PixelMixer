#include "HeaderPanel.h"
#include "MainFrame.h"

//todo make custom button class

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
    EVT_LEFT_DOWN(HeaderPanel::OnLeftDown)
    EVT_LEFT_UP(HeaderPanel::OnLeftUp)
    EVT_MOTION(HeaderPanel::OnMouseMove)
    EVT_LEFT_DCLICK(HeaderPanel::OnDoubleClick)

    // Button controls
    EVT_BUTTON(exitButton_ID, HeaderPanel::OnExitButtonClick)
    EVT_BUTTON(maximizeButton_ID, HeaderPanel::OnMaximizeButtonClick)
    EVT_BUTTON(minimizeButton_ID, HeaderPanel::OnMinimizeButtonClick)
    EVT_BUTTON(iconButton_ID, HeaderPanel::OnIconButtonClick)
    EVT_BUTTON(dropdown_ID, HeaderPanel::OnDropDownButtonClick)

    // Button hover events // todo put into table?
END_EVENT_TABLE()

HeaderPanel::HeaderPanel(wxWindow* parent) : wxPanel(parent) {
    wxInitAllImageHandlers();
    wxWindowBase::SetBackgroundColour("#2c2f33");

    auto* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    
    exitbutton_ = new Button(this, exitButton_ID, wxString(wxT("\U0001F5D9")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER, wxNullBitmap, "#ff3333");
    maximizebutton_ = new Button(this, maximizeButton_ID, wxString(wxT("\U0001F5D6")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER);
    minimizebutton_ = new Button(this, minimizeButton_ID, wxString(wxT("\U0001F5D5")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER | wxBU_TOP);
    iconbutton_ = new Button(this, iconButton_ID, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxNO_BORDER, logoicon_);
    dropdownbutton_ = new Button(this, dropdown_ID, wxString(wxT("\U00002263")), wxDefaultPosition, wxSize(32, 30), wxNO_BORDER, dropdownicon_);

    headerSizer->Add(iconbutton_, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 5);
    headerSizer->Add(dropdownbutton_);

    headerSizer->AddStretchSpacer();

    headerSizer->Add(minimizebutton_);
    headerSizer->Add(maximizebutton_);
    headerSizer->Add(exitbutton_);
    
    SetSizer(headerSizer);
}

// must put 'mainFrame->' in front of all controls for the parent frame. Harder to read here but cleans up MainFrame.cpp
void HeaderPanel::OnLeftDown(wxMouseEvent& e) {
    isDragging_ = true;
    dragStart_ = e.GetPosition();
    CaptureMouse();
}

void HeaderPanel::OnLeftUp(wxMouseEvent& e) { // todo add docking ?
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
    if (!isDragging_) return;
    const wxPoint newPos = e.GetPosition() - dragStart_;
    mainframe_->SetPosition(mainframe_->GetPosition() + newPos);
}

void HeaderPanel::OnDoubleClick(wxMouseEvent& e) {
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
    wxLaunchDefaultBrowser("https://github.com/nnmarcoo");
}

void HeaderPanel::OnDropDownButtonClick(wxCommandEvent& e) { // fit to media, import, 
    auto* test = new wxFrame(this, wxID_ANY, wxEmptyString, mainframe_->GetPosition() + wxPoint(36, 30), wxSize(230, 200), wxFRAME_NO_TASKBAR | wxFRAME_SHAPED);
    test->SetBackgroundColour("#2f3238");
    test->Show();
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
