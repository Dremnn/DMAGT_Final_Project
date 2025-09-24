#pragma once
#include <wx/wx.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/panel.h>

class SimpleUIHelper {     
public:
    static void StyleButton(wxButton* button, bool isPrimary = true);
    static void StyleTextCtrl(wxTextCtrl* textCtrl);
    static void StylePanel(wxPanel* panel);
    static void SetModernFont(wxWindow* window);
};