#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    auto* mainFrame = new MainFrame("Pixel Mixer");
    mainFrame->Show();
    
    return true;
}