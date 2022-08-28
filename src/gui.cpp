#include "gui.h"
#include "gui/mainwindow.h"
#include "gui/id.h"

bool LibraryManagementApp::OnInit()
{
    // wxIdleEvent::SetMode(wxIDLE_PROCESS_SPECIFIED);
    // wxUpdateUIEvent::SetMode(wxUPDATE_UI_PROCESS_SPECIFIED);
    // wxUpdateUIEvent::SetUpdateInterval(100);

    if (!wxApp::OnInit())
        return false;

    MainWindow *window = new MainWindow(
        nullptr, 
        window::id::MAINWINDOW,
        _("Library Management Client"));
    window->Show(true);
    
    return true;
}