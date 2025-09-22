#pragma once
#include <wx/wx.h>

struct ModernColors {
    // Declare the static members in the header
    // Use the `extern` keyword to indicate they are defined elsewhere
    static const wxColor PRIMARY;
    static const wxColor SECONDARY;
    static const wxColor SUCCESS;
    static const wxColor WARNING;
    static const wxColor DANGER;
    static const wxColor BACKGROUND;
    static const wxColor SURFACE;
    static const wxColor TEXT_PRIMARY;
    static const wxColor TEXT_SECONDARY;
};