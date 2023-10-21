#include "TextSlider.h"

#include "Palette.h"

wxBEGIN_EVENT_TABLE(TextSlider, wxStaticText)
    EVT_LEFT_DOWN(TextSlider::OnMouseLeftDown)

wxEND_EVENT_TABLE()

TextSlider::TextSlider(wxWindow* parent, wxWindowID id, const wxString& label, wxPoint pos, wxSize size, long style) : wxStaticText(parent, id, label, pos, size, style) {
    wxWindowBase::SetForegroundColour(Palette::bhover);
}

void TextSlider::OnMouseLeftDown(wxMouseEvent& e) {
    SetLabel("test");
}
