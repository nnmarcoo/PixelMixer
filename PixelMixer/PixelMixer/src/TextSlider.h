#pragma once
#include "wx/wx.h"

class TextSlider : wxTextCtrl {
public:
    TextSlider(wxWindow* parent, wxWindowID id, const wxString& defaultval, float minval, float maxval, const wxPoint& pos = wxDefaultPosition);
    float getvalue();
private:
    DECLARE_EVENT_TABLE()

    void OnMouseLeftDown(wxMouseEvent& e);
    void OnMouseLeftUp(wxMouseEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnMouseEnter(wxMouseEvent& e);
    
    wxPoint dragpos_;

    float val_;
    float min_;
    float max_;
    wxPoint prevpos_;
};