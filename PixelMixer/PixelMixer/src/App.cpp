#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() // todo fix the dependencies to be forwarded
{
    auto* mainFrame = new MainFrame("Pixel Mixer");
    mainFrame->Show();
    
    return true; // processing should continue (if return false, app closes)
}