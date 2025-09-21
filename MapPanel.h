#pragma once
#include <wx/wx.h>
#include <vector>
#include <map>
#include <utility> 

using namespace std;

struct MapNode {
    wxPoint pos;
    bool isBigNode = false; // true : địa danh , false : giao lộ
	wxString name; // Tên của node, chỉ dùng cho node lớn (địa danh)
};

class MapPanel : public wxPanel
{
public:
    MapPanel(wxWindow* parent);
    // Hàm để SearchPanel gọi tìm node từ tên
    int FindNodeIndexByName(const wxString& name) const;
    // Hàm để SearchPanel gọi vẽ đường đi
    void FindAndDrawNewPath(int startIndex, int endIndex);

private:
    wxBitmap m_mapBitmap;
    wxBitmap m_scaledBitmap;

    // Vector chứa tất cả các node trên bản đồ
    vector<MapNode> m_nodes;
    // Vector chứa các cặp chỉ số của các node để định nghĩa đường đi
    vector<pair<int, int>> m_path;
    // Vecto chứa chỉ số các node trong đường đi ngắn nhất
    std::map<int, std::map<int, double>> m_adajacentList;

    void CreateGraph();

    void OnPaint(wxPaintEvent& event);

    double m_scale = 1.0;
    wxPoint m_offset = wxPoint(0, 0);
    wxPoint m_dragStart;
    bool m_isDragging = false;


    void UpdateScaledBitmap();
    void ClampOffset();

    void OnMouseWheel(wxMouseEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
};