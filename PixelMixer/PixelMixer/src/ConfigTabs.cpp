#include "ConfigTabs.h"


wxBEGIN_EVENT_TABLE(ConfigTabs, wxPanel)
    EVT_BUTTON(SourceButton_ID, ConfigTabs::OnSourceButtonClick)
    EVT_BUTTON(OperationButton_ID, ConfigTabs::OnOperationButtonClick)
    EVT_BUTTON(OutputButton_ID, ConfigTabs::OnExportButtonClick)
    EVT_BUTTON(StatsButton_ID, ConfigTabs::OnDebugButtonClick)
wxEND_EVENT_TABLE()

ConfigTabs::ConfigTabs(wxWindow* parent) : wxPanel(parent) {
    wxWindowBase::SetBackgroundColour("#2c2f33");
    wxWindowBase::SetMinSize(wxSize(30, -1)); // Is this necessary?

    wxBoxSizer* tabsizer = new wxBoxSizer(wxVERTICAL);

    sourcebutton_ = new Button(this, SourceButton_ID, wxString(wxT("\U0001F4C1")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43", "#282b30"); // brighter "#36393e"
    operationbutton_ = new Button(this, OperationButton_ID, wxString(wxT("\U00002699")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43");
    outputbutton_ = new Button(this, OutputButton_ID, wxString(wxT("\U0001F4C2")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43");
    statsbutton_ = new Button(this, StatsButton_ID, wxString(wxT("\U0001F41B")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43");
    tabsizer->Add(sourcebutton_, 0, wxBOTTOM, 5);
    tabsizer->Add(operationbutton_, 0, wxBOTTOM, 5);
    tabsizer->Add(outputbutton_, 0, wxBOTTOM, 5);
    tabsizer->Add(statsbutton_);
    
    SetSizerAndFit(tabsizer);
}

void ConfigTabs::OnSourceButtonClick(wxCommandEvent& e) {
    sourcebutton_->SetDefaultColor("#282b30");
    tabcycle_ = 0; SetTabColors();
}
void ConfigTabs::OnOperationButtonClick(wxCommandEvent& e) {
    operationbutton_->SetDefaultColor("#282b30");
    tabcycle_ = 1; SetTabColors();
}
void ConfigTabs::OnExportButtonClick(wxCommandEvent& e) {
    outputbutton_->SetDefaultColor("#282b30");
    tabcycle_ = 2; SetTabColors();
}
void ConfigTabs::OnDebugButtonClick(wxCommandEvent& e) {
    statsbutton_->SetDefaultColor("#282b30");
    tabcycle_ = 3; SetTabColors();
}

void ConfigTabs::SetTabColors() const {
    switch(tabcycle_) {
    case 0:
        operationbutton_->SetDefaultColor("#2c2f33");
        outputbutton_->SetDefaultColor("#2c2f33");
        statsbutton_->SetDefaultColor("#2c2f33");
        break;
    case 1:
        sourcebutton_->SetDefaultColor("#2c2f33");
        outputbutton_->SetDefaultColor("#2c2f33");
        statsbutton_->SetDefaultColor("#2c2f33");
        break;
    case 2:
        sourcebutton_->SetDefaultColor("#2c2f33");
        operationbutton_->SetDefaultColor("#2c2f33");
        statsbutton_->SetDefaultColor("#2c2f33");
        break;
    case 3:
        sourcebutton_->SetDefaultColor("#2c2f33");
        operationbutton_->SetDefaultColor("#2c2f33");
        outputbutton_->SetDefaultColor("#2c2f33");
        break;
    default:;}
}
