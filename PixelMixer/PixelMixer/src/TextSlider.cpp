#include "TextSlider.h"

#include "Palette.h"

wxBEGIN_EVENT_TABLE(TextSlider, wxStaticText)
    EVT_LEFT_DOWN(TextSlider::OnMouseLeftDown)

wxEND_EVENT_TABLE()

TextSlider::TextSlider(wxWindow* parent, wxWindowID id, const wxString& defaultval, int minval, int maxval) :
    wxStaticText(parent, id, defaultval), val_(std::stoi(static_cast<std::string>(defaultval))), min_(minval),
    max_(maxval)
{
    wxWindowBase::SetForegroundColour(Palette::clickable);
}

void TextSlider::OnMouseLeftDown(wxMouseEvent& e) {
    SetLabel(std::to_string(val_));
}
