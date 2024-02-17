#include "OutputPanel.h"
#include "Palette.h"

OutputPanel::OutputPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxWindowBase::SetBackgroundColour(Palette::config);

}
