#include "FileDropTarget.h"
#include "ViewportPanel.h"

FileDropTarget::FileDropTarget(ViewportPanel* panel) : panel_(panel) {}

bool FileDropTarget::OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) {
    if (filenames.GetCount() == 1) {
        panel_->LoadAndDisplayImage(filenames[0]);
    }
    return true;
}
