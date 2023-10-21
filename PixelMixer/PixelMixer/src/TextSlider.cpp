#include "TextSlider.h"

#include "Palette.h"

wxBEGIN_EVENT_TABLE(TextSlider, wxStaticText)
    EVT_LEFT_DOWN(TextSlider::OnMouseLeftDown)
    EVT_LEFT_UP(TextSlider::OnMouseLeftUp)
    EVT_MOTION(TextSlider::OnMouseMove)
wxEND_EVENT_TABLE()

TextSlider::TextSlider(wxWindow* parent, wxWindowID id, const wxString& defaultval, int minval, int maxval) : wxStaticText(parent, id, defaultval), val_(std::stoi(static_cast<std::string>(defaultval))), min_(minval), max_(maxval) {
    wxWindowBase::SetForegroundColour(Palette::clickable);
}

void TextSlider::OnMouseLeftDown(wxMouseEvent& e) {
    prevpos_ = e.GetPosition();
    CaptureMouse();
}

void TextSlider::OnMouseLeftUp(wxMouseEvent& e) {
    if (HasCapture()) ReleaseMouse();
}

void TextSlider::OnMouseMove(wxMouseEvent& e) {
    if (!HasCapture()) return;
    
    wxPoint currpos = e.GetPosition();
    int deltaX = currpos.x - prevpos_.x;

    val_ += deltaX > 0 ? 1 : -1;
    val_ = val_ > max_ ? max_ : val_;   // val_ = std::min(max_, std::max(min_, val_ + (deltaX > 0 ? 1 : -1)));
    val_ = val_ < min_ ? min_ : val_;
    
    prevpos_ = currpos;
    SetLabel(std::to_string(val_));
}
