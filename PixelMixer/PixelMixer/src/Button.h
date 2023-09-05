#pragma once
#include <wx/wx.h>

class Button : public wxButton {
public:
    Button(wxWindow* parent, wxWindowID id = wxID_ANY,
                 const wxString& label = wxEmptyString,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxNO_BORDER,
                 const wxBitmapBundle& bitmap = wxNullBitmap,
                 wxColour hovercolor = "#3c3f43",
                 wxColour defaultcolor = "#2c2f33",
                 wxFont font = wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
};
