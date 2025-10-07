#pragma once
#include <wx/wx.h>
#include <vector>
#include <map>
#include <set>
#include <utility> 

// Forward declarations
struct SimplePath;
struct ModernColors;
class SearchPanel;

using namespace std;

struct MapNode {
    wxPoint pos;
	bool isBigNode = false; // true nếu là địa danh, false nếu là giao lộ
    wxString name;
};

class MapPanel : public wxPanel
{
public:
    MapPanel(wxWindow* parent);
    int FindNodeIndexByName(const wxString& name) const;
    void FindAndDrawNewPath(int startIndex, int endIndex);
    void FindAndDrawAllPaths(int startIndex, int endIndex);
    void SetSelectionMode(bool enable);
    void ResetSelection();
    void SetSearchPanel(SearchPanel* panel) { m_searchPanel = panel; }
    wxString GetNodeNameByIndex(int idx) const {
        if (idx >= 0 && idx < static_cast<int>(m_nodes.size())) {
            return m_nodes[idx].name;
        }
        return wxEmptyString;
    }
    void ClearAllPaths();
    void HideRouteInfo();

private:
    wxBitmap m_mapBitmap;
    wxBitmap m_scaledBitmap;

    SearchPanel* m_searchPanel = nullptr;

	// Graph data
    vector<MapNode> m_nodes;
    vector<pair<int, int>> m_path;
    vector<SimplePath> m_allPaths;
    map<int, map<int, double>> m_adajacentList;

	// Display nodes
    bool m_showAllNodes = false;
    set<int> m_activeNodes;
    int m_startNodeIndex = -1;  
    int m_endNodeIndex = -1;   
	// Selection mode
    bool m_selectionMode = false;
    int m_selectedNode = -1;

    // Route info popup
    wxPanel* m_routeInfoPanel = nullptr;
    wxPanel* m_routeContentPanel = nullptr;  // Panel chứa nội dung
    wxButton* m_toggleButton = nullptr;      // Nút toggle với mũi tên
    wxStaticText* m_distanceLabel = nullptr;
    wxStaticText* m_grabPriceLabel = nullptr;
    wxStaticText* m_timeLabel = nullptr;
    bool m_showRouteInfo = false;
    bool m_isExpanded = true;               // Trạng thái mở rộng/thu gọn
    wxTimer* m_slideTimer = nullptr;

    // Zoom buttons
    wxButton* m_zoomInButton = nullptr;
    wxButton* m_zoomOutButton = nullptr;

    int m_popupHeight = 120;               // Chiều cao khi mở rộng
    int m_collapsedHeight = 40;            // Chiều cao khi thu gọn (chỉ hiện nút)
    int m_currentHeight = 0;               // Chiều cao hiện tại
    int m_targetHeight = 0;                // Chiều cao mục tiêu
    int m_popupY = 0;                      // Vị trí Y cố định của popup

    // View transformation
    double m_scale = 1.0;
    wxPoint m_offset = wxPoint(0, 0);
    wxPoint m_dragStart;
    bool m_isDragging = false;

    // Methods
    void CreateGraph();
    void UpdateScaledBitmap();
    void ClampOffset();
    void DrawSingleNode(wxDC& dc, const MapNode& node);  // Sửa parameter
	// Methods cho popup panel ( route info)
    void CreateRouteInfoPanel();
    void ShowRouteInfo(double distance);
    void OnSlideTimer(wxTimerEvent& event);
    void OnTogglePopup(wxCommandEvent& event);    // Thay thế OnClosePopup
    void UpdateRouteInfoPosition();
    void ExpandPopup();                           // Mở rộng popup
    void CollapsePopup();                         // Thu gọn popup
    void UpdateToggleButtonText();                // Cập nhật text nút toggle

    // Zoom button methods
    void CreateZoomButtons();
    void UpdateZoomButtonsPosition();
    void OnZoomInClicked(wxCommandEvent& event);
    void OnZoomOutClicked(wxCommandEvent& event);

    // Event handlers
    void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void HandleNodeSelection(const wxPoint& clickPos);
    void OnMouseMove(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
};
