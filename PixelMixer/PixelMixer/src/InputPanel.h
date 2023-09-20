#pragma once
#include "ViewportPanel.h"

class InputPanel : public wxPanel {
public:
    InputPanel(wxWindow* parent, ViewportPanel* viewport);
private:
    ViewportPanel* viewport_;
};
