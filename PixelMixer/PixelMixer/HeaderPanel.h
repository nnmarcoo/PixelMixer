#pragma once
#include <wx/wx.h>
#include <wx/display.h>

class HeaderPanel : public wxPanel {
public:
    HeaderPanel(wxWindow* parent, wxWindowID winid);

private:
    bool isDragging_;
    wxPoint dragStart_;
    void OnHeaderLeftDown(const wxMouseEvent& e);
    void OnHeaderLeftUp(wxMouseEvent& e);
    void OnMouseMove(const wxMouseEvent& e);
    void OnHeaderDoubleClick(wxMouseEvent& e);

    wxFrame* mainFrame = static_cast<wxFrame*>(wxGetTopLevelParent(this));
};
