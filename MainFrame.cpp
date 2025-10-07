#include "MainFrame.h"
#include "SearchPanel.h"
#include "MapPanel.h"
#include "ModernColors.h"
#include "SimpleUIHelper.h"

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1200, 800))
{
    SetupModernUI();
}

void MainFrame::SetupModernUI()
{
    // Set modern background color for main frame
    SetBackgroundColour(ModernColors::BACKGROUND_MAIN);

    // Create main container panel
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);
    mainPanel->SetBackgroundColour(ModernColors::BACKGROUND_MAIN);

    // Create main horizontal layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // 1. Create and setup sidebar (SearchPanel)
    SearchPanel* searchPanel = new SearchPanel(mainPanel);

    // Apply modern styling to search panel
    SimpleUIHelper::StylePanel(searchPanel);

    // 2. Create and setup map area
    MapPanel* mapPanel = new MapPanel(mainPanel);

    // Set modern background for map panel
    mapPanel->SetBackgroundColour(ModernColors::BACKGROUND_CARD);

    // 3. Connect the panels (existing functionality)
    searchPanel->SetMapPanel(mapPanel);
    mapPanel->SetSearchPanel(searchPanel);

    // 4. Setup layout with modern spacing
    // Sidebar: fixed width with padding all around
    mainSizer->Add(searchPanel, 0, wxEXPAND | wxALL, 12);

    // Map area: expandable with padding on top, bottom, right
    mainSizer->Add(mapPanel, 1, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 12);

    // Apply the layout to main panel
    mainPanel->SetSizer(mainSizer);

    // Optional: Create modern status bar
    CreateStatusBar();
    GetStatusBar()->SetBackgroundColour(ModernColors::BACKGROUND_CARD);
    GetStatusBar()->SetForegroundColour(ModernColors::TEXT_SECONDARY);

    // Apply modern font to status bar
    SimpleUIHelper::SetModernFont(GetStatusBar());

    // Center the window on screen
    Center();

    // Set minimum size to prevent UI from breaking
    SetMinSize(wxSize(800, 600));
}