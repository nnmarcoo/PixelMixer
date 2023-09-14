#pragma once
#include "wx/wx.h"

enum Tabs_IDs {
    SourceButton_ID = 100,
    OperationButton_ID = 99
};

class ConfigTabs : public wxPanel{
public:
    ConfigTabs(wxWindow* parent);
    
};
