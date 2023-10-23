#include "TextSlider.h"
#include "Palette.h"

wxBEGIN_EVENT_TABLE(TextSlider, wxStaticText)
    EVT_LEFT_DOWN(TextSlider::OnMouseLeftDown)
    EVT_LEFT_UP(TextSlider::OnMouseLeftUp)
    EVT_MOTION(TextSlider::OnMouseMove)
    EVT_ENTER_WINDOW(TextSlider::OnMouseEnter)
wxEND_EVENT_TABLE()

TextSlider::TextSlider(wxWindow* parent, wxWindowID id, const wxString& defaultval, int minval, int maxval) : wxStaticText(parent, id, defaultval), val_(std::stoi(static_cast<std::string>(defaultval))), min_(minval), max_(maxval) {
    wxWindowBase::SetForegroundColour(Palette::clickable);
    wxStaticText::SetFont(wxFont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
}

void TextSlider::OnMouseLeftDown(wxMouseEvent& e) {
    prevpos_ = e.GetPosition();
    CaptureMouse();
    
}

void TextSlider::OnMouseLeftUp(wxMouseEvent& e) {
    if (HasCapture()) ReleaseMouse();
    SetCursor(wxCursor(wxCURSOR_DEFAULT));
}

void TextSlider::OnMouseMove(wxMouseEvent& e) { // todo a bug where if the mouse moves down, the value goes down too??
    if (!HasCapture()) return;
    
    wxPoint currpos = e.GetPosition();
    int deltaX = currpos.x - prevpos_.x;

    val_ += deltaX > 0 ? 1 : -1;
    val_ = val_ > max_ ? max_ : val_;   // val_ = std::min(max_, std::max(min_, val_ + (deltaX > 0 ? 1 : -1)));
    val_ = val_ < min_ ? min_ : val_;
    
    prevpos_ = currpos;
    SetLabel(std::to_string(val_));
}

void TextSlider::OnMouseEnter(wxMouseEvent& e) {
    SetCursor(wxCursor(wxCURSOR_SIZEWE));
}
