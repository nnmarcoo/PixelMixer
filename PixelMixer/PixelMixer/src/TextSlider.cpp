#include "TextSlider.h"
#include "Palette.h"

//todo: scroll wheel scrolls value
//todo: isn't storing the value correctly

wxBEGIN_EVENT_TABLE(TextSlider, wxTextCtrl)
    EVT_LEFT_DOWN(TextSlider::OnMouseLeftDown)
    EVT_LEFT_UP(TextSlider::OnMouseLeftUp)
    EVT_MOTION(TextSlider::OnMouseMove)
    EVT_ENTER_WINDOW(TextSlider::OnMouseEnter)
    EVT_KILL_FOCUS(TextSlider::OnKillFocus)
    EVT_TEXT(wxID_ANY, TextSlider::OnText)
    EVT_CHAR(TextSlider::OnChar)
wxEND_EVENT_TABLE()

TextSlider::TextSlider(wxWindow* parent, wxWindowID id, const wxString& defaultval, float minval, float maxval, const wxPoint& pos) : wxTextCtrl(parent, id, defaultval, pos, wxDefaultSize, wxNO_BORDER), val_(std::stof(static_cast<std::string>(defaultval))), min_(minval), max_(maxval) {
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

void TextSlider::OnMouseLeftUp(wxMouseEvent& e) { // todo: change so it only displays as many decimals as needed
    if (clickpos_ == prevpos_) { // todo change text to real value
        SetFocus();
        SetValue(std::to_string(val_));
        SetSelection(0, -1);
    }
     if (HasCapture()) ReleaseMouse();
}

void TextSlider::OnMouseMove(wxMouseEvent& e) { // todo if shift is held, snap to increments of 5
    if (!HasCapture()) return;
    callcount_++;
    if (callcount_ < 3) return;

    const wxPoint currpos = e.GetPosition();
    const int deltaX = currpos.x - prevpos_.x;
    
    val_ += deltaX > 0 ? 1.0f : deltaX < 0 ? -1.0f : 0.0f;
    val_ = val_ > max_ ? max_ : val_;   // val_ = std::min(max_, std::max(min_, val_ + (deltaX > 0 ? 1 : -1)));
    val_ = val_ < min_ ? min_ : val_;
    
    prevpos_ = currpos;
    callcount_ = 0;
    
    const std::string label = std::to_string(val_);
    SetValue(label.substr(0,label.find('.')+3));
}

void TextSlider::OnMouseEnter(wxMouseEvent& e) {
    SetCursor(wxCursor(wxCURSOR_SIZEWE));
}

void TextSlider::OnKillFocus(wxFocusEvent& e) {
    const std::string label = std::to_string(val_);
    SetValue(label.substr(0,label.find('.')+3));
    Hide();Show(); // dumbass work around for cursor problem
    SetInsertionPointEnd();
}

void TextSlider::OnText(wxCommandEvent& e) {
    //if (GetValue().substr(GetValue().find('.')).size() > 2)
        val_ = std::stof(static_cast<std::string>(GetValue()));
}

void TextSlider::OnChar(wxKeyEvent& e) {
    const int key = e.GetKeyCode();
    
    if (!((key >= '0' && key <= '9') ||
        key == WXK_BACK || key == WXK_DELETE ||
        key == WXK_LEFT || key == WXK_RIGHT  ||
        key == WXK_UP   || key == WXK_DOWN   || key == '.'))
        return;
    e.Skip();
}

float TextSlider::getvalue() const {
    return val_;
}
