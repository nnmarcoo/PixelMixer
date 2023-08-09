#include <wx/wx.h>

class App final : public wxApp {
public:
    bool OnInit() override
    {
        auto window = new wxFrame(nullptr, wxID_ANY, "GUI Test", wxDefaultPosition, wxSize(600, 400));
        auto* sizer = new wxBoxSizer(wxHORIZONTAL);
        auto* text = new wxStaticText(window, wxID_ANY, "Well Done!\nEverything seems to be working",
                                      wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE_HORIZONTAL);
        text->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));
        sizer->Add(text, 1, wxALIGN_CENTER);
        window->SetSizer(sizer);
        window->Show();
        return true;
    }
};

wxIMPLEMENT_APP(App);