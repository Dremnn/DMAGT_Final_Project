#pragma once
#include <wx/wx.h>
#include <wx/srchctrl.h>

// Forward declaration
struct ModernColors;

class MapPanel; 


class SearchPanel : public wxPanel
{
public:
    SearchPanel(wxWindow* parent);
    // Hàm này cho phép truyền con trỏ MapPanel vào
    void SetMapPanel(MapPanel* mapPanel);

private:
    wxTextCtrl* m_startPointCtrl;
    wxTextCtrl* m_endPointCtrl;
    MapPanel* m_mapPanel = nullptr; // Con trỏ tới MapPanel

    void OnSearchClicked(wxCommandEvent& event);
    void OnShowAllPathsClicked(wxCommandEvent& event);
    void OnClearClicked(wxCommandEvent& event);
};