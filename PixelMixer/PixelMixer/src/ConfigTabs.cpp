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

    tabsizer_ = new wxBoxSizer(wxVERTICAL);

    sourcebutton_ = new Button(this, SourceButton_ID, wxString(wxT("\U0001F4C1")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43", "#282b30"); // brighter "#36393e"
    operationbutton_ = new Button(this, OperationButton_ID, wxString(wxT("\U00002699")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43");
    outputbutton_ = new Button(this, OutputButton_ID, wxString(wxT("\U0001F4C2")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43");
    statsbutton_ = new Button(this, StatsButton_ID, wxString(wxT("\U0001F41B")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43");
    tabsizer_->Add(sourcebutton_, 0, wxBOTTOM, 5);
    tabsizer_->Add(operationbutton_, 0, wxBOTTOM, 5);
    tabsizer_->Add(outputbutton_, 0, wxBOTTOM, 5);
    tabsizer_->Add(statsbutton_);
    
    SetSizerAndFit(tabsizer_);
}

void ConfigTabs::OnSourceButtonClick(wxCommandEvent& e) {
    SetTabColors(0);
}
void ConfigTabs::OnOperationButtonClick(wxCommandEvent& e) {
    SetTabColors(1);
}
void ConfigTabs::OnExportButtonClick(wxCommandEvent& e) {
    SetTabColors(2);
}
void ConfigTabs::OnDebugButtonClick(wxCommandEvent& e) {
    SetTabColors(3);
}

void ConfigTabs::SetTabColors(const int tab) {
    prevtab_ = tabcycle_; tabcycle_ = tab;
    
    Button* buttons[] = {sourcebutton_, operationbutton_, outputbutton_, statsbutton_};
    buttons[tabcycle_]->SetDefaultColor("#282b30");
    buttons[prevtab_]->SetDefaultColor("#2c2f33");
}

