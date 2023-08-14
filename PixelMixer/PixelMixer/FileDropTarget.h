#pragma once
#include <wx/wx.h>
#include <wx/dnd.h>

class ViewportPanel; // Forward declaration

class FileDropTarget : public wxFileDropTarget {
public:
    FileDropTarget(ViewportPanel* panel);

    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override;

private:
    ViewportPanel* panel_;
};
