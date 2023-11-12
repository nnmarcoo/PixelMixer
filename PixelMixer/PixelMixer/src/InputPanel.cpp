#include "InputPanel.h"
#include "Palette.h"
#include "TextSlider.h"

InputPanel::InputPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxWindowBase::SetBackgroundColour(Palette::config);

    scalelabel_ = new wxStaticText(this, wxID_ANY, "Scale", wxPoint(30, 30));
    scaleslider_ = new TextSlider(this, wxID_ANY, "50.00", 0 , 100, wxPoint(150, 30), "%");

    scalelabel_->SetForegroundColour(Palette::text);
    scalelabel_->SetFont(Font::click);

}
