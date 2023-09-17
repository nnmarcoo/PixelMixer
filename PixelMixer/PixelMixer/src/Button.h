#pragma once
#include <wx/wx.h>
#include "Palette.h"

class Button : public wxButton {
public:
    Button(wxWindow* parent, wxWindowID id = wxID_ANY,
                 const wxString& label = wxEmptyString,
                 const wxPoint& pos = wxDefaultPosition,
                 const wxSize& size = wxDefaultSize,
                 long style = wxNO_BORDER,
                 const wxBitmapBundle& bitmap = wxNullBitmap,
                 wxColour hovercolor = Palette::ghover,
                 wxColour defaultcolor = Palette::border,
                 wxFont font = wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

    void SetDefaultColor(wxColour c);
    
};
