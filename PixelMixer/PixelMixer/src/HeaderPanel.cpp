#include "HeaderPanel.h"
#include "Palette.h"

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
    EVT_BUTTON(exportmediaButton_ID, HeaderPanel::OnExportMediaClick)
    EVT_BUTTON(screenshotButton_ID, HeaderPanel::OnScreenshotClick)
END_EVENT_TABLE()

HeaderPanel::HeaderPanel(wxWindow* parent) : wxPanel(parent) {
    wxInitAllImageHandlers();
    wxWindowBase::SetBackgroundColour(Palette::border);

    wxSizer* headersizer = new wxBoxSizer(wxHORIZONTAL);
    
    exitbutton_ = new Button(this, exitButton_ID, wxString(wxT("\U0001F5D9")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER, wxNullBitmap, Palette::red);
    maximizebutton_ = new Button(this, maximizeButton_ID, wxString(wxT("\U0001F5D6")), wxDefaultPosition, wxSize(50, 30));
    minimizebutton_ = new Button(this, minimizeButton_ID, wxString(wxT("\U0001F5D5")), wxDefaultPosition, wxSize(50, 30), wxNO_BORDER | wxBU_TOP);
    iconbutton_ = new Button(this, iconButton_ID, wxEmptyString, wxDefaultPosition, wxSize(20, 20), wxNO_BORDER, logoicon_);
    dropdownbutton_ = new Button(this, dropdownButton_ID, wxEmptyString, wxDefaultPosition, wxSize(32, 30), wxNO_BORDER, dropdownicon_); // U00002263

    headersizer->Add(iconbutton_, 0, wxALIGN_CENTER | wxLEFT | wxRIGHT, 5);
    headersizer->Add(dropdownbutton_);
    headersizer->AddStretchSpacer();
    headersizer->Add(minimizebutton_);
    headersizer->Add(maximizebutton_);
    headersizer->Add(exitbutton_);
    
    SetSizer(headersizer);

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
    if (dropdown_->IsShown()) dropdown_->Hide();
    mainframe_->Hide();
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
    dropdownbutton_->SetDefaultColor(Palette::ghover);
    dropdown_->SetPosition(mainframe_->GetPosition() + wxPoint(36, 30));
    dropdown_->Show();
    dropdown_->Raise();
    
    while (dropdown_->IsActive())
        wxYield();
    dropdown_->Hide();
    dropdownbutton_->SetDefaultColor(Palette::border);
}

void HeaderPanel::CreateDropDown() {
    const wxFont font(11, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_THIN);
    dropdown_ = new wxFrame(this, dropdownFrame_ID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxFRAME_NO_TASKBAR | wxFRAME_SHAPED | wxBORDER_SIMPLE);
    dropdown_->SetBackgroundColour(Palette::viewport);
    
    // 1F5AB save icon
    auto* importmedia = new Button(dropdown_, importmediaButton_ID, "Import                                      ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, Palette::bhover, Palette::viewport, font); // this is so jank
    auto* exportmedia = new Button(dropdown_, exportmediaButton_ID, "Export                                      ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, Palette::bhover, Palette::viewport, font);
    auto* centermedia = new Button(dropdown_, centermediaButton_ID, "Center Media                           ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, Palette::bhover, Palette::viewport, font);
    auto* screenshot = new Button(dropdown_, screenshotButton_ID, "Screenshot                             ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, Palette::bhover, Palette::viewport, font);
    auto* settings = new Button(dropdown_, settingsButton_ID, "Settings                                   ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, Palette::bhover, Palette::viewport, font);
    auto* help = new Button(dropdown_, helpButton_ID, "Help                                        ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, Palette::bhover, Palette::viewport, font);
    auto* exit = new Button(dropdown_, exitdropdownButton_ID, "Exit                                         ", wxDefaultPosition, wxSize(230, 30), wxBORDER_NONE | wxALIGN_LEFT, wxNullBitmap, Palette::bhover, Palette::viewport, font);
    
    wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(importmedia, 0, wxALL, 5);
    sizer->Add(exportmedia, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(new DropdownLineBreak(dropdown_), 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(centermedia, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(screenshot, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);
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

void HeaderPanel::OnExportMediaClick(wxCommandEvent& e) {
    wxFileDialog exportmedia(this, "Export Media", "", "", "Image files (*.png;*.jpg;*.bmp)|*.png;*.jpg;*.bmp|All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (exportmedia.ShowModal() == wxID_CANCEL) return;
    viewport_->ExportMedia(static_cast<std::string>(exportmedia.GetPath()));
}

void HeaderPanel::OnScreenshotClick(wxCommandEvent& e) {
    wxFileDialog screenshot(this, "Save Screenshot", "", "ss", "Image files (*.png)|*.png|All files (*.*)|*.*", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (screenshot.ShowModal() == wxID_CANCEL) return;
    viewport_->Screenshot(static_cast<std::string>(screenshot.GetPath()));
}

void HeaderPanel::OnSettingsClick(wxCommandEvent& e) {
}

void HeaderPanel::OnHelpClick(wxCommandEvent& e) {
}
