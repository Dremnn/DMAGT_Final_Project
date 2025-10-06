#pragma once
#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/statline.h>

// Forward declaration
class MapPanel;
class wxPopupWindow; // Thêm forward declaration cho popup

class SearchPanel : public wxPanel
{
public:
    SearchPanel(wxWindow* parent);
    void SetMapPanel(MapPanel* mapPanel);

private:
    wxTextCtrl* m_startPointCtrl;
    wxTextCtrl* m_endPointCtrl;
    MapPanel* m_mapPanel = nullptr;

    // --- BẮT ĐẦU THAY ĐỔI ---
    wxButton* m_infoButton = nullptr;
    wxPopupWindow* m_infoPopup = nullptr;
    // --- KẾT THÚC THAY ĐỔI ---

    // UI setup methods
    void SetupModernUI();
    void CreateSearchSection(wxBoxSizer* mainSizer);
    void CreateButtonSection(wxBoxSizer* mainSizer);
    void CreateSavedPlacesSection(wxBoxSizer* mainSizer);

    // Event handlers
    void OnSearchClicked(wxCommandEvent& event);
    void OnShowAllPathsClicked(wxCommandEvent& event);
    void OnClearClicked(wxCommandEvent& event);

    // --- BẮT ĐẦU THAY ĐỔI ---
    // Sự kiện di chuột vào/ra khỏi nút info
    void OnInfoButtonEnter(wxMouseEvent& event);
    void OnInfoButtonLeave(wxMouseEvent& event);
    // --- KẾT THÚC THAY ĐỔI ---
};