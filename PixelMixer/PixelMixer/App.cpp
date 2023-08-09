#include "App.h"
#include "MainFrame.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    MainFrame* mainFrame = new MainFrame("Pixel Mixer");
    mainFrame->SetClientSize(800, 600);
    mainFrame->Center();
    //mainFrame->SetWindowStyle(wxFRAME_NO_TASKBAR);


    
    mainFrame->Show();
    return true; // processing should continue (if return false, app closes)
}
