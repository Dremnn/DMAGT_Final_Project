#pragma once
#include <wx/wx.h>
#include <vector>
#include <map>
#include <set>
#include <utility> 

// Forward declarations
struct SimplePath;

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
    void ClearAllPaths();

private:
    wxBitmap m_mapBitmap;
    wxBitmap m_scaledBitmap;
    vector<MapNode> m_nodes;
    vector<pair<int, int>> m_path;
    vector<SimplePath> m_allPaths;
    std::map<int, std::map<int, double>> m_adajacentList;

    // Hiển thị nodes
    bool m_showAllNodes = false;
    std::set<int> m_activeNodes;

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

    // Event handlers
    void OnPaint(wxPaintEvent& event);
    void OnMouseWheel(wxMouseEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
};