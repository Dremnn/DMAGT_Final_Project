#pragma once
#include <wx/wx.h>
#include <vector> // Thêm thư viện vector

class MapPanel : public wxPanel
{
public:
    MapPanel(wxWindow* parent);

private:
    // Các biến thành viên cho bản đồ và pan/zoom (giữ nguyên)
    wxBitmap m_mapBitmap;
    wxBitmap m_scaledBitmap;
    double m_scale = 1.0;
    wxPoint m_offset = wxPoint(0, 0);
    wxPoint m_dragStart;
    bool m_isDragging = false;

    // Biến thành viên mới để lưu tọa độ đường đi
    std::vector<wxPoint> m_routePointsGoc;

    // Các hàm xử lý (giữ nguyên)
    void OnPaint(wxPaintEvent& event);
    void UpdateScaledBitmap();
    void ClampOffset();
    void OnMouseWheel(wxMouseEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
};