#pragma once
#include <wx/wx.h>

class DropdownLineBreak : public wxStaticBox {
public:
    DropdownLineBreak(wxWindow* parent, const wxString& label = wxEmptyString, const wxSize& size = wxSize(230, 1))
        : wxStaticBox(parent, wxID_ANY, label, wxDefaultPosition, size) {
        wxStaticBox::SetBackgroundColour("#646464");
    }
};