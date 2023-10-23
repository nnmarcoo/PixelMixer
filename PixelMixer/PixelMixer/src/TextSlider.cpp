#include "TextSlider.h"
#include "Palette.h"

//todo: scroll wheel scrolls value

wxBEGIN_EVENT_TABLE(TextSlider, wxTextCtrl)
    EVT_LEFT_DOWN(TextSlider::OnMouseLeftDown)
    EVT_LEFT_UP(TextSlider::OnMouseLeftUp)
    EVT_MOTION(TextSlider::OnMouseMove)
    EVT_ENTER_WINDOW(TextSlider::OnMouseEnter)
wxEND_EVENT_TABLE()

TextSlider::TextSlider(wxWindow* parent, wxWindowID id, const wxString& defaultval, float minval, float maxval, const wxPoint& pos) : wxTextCtrl(parent, id, defaultval, pos, wxDefaultSize,  wxNO_BORDER), val_(std::stof(static_cast<std::string>(defaultval))), min_(minval), max_(maxval) {
    wxTextCtrl::SetForegroundColour(Palette::clickable);
    wxTextCtrl::SetBackgroundColour(Palette::config);
    wxTextCtrl::SetFont(wxFont(13, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
}

void TextSlider::OnMouseLeftDown(wxMouseEvent& e) {
    const wxPoint mousepos = e.GetPosition();
    prevpos_ = mousepos;
    clickpos_ = mousepos;
    CaptureMouse();
}

void TextSlider::OnMouseLeftUp(wxMouseEvent& e) {
    if (clickpos_ == prevpos_) { // todo change text to real value
        SetFocus(); 
        SetSelection(0, -1);
    }
    
     if (HasCapture()) ReleaseMouse();
}

void TextSlider::OnMouseMove(wxMouseEvent& e) {
    if (!HasCapture()) return;

    const wxPoint currpos = e.GetPosition();
    const int deltaX = currpos.x - prevpos_.x;

    val_ += deltaX > 0 ? 1.0f : deltaX < 0 ? -1.0f : 0.0f;
    val_ = val_ > max_ ? max_ : val_;   // val_ = std::min(max_, std::max(min_, val_ + (deltaX > 0 ? 1 : -1)));
    val_ = val_ < min_ ? min_ : val_;
    
    prevpos_ = currpos;
    
    const std::string label = std::to_string(val_);
    SetValue(label.substr(0,label.find('.')+3));
}

void TextSlider::OnMouseEnter(wxMouseEvent& e) {
    SetCursor(wxCursor(wxCURSOR_SIZEWE));
}

float TextSlider::getvalue() const {
    return val_;
}
