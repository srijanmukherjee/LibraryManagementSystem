#pragma once
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
 
class LibraryManagementApp : public wxApp
{
public:
    virtual bool OnInit();
};

DECLARE_APP(LibraryManagementApp);

enum
{
    ID_Hello = 1
};