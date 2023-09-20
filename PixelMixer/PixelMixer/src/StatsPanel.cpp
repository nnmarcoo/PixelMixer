#include "StatsPanel.h"
#include "Palette.h"

StatsPanel::StatsPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxWindowBase::SetBackgroundColour(Palette::config);

}
