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
    bool isPanning;
    wxPoint lastMousePos;
    
    void OnDropFiles(wxDropFilesEvent& e);
    void OnPaint(wxPaintEvent& e);
    void OnSize(wxSizeEvent& e);
    void OnMouseMove(wxMouseEvent& e);
    void OnRightMouseUp(wxMouseEvent& e);
    void OnRightMouseDown(wxMouseEvent& e);
    
    
};