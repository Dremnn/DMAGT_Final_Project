#pragma once
#include <wx/wx.h>

class MapPanel : public wxPanel
{
public:
    MapPanel(wxWindow* parent);

private:
    wxBitmap m_mapBitmap;
    wxBitmap m_scaledBitmap;

    void OnPaint(wxPaintEvent& event);
    
    double m_scale = 1.0; 
    wxPoint m_offset = wxPoint(0, 0); 
    wxPoint m_dragStart; 
    bool m_isDragging = false; 

    void UpdateScaledBitmap();
    void ClampOffset(); // Hàm mới để cố định vị trí offset

    void OnMouseWheel(wxMouseEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
};