#include "HeaderPanel.h"

HeaderPanel::HeaderPanel(wxWindow *parent, wxWindowID winid) : wxPanel(parent, wxID_ANY)
{
    wxWindowBase::SetBackgroundColour("#2c2f33");

    Bind(wxEVT_LEFT_DOWN, &HeaderPanel::OnHeaderLeftDown, this);
    Bind(wxEVT_LEFT_UP, &HeaderPanel::OnHeaderLeftUp, this);
    Bind(wxEVT_MOTION, &HeaderPanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_DCLICK, &HeaderPanel::OnHeaderDoubleClick, this);
}

// must put 'mainFrame->' in front of all controls for the parent frame. Harder to read here but cleans up MainFrame.cpp
void HeaderPanel::OnHeaderLeftDown(const wxMouseEvent& e) {
    isDragging_ = true; //          todo WTF is the point of isDragging_ if I can just check the mouse state???
    dragStart_ = e.GetPosition();
}

void HeaderPanel::OnHeaderLeftUp(wxMouseEvent& e) {
    isDragging_ = false;

    const wxPoint mousePos = wxGetMousePosition(); // these two lines get the mouse position on the monitor it is on
    const wxPoint rMousePos = mousePos - wxDisplay(wxDisplay::GetFromPoint(mousePos)).GetGeometry().GetPosition();

    if (rMousePos.y == 0) { // todo implement docking for side of screen
        mainFrame->SetPosition(mainFrame->GetPosition() + wxPoint(0, 50));
        mainFrame->Maximize();
    }
}

void HeaderPanel::OnMouseMove(const wxMouseEvent& e) {
    if (mainFrame->IsMaximized()) isDragging_ = false;
    if (isDragging_  && wxGetMouseState().LeftIsDown()) {
        const wxPoint newPos = e.GetPosition() - dragStart_;
        mainFrame->SetPosition(mainFrame->GetPosition() + newPos);
    }
}

void HeaderPanel::OnHeaderDoubleClick(wxMouseEvent& e) {
    if (mainFrame->IsMaximized()) 
        mainFrame->Restore();
    else 
        mainFrame->Maximize();
}