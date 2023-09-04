#include "Button.h"

Button::Button(wxWindow* parent, wxWindowID id, const wxString& label, const wxPoint& pos, const wxSize& size, long style, const wxBitmapBundle& bitmap, wxColour hovercolor, wxColour defaultcolor, wxFont font) : wxButton(parent, id, label, pos, size, style) {
    wxControlBase::SetFont(font);
    wxAnyButton::SetForegroundColour("#bfbfbf");
    wxAnyButton::SetBackgroundColour(defaultcolor);

    Bind(wxEVT_ENTER_WINDOW, [=](wxMouseEvent& e) {
        SetBackgroundColour(hovercolor);  // Change the background color on hover
        Refresh();
        e.Skip();
    });

    Bind(wxEVT_LEAVE_WINDOW, [=](wxMouseEvent& e) {
        SetBackgroundColour(defaultcolor);  // Change the background color back when leaving hover
        Refresh();
        e.Skip();
    });

    if (bitmap.IsOk())
        SetBitmap(bitmap);
}