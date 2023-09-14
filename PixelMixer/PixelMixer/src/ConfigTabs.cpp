#include "ConfigTabs.h"
#include "Button.h"

ConfigTabs::ConfigTabs(wxWindow* parent) : wxPanel(parent) {
    wxWindowBase::SetBackgroundColour("#2c2f33");
    wxWindowBase::SetMinSize(wxSize(30, -1)); // Is this necessary?

    wxBoxSizer* tabsizer = new wxBoxSizer(wxVERTICAL);

    Button* sourcebutton = new Button(this, SourceButton_ID, wxString(wxT("\U0001F5D9")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43", "#282b30"); // brighter "#36393e"
    Button* operationbutton = new Button(this, OperationButton_ID, wxString(wxT("\U0001F5D9")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43");
    Button* outputbutton = new Button(this, wxID_ANY, wxString(wxT("\U0001F5D9")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43");
    Button* statsbutton = new Button(this, wxID_ANY, wxString(wxT("\U0001F5D9")), wxDefaultPosition, wxSize(30, 30), wxNO_BORDER, wxNullBitmap, "#3c3f43");
    tabsizer->Add(sourcebutton, 0, wxBOTTOM, 5);
    tabsizer->Add(operationbutton, 0, wxBOTTOM, 5);
    tabsizer->Add(outputbutton, 0, wxBOTTOM, 5);
    tabsizer->Add(statsbutton);
    
    SetSizerAndFit(tabsizer);
}
