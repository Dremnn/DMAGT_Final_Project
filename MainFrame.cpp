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
    SetBackgroundColour(ModernColors::BACKGROUND_MAIN);

	// Create base panel
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);
    mainPanel->SetBackgroundColour(ModernColors::BACKGROUND_MAIN);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // Create search panel
    SearchPanel* searchPanel = new SearchPanel(mainPanel);
    SimpleUIHelper::StylePanel(searchPanel);

	// Create map panel
    MapPanel* mapPanel = new MapPanel(mainPanel);
    mapPanel->SetBackgroundColour(ModernColors::BACKGROUND_CARD);

	// Connect panels
    searchPanel->SetMapPanel(mapPanel);
    mapPanel->SetSearchPanel(searchPanel);
    mainSizer->Add(searchPanel, 0, wxEXPAND | wxALL, 12);
    mainSizer->Add(mapPanel, 1, wxEXPAND | wxTOP | wxBOTTOM | wxRIGHT, 12);

	// Configure main panel
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