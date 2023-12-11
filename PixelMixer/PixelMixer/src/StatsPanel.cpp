#include "StatsPanel.h"
#include "Palette.h"
#include "ViewportPanel.h"

BEGIN_EVENT_TABLE(StatsPanel, wxPanel)
END_EVENT_TABLE()

StatsPanel::StatsPanel(wxWindow* parent, ViewportPanel* viewport) : wxPanel(parent), viewport_(viewport) {
    wxWindowBase::SetBackgroundColour(Palette::config);

    rendertime_ = new wxStaticText(this, wxID_ANY, "Render time", wxPoint(30, 30));
    rendertimevalue_ = new wxStaticText(this, wxID_ANY, "0.00ms", wxPoint(150, 30));
    rendertime_->SetForegroundColour(Palette::text);
    rendertime_->SetFont(Font::read);
    rendertimevalue_->SetForegroundColour(Palette::clickable);
    rendertimevalue_->SetFont(Font::read);
    
    zoomfactor_ = new wxStaticText(this, wxID_ANY, "Zoom factor", wxPoint(30, 60));
    zoomfactorvalue_ = new wxStaticText(this, wxID_ANY, "100%", wxPoint(150, 60));
    zoomfactor_->SetForegroundColour(Palette::text);
    zoomfactor_->SetFont(Font::read);
    zoomfactorvalue_->SetForegroundColour(Palette::clickable);
    zoomfactorvalue_->SetFont(Font::read);

    position_ = new wxStaticText(this, wxID_ANY, "Position", wxPoint(30, 90));
    positionvalue_ = new wxStaticText(this, wxID_ANY, "(0, 0)", wxPoint(150, 90));
    position_->SetForegroundColour(Palette::text);
    position_->SetFont(Font::read);
    positionvalue_->SetForegroundColour(Palette::clickable);
    positionvalue_->SetFont(Font::read);
    
    viewport_->SetStatsPanel(this);
}

void StatsPanel::UpdateRenderTime(double time) const {
    if (!IsShown()) return;
    rendertimevalue_->SetLabel(std::to_string(time).substr(0, 4) + "ms");
}

void StatsPanel::UpdateZoomFactor(double zoomfactor) const { // technically this isn't accurate
    if (!IsShown()) return;
    std::string string = std::to_string(zoomfactor*100);
    zoomfactorvalue_->SetLabel(string.substr(0, string.find('.')) + "%");
}

void StatsPanel::UpdatePosition(int x, int y) const {
    if (!IsShown()) return;
    positionvalue_->SetLabel("(" + std::to_string(x) + ", " + std::to_string(y) + ")");
}