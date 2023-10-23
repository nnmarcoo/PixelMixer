#pragma once
#include "wx/wx.h"

class TextSlider : wxStaticText {
public:
    TextSlider(wxWindow* parent, wxWindowID id, const wxString& defaultval, int minval, int maxval);
private:
    DECLARE_EVENT_TABLE()

    void OnMouseLeftDown(wxMouseEvent& e);
    void OnMouseLeftUp(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnMouseEnter(wxMouseEvent& e);

    float val_;
    float min_;
    float max_;
    wxPoint prevpos_;
};