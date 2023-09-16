#include "Button.h"

Button::Button(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size, long style, const wxBitmapBundle& bitmap, wxColour hovercolor, wxColour defaultcolor, wxFont font) : wxButton(parent, id, label, pos, size, style) {
    wxControlBase::SetFont(font);
    wxAnyButton::SetForegroundColour("#bfbfbf");
    wxAnyButton::SetBackgroundColour(defaultcolor);

    Bind(wxEVT_ENTER_WINDOW, [=](wxMouseEvent& e) {
        SetBackgroundColour(hovercolor);
        Refresh();
    });

    Bind(wxEVT_LEAVE_WINDOW, [=](wxMouseEvent& e) {
        SetBackgroundColour(defaultcolor);
        Refresh();
    });

    if (bitmap.IsOk())
        SetBitmap(bitmap);
}

void Button::SetDefaultColor(wxColour c) {
    SetBackgroundColour(c);
    
    Bind(wxEVT_LEAVE_WINDOW, [=](wxMouseEvent& e) {
        SetBackgroundColour(c);
        Refresh();
    });
}
