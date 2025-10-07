#pragma once
#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/statline.h>

// Forward declaration
class MapPanel;
class wxPopupWindow; 

class SearchPanel : public wxPanel
{
public:
    SearchPanel(wxWindow* parent);
    void SetMapPanel(MapPanel* mapPanel);
    void UpdateRouteSelection(int startIdx, int endIdx);

private:
    bool m_isSelectingRoute = false;
	
    wxTextCtrl* m_startPointCtrl;
    wxTextCtrl* m_endPointCtrl;

	// các con trỏ đến các thành phần giao diện khác
    MapPanel* m_mapPanel = nullptr;
    wxButton* m_selectRouteButton = nullptr;
    wxButton* m_infoButton = nullptr;
    wxPopupWindow* m_infoPopup = nullptr;

    // UI setup methods
    void SetupModernUI();
    void CreateSearchSection(wxBoxSizer* mainSizer);
    void CreateButtonSection(wxBoxSizer* mainSizer);
    void CreateSavedPlacesSection(wxBoxSizer* mainSizer);

    // Event handlers
    void OnSearchClicked(wxCommandEvent& event);
    void OnShowAllPathsClicked(wxCommandEvent& event);
    void OnClearClicked(wxCommandEvent& event);
    void OnSelectRouteClicked(wxCommandEvent& event);

    // Sự kiện di chuột vào/ra khỏi nút info
    void OnInfoButtonEnter(wxMouseEvent& event);
    void OnInfoButtonLeave(wxMouseEvent& event);
};