#include "InputPanel.h"
#include "Palette.h"

InputPanel::InputPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxWindowBase::SetBackgroundColour(Palette::config);

}
