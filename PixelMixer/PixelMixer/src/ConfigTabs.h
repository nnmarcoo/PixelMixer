#pragma once
#include "Button.h"

enum Tabs_IDs {
    SourceButton_ID = 100,
    OperationButton_ID = 99,
    OutputButton_ID = 98,
    StatsButton_ID = 97
};

class ConfigTabs : public wxPanel{
public:
    ConfigTabs(wxWindow* parent);
private:
    wxDECLARE_EVENT_TABLE();
    void OnSourceButtonClick(wxCommandEvent& e);
    void OnOperationButtonClick(wxCommandEvent& e);
    void OnExportButtonClick(wxCommandEvent& e);
    void OnDebugButtonClick(wxCommandEvent& e);

    wxBoxSizer* tabsizer_;
    Button* sourcebutton_;
    Button* operationbutton_;
    Button* outputbutton_;
    Button* statsbutton_;
    int tabcycle_;
    int prevtab_;
    void SetTabColors(const int tab);
};
