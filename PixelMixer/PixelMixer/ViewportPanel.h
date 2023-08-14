#pragma once
#include <wx/wx.h>

class ViewportPanel : public wxPanel {
public:
    ViewportPanel(wxWindow* parent);
    void LoadAndDisplayImage(const wxString& filePath);

private:
    wxString droppedFilePath;
    bool canDND = true;
    
    void OnDropFiles(wxDropFilesEvent& event);
    
};