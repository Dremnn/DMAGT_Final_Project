#pragma once
#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/statline.h>

class MapPanel;

class SearchPanel : public wxPanel
{
public:
    SearchPanel(wxWindow* parent);
    void SetMapPanel(MapPanel* mapPanel);

private:
    wxTextCtrl* m_startPointCtrl;
    wxTextCtrl* m_endPointCtrl;
    MapPanel* m_mapPanel = nullptr;

    // UI setup methods
    void SetupModernUI();
    void CreateSearchSection(wxBoxSizer* mainSizer);
    void CreateButtonSection(wxBoxSizer* mainSizer);
    void CreateSavedPlacesSection(wxBoxSizer* mainSizer);

    // Event handlers
    void OnSearchClicked(wxCommandEvent& event);
    void OnShowAllPathsClicked(wxCommandEvent& event);
    void OnClearClicked(wxCommandEvent& event);
};