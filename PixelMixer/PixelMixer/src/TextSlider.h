#pragma once
#include "wx/wx.h"

class TextSlider : wxStaticText {
public:
    TextSlider(wxWindow* parent, wxWindowID id, const wxString& label, wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize, long style = 0);
private:
    DECLARE_EVENT_TABLE()

    void OnMouseLeftDown(wxMouseEvent& e);
    
};