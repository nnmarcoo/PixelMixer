#include "ConfigTabs.h"
#include "Palette.h"

wxBEGIN_EVENT_TABLE(ConfigTabs, wxPanel)
    EVT_BUTTON(SourceButton_ID, ConfigTabs::OnSourceButtonClick)
    EVT_BUTTON(OperationButton_ID, ConfigTabs::OnOperationButtonClick)
    EVT_BUTTON(OutputButton_ID, ConfigTabs::OnExportButtonClick)
    EVT_BUTTON(StatsButton_ID, ConfigTabs::OnDebugButtonClick)
wxEND_EVENT_TABLE()

ConfigTabs::ConfigTabs(wxWindow* parent) : wxPanel(parent) {
    wxWindowBase::SetBackgroundColour(Palette::border);
    wxWindowBase::SetMinSize(wxSize(30, -1)); // Is this necessary?
    tabcycle_ = 1;
    tabsizer_ = new wxBoxSizer(wxVERTICAL);

    sourcebutton_ = new Button(this, SourceButton_ID, wxString(wxT("\U0001F4C1")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxIcon("res/images/source.ico", wxBITMAP_TYPE_ICO));
    operationbutton_ = new Button(this, OperationButton_ID, wxString(wxT("\U00002699")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, Palette::ghover, Palette::config, wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
    outputbutton_ = new Button(this, OutputButton_ID, wxString(wxT("\U0001F4C2")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxIcon("res/images/output.ico", wxBITMAP_TYPE_ICO));
    statsbutton_ = new Button(this, StatsButton_ID, wxString(wxT("\U0001F41B")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxIcon("res/images/stats.ico", wxBITMAP_TYPE_ICO));
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

void ConfigTabs::SetTabColors(const short tab) {
    prevtab_ = tabcycle_; tabcycle_ = tab;
    if (prevtab_ == tabcycle_) return;
    configpanel_->SetTabs(tabcycle_, prevtab_);
    
    Button* buttons[] = {sourcebutton_, operationbutton_, outputbutton_, statsbutton_};
    buttons[tabcycle_]->SetDefaultColor(Palette::config);
    buttons[prevtab_]->SetDefaultColor(Palette::border);
}

