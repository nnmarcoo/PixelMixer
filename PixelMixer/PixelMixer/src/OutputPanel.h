#pragma once
#include "ViewportPanel.h"

class OutputPanel : public wxPanel {
public:
    OutputPanel(wxWindow* parent, ViewportPanel* viewport);
private:
    ViewportPanel* viewport_;
};
