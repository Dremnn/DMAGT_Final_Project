#include "App.h"
#include "MainFrame.h"

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    wxImage::AddHandler(new wxPNGHandler);

    MainFrame* frame = new MainFrame("Finding Grab - Routes");
    frame->SetSize(1200, 800);
    frame->Center();
    frame->Show(true);
    return true;
}