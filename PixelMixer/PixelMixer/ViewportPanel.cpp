#include "ViewportPanel.h"
#include "FileDropTarget.h"

wxBEGIN_EVENT_TABLE(ViewportPanel, wxPanel)
    EVT_SIZE(ViewportPanel::OnSize)
    EVT_PAINT(ViewportPanel::OnPaint)
    EVT_DROP_FILES(ViewportPanel::OnDropFiles)
wxEND_EVENT_TABLE()

ViewportPanel::ViewportPanel(wxWindow* parent) : wxPanel(parent) {
    canDND = true;
    wxWindowBase::SetBackgroundColour("#36393e");

    // Enable DND
    if (canDND)
        wxWindow::SetDropTarget(new FileDropTarget(this));
}

void ViewportPanel::OnDropFiles(wxDropFilesEvent& e) {
    LoadAndDisplayImage(e.GetFiles()[0]);
    e.Skip();
}

void ViewportPanel::LoadAndDisplayImage(const wxString& filePath) {
    if (!canDND) return;
    const wxImage image(filePath, wxBITMAP_TYPE_ANY);

    if (image.IsOk()) {
        // Store image data and center positions
        loadedBitmap = wxBitmap(image);
        imageCenterX = (GetSize().GetWidth() - image.GetWidth()) / 2;
        imageCenterY = (GetSize().GetHeight() - image.GetHeight()) / 2;

        Refresh();  // Trigger repainting
        canDND = false;
    }
}

void ViewportPanel::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);

    if (loadedBitmap.IsOk()) {
        dc.DrawBitmap(loadedBitmap, imageCenterX, imageCenterY, true);
    }
}

void ViewportPanel::OnSize(wxSizeEvent& event) {
    // Update center position based on the new panel size
    imageCenterX = (GetSize().GetWidth() - loadedBitmap.GetWidth()) / 2;
    imageCenterY = (GetSize().GetHeight() - loadedBitmap.GetHeight()) / 2;

    Refresh();  // Trigger repainting
    event.Skip();
}