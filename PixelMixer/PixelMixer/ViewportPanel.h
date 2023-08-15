#pragma once
#include <wx/wx.h>

class ViewportPanel : public wxPanel {
public:
    ViewportPanel(wxWindow* parent);
    void LoadAndDisplayImage(const wxString& filePath);

private:
    wxDECLARE_EVENT_TABLE();
    
    wxString droppedFilePath;
    bool canDND = true;
    wxBitmap loadedBitmap;
    int imageCenterX;
    int imageCenterY;
    
    void OnDropFiles(wxDropFilesEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    
};