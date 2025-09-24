#include "SimpleUIHelper.h"
#include "ModernColors.h"  

void SimpleUIHelper::StyleButton(wxButton* button, bool isPrimary) {
    if (!button) return;

    if (isPrimary) {
        button->SetBackgroundColour(ModernColors::PRIMARY_GREEN);
        button->SetForegroundColour(ModernColors::TEXT_WHITE);
    }
    else {
        button->SetBackgroundColour(ModernColors::BACKGROUND_CARD);
        button->SetForegroundColour(ModernColors::PRIMARY_GREEN);
    }

    wxFont font = button->GetFont();
    font.SetFaceName("Segoe UI");
    font.SetWeight(wxFONTWEIGHT_MEDIUM);
    button->SetFont(font);
    button->SetMinSize(wxSize(-1, 36));
}

void SimpleUIHelper::StyleTextCtrl(wxTextCtrl* textCtrl) {
    if (!textCtrl) return;

    textCtrl->SetBackgroundColour(ModernColors::BACKGROUND_INPUT);
    textCtrl->SetForegroundColour(ModernColors::TEXT_PRIMARY);

    wxFont font = textCtrl->GetFont();
    font.SetFaceName("Segoe UI");
    textCtrl->SetFont(font);
    textCtrl->SetMinSize(wxSize(-1, 36));
}

void SimpleUIHelper::StylePanel(wxPanel* panel) {
    if (!panel) return;

    panel->SetBackgroundColour(ModernColors::BACKGROUND_CARD);

    wxWindowList& children = panel->GetChildren();
    for (auto child : children) {
        if (auto btn = dynamic_cast<wxButton*>(child)) {
            StyleButton(btn);
        }
        else if (auto txt = dynamic_cast<wxTextCtrl*>(child)) {
            StyleTextCtrl(txt);
        }
    }
}

void SimpleUIHelper::SetModernFont(wxWindow* window) {
    if (!window) return;

    wxFont font = window->GetFont();
    font.SetFaceName("Segoe UI");
    window->SetFont(font);
}