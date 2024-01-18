#include "TextSlider.h"
#include "Palette.h"

// todo: scroll wheel scrolls value
// todo: fix cursor not going away on focus kill (fix: have dummy textctrl to switch to?)

//Causing error with comilation: wxEvtHandler is an inaccessible base of 'TextSlider'
//Might just be a me issue.
// wxBEGIN_EVENT_TABLE(TextSlider, wxTextCtrl)
//     EVT_LEFT_DOWN(TextSlider::OnMouseLeftDown)
//     EVT_LEFT_UP(TextSlider::OnMouseLeftUp)
//     EVT_MOTION(TextSlider::OnMouseMove)
//     EVT_ENTER_WINDOW(TextSlider::OnMouseEnter)
//     EVT_KILL_FOCUS(TextSlider::OnKillFocus)
//     EVT_CHAR(TextSlider::OnChar)
//     EVT_RIGHT_DOWN(TextSlider::OnRightDown)
// wxEND_EVENT_TABLE()

TextSlider::TextSlider(wxWindow* parent, wxWindowID id, const wxString& defaultval, float minval, float maxval, const wxPoint& pos, wxString unit) : wxTextCtrl(parent, id, defaultval + unit, pos, wxDefaultSize, wxNO_BORDER), val_(std::stof(static_cast<std::string>(defaultval))), min_(minval), max_(maxval), unit_(unit) {
    wxTextCtrl::SetForegroundColour(Palette::clickable);
    wxTextCtrl::SetBackgroundColour(Palette::config);
    wxTextCtrl::SetFont(Font::click);
}

void TextSlider::OnMouseLeftDown(wxMouseEvent& e) {
    const wxPoint mousepos = e.GetPosition();
    prevpos_ = mousepos;
    clickpos_ = mousepos;
    CaptureMouse();
}

void TextSlider::OnMouseLeftUp(wxMouseEvent& e) {
    if (clickpos_ == prevpos_) {
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
    const float step = max_ / 100;
    
    val_ += deltaX > 0 ? step : deltaX < 0 ? -step : 0.0f;
    val_ = val_ > max_ ? max_ : val_;                           // val_ = std::min(max_, std::max(min_, val_ + (deltaX > 0 ? 1 : -1)));
    val_ = val_ < min_ ? min_ : val_;
    
    prevpos_ = currpos;
    callcount_ = 0;
    
    const std::string label = std::to_string(val_);
    SetValue(label.substr(0,label.find('.')+3) + unit_);
}

void TextSlider::OnMouseEnter(wxMouseEvent& e) {
    SetCursor(wxCursor(wxCURSOR_SIZEWE));
}

void TextSlider::OnKillFocus(wxFocusEvent& e) {
    const std::string label = std::to_string(val_);
    SetValue(label.substr(0,label.find('.')+3) + unit_);
    SetInsertionPointEnd();
}

//undefined vtable for TextSlider, fixed by implementing all
//the methods in textslider
void TextSlider::OnText(wxCommandEvent& e) {
}

void TextSlider::OnChar(wxKeyEvent& e) {
    const int key = e.GetKeyCode();
    
    if (!((key >= '0' && key <= '9') ||
        key == WXK_BACK || key == WXK_DELETE ||
        key == WXK_LEFT || key == WXK_RIGHT  ||
        key == WXK_UP   || key == WXK_DOWN   ||
        key == '.'                           ))
        return;
    
    if (std::stof(std::string(GetValue() + static_cast<char>(key))) > max_ ||
        std::stof(std::string(GetValue() + static_cast<char>(key))) < min_  ) 
        return;
    
    // Handle inputs manually because e.Skip() causes mistake where the input is 1 step behind 
    // todo: handle exceptions
    // todo: add shift < > controls
    
    long start, end;
    GetSelection(&start, &end);

    if (start != end)
        Remove(start, end);

    std::string value{GetValue()};
    const long insertion = GetInsertionPoint();

    switch(key) { // todo: support shift selection
    case WXK_RIGHT:
    case WXK_UP:
        SetInsertionPoint(insertion+1);
        break;
    case WXK_LEFT:
    case WXK_DOWN:
        SetInsertionPoint(insertion-1);
        break;
    case WXK_BACK:
        Remove(insertion, insertion-1);
        break;
    case WXK_DELETE:
        Remove(insertion, insertion+1);
        break;
    default:
        WriteText(static_cast<char>(key));
    }
    
    try {
        val_ = std::stof(std::string(GetValue()));
    } catch (const std::invalid_argument&) {
        val_ = 0;
    }

    //SetValue(GetValue());
}

void TextSlider::OnRightDown(wxMouseEvent& e) { // Remove native m2 feature
    e.Skip(false);
}

float TextSlider::Value() const {
    return val_;
}
