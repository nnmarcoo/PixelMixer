#include "HeaderPanel.h"

#include <thread>

#include "DropdownLineBreak.h"
#include "MainFrame.h"

BEGIN_EVENT_TABLE(HeaderPanel, wxPanel)
    // Empty header space controls
    EVT_LEFT_DOWN(HeaderPanel::OnLeftDown)
    EVT_LEFT_UP(HeaderPanel::OnLeftUp)
    EVT_MOTION(HeaderPanel::OnMouseMove)
    EVT_LEFT_DCLICK(HeaderPanel::OnDoubleClick)

    // Header button controls
    EVT_BUTTON(exitButton_ID, HeaderPanel::OnExitButtonClick)
    EVT_BUTTON(maximizeButton_ID, HeaderPanel::OnMaximizeButtonClick)
    EVT_BUTTON(minimizeButton_ID, HeaderPanel::OnMinimizeButtonClick)
    EVT_BUTTON(iconButton_ID, HeaderPanel::OnIconButtonClick)
    EVT_BUTTON(dropdownButton_ID, HeaderPanel::OnDropDownButtonClick)

    // Dropdown button controls
    EVT_BUTTON(centermediaButton_ID, HeaderPanel::OnCenterMediaClick)
    EVT_BUTTON(importmediaButton_ID, HeaderPanel::OnImportMediaClick)
    EVT_BUTTON(exitdropdownButton_ID, HeaderPanel::OnExitButtonClick)
    EVT_BUTTON(settingsButton_ID, HeaderPanel::OnSettingsClick)
    EVT_BUTTON(helpButton_ID, HeaderPanel::OnHelpClick)
END_EVENT_TABLE()

HeaderPanel::HeaderPanel(wxWindow* parent) : wxPanel(parent) {
    wxInitAllImageHandlers();
    wxWindowBase::SetBackgroundColour("#2c2f33");

    headerSizer_ = new wxBoxSizer(wxHORIZONTAL);
    
    exitbutton_ = new Button(this, exitButton_ID, wxString(wxT("\U0001F5D9")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER, wxNullBitmap, "#ff3333");
    maximizebutton_ = new Button(this, maximizeButton_ID, wxString(wxT("\U0001F5D6")), wxDefaultPosition, wxSize(50, 30));
    minimizebutton_ = new Button(this, minimizeButton_ID, wxString(wxT("\U0001F5D5")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER | wxBU_TOP);
    iconbutton_ = new Button(this, iconButton_ID, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxNO_BORDER, logoicon_);
    dropdownbutton_ = new Button(this, dropdownButton_ID, wxString(wxT("\U00002263")), wxDefaultPosition, wxSize(32, 30), wxNO_BORDER, dropdownicon_);

    headerSizer_->Add(iconbutton_, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 5);
    headerSizer_->Add(dropdownbutton_);
    headerSizer_->AddStretchSpacer();
    headerSizer_->Add(minimizebutton_);
    headerSizer_->Add(maximizebutton_);
    headerSizer_->Add(exitbutton_);
    
    SetSizer(headerSizer_);

    CreateDropDown();
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

void HeaderPanel::OnExitButtonClick(wxCommandEvent& e) { // todo ask to save
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

void HeaderPanel::OnDropDownButtonClick(wxCommandEvent& e) {
    dropdown_->SetPosition(mainframe_->GetPosition() + wxPoint(36, 30));
    dropdown_->Show();
    dropdown_->Raise();
    
    while (dropdown_->IsActive())
        wxYield();
    dropdown_->Hide();
}

void HeaderPanel::CreateDropDown() {
    const wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN);
    dropdown_ = new wxFrame(this, dropdownFrame_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxFRAME_NO_TASKBAR | wxFRAME_SHAPED | wxBORDER_SIMPLE);
    dropdown_->SetBackgroundColour("#2f3238");
    
    // 1F5AB save icon
    auto* importmedia = new Button(dropdown_, importmediaButton_ID, "Import                                      ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, "#2e436e", "#2f3238", font);
    auto* exportmedia = new Button(dropdown_, wxID_ANY, "Export                                      ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, "#2e436e", "#2f3238", font);
    auto* centermedia = new Button(dropdown_, centermediaButton_ID, "Center Media                           ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, "#2e436e", "#2f3238", font);
    auto* settings = new Button(dropdown_, settingsButton_ID, "Settings                                   ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, "#2e436e", "#2f3238", font);
    auto* help = new Button(dropdown_, helpButton_ID, "Help                                        ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, "#2e436e", "#2f3238", font);
    auto* exit = new Button(dropdown_, exitdropdownButton_ID, "Exit                                         ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, "#2e436e", "#2f3238", font);
    
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(importmedia, 0, wxALL, 5);
    sizer->Add(exportmedia, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(new DropdownLineBreak(dropdown_), 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(centermedia, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(new DropdownLineBreak(dropdown_), 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(settings, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(help, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(new DropdownLineBreak(dropdown_), 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(exit, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    
    dropdown_->SetSizerAndFit(sizer);
}

void HeaderPanel::OnCenterMediaClick(wxCommandEvent& e) { // todo implement
    viewport_->CenterMedia();
    mainframe_->Raise();
}

void HeaderPanel::OnImportMediaClick(wxCommandEvent& e) {
    wxFileDialog importmedia(this, "Import Media", "", "", "Image files (*.png;*.jpg;*.bmp)|*.png;*.jpg;*.bmp|All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (importmedia.ShowModal() == wxID_CANCEL) return;
    viewport_->SetMedia(static_cast<std::string>(importmedia.GetPath()));
}

void HeaderPanel::OnSettingsClick(wxCommandEvent& e) {
}

void HeaderPanel::OnHelpClick(wxCommandEvent& e) {
}
