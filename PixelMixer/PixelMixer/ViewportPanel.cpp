#include "ViewportPanel.h"

#include "FileDropTarget.h"

ViewportPanel::ViewportPanel(wxWindow* parent) : wxPanel(parent) {
    wxWindowBase::SetBackgroundColour("#36393e");

    // Enable drag-and-drop support
    wxWindow::SetDropTarget(new FileDropTarget(this));
    
    // Bind the drop event handler
    Bind(wxEVT_DROP_FILES, &ViewportPanel::OnDropFiles, this);
}

void ViewportPanel::OnDropFiles(wxDropFilesEvent& event) {
    // Get the dropped file path
    droppedFilePath = event.GetFiles()[0];
    
    // Load and display the image
    LoadAndDisplayImage(droppedFilePath);
    
    event.Skip();
}

void ViewportPanel::LoadAndDisplayImage(const wxString& filePath) {
    wxInitAllImageHandlers();
    const wxImage image(filePath, wxBITMAP_TYPE_ANY);

    if (image.IsOk()) {
        // Calculate the center position
        int centerX = (GetSize().GetWidth() - image.GetWidth()) / 2;
        int centerY = (GetSize().GetHeight() - image.GetHeight()) / 2;
        
        // Create a bitmap from the image
        wxBitmap bitmap(image);
        
        // Create a memory DC and draw the image onto it
        wxClientDC dc(this);
        wxMemoryDC memDC;
        memDC.SelectObject(bitmap);
        dc.Blit(centerX, centerY, image.GetWidth(), image.GetHeight(), &memDC, 0, 0);
    }
}