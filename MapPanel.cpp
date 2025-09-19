#include "MapPanel.h"
#include <wx/dcclient.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

MapPanel::MapPanel(wxWindow* parent)
    : wxPanel(parent)
{
    // Tải hình ảnh bản đồ từ file
    if (!m_mapBitmap.LoadFile("MapQuan1.png", wxBITMAP_TYPE_PNG)) {
        wxLogError("Không thể tải ảnh MapQuan1.png");
    }

    // Liên kết các sự kiện chuột với các hàm xử lý
    Bind(wxEVT_PAINT, &MapPanel::OnPaint, this);
    Bind(wxEVT_MOUSEWHEEL, &MapPanel::OnMouseWheel, this);
    Bind(wxEVT_LEFT_DOWN, &MapPanel::OnLeftDown, this);
    Bind(wxEVT_MOTION, &MapPanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_UP, &MapPanel::OnLeftUp, this);

    // Lần đầu tiên, cập nhật bitmap đã co giãn
    UpdateScaledBitmap();
}

void MapPanel::UpdateScaledBitmap()
{
    if (!m_mapBitmap.IsOk()) return;

    wxSize originalSize = m_mapBitmap.GetSize();
    int scaledWidth = originalSize.GetWidth() * m_scale;
    int scaledHeight = originalSize.GetHeight() * m_scale;

    wxImage originalImage = m_mapBitmap.ConvertToImage();
    wxImage scaledImage = originalImage.Scale(scaledWidth, scaledHeight, wxIMAGE_QUALITY_HIGH);

    m_scaledBitmap = wxBitmap(scaledImage);

    // Sau khi thay đổi tỉ lệ, cần clamp lại offset để đảm bảo nằm trong giới hạn
    ClampOffset();
}

void MapPanel::ClampOffset()
{
    if (!m_scaledBitmap.IsOk()) return;

    wxSize panelSize = GetSize();
    wxSize scaledSize = m_scaledBitmap.GetSize();

    // Giới hạn cho trục X
    if (scaledSize.GetWidth() > panelSize.GetWidth()) {
        int minX = panelSize.GetWidth() - scaledSize.GetWidth();
        if (m_offset.x > 0) m_offset.x = 0;
        if (m_offset.x < minX) m_offset.x = minX;
    }
    else {
        m_offset.x = (panelSize.GetWidth() - scaledSize.GetWidth()) / 2;
    }

    // Giới hạn cho trục Y
    if (scaledSize.GetHeight() > panelSize.GetHeight()) {
        int minY = panelSize.GetHeight() - scaledSize.GetHeight();
        if (m_offset.y > 0) m_offset.y = 0;
        if (m_offset.y < minY) m_offset.y = minY;
    }
    else {
        m_offset.y = (panelSize.GetHeight() - scaledSize.GetHeight()) / 2;
    }
}

void MapPanel::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);
    dc.Clear();

    if (!m_scaledBitmap.IsOk()) return;

    dc.DrawBitmap(m_scaledBitmap, m_offset.x, m_offset.y, false);
}

void MapPanel::OnMouseWheel(wxMouseEvent& event)
{
    int rotation = event.GetWheelRotation();
    wxSize panelSize = GetSize();
    wxSize bitmapSize = m_mapBitmap.GetSize();

    if (rotation > 0) {
        // Zoom In
        m_scale *= 1.1;
        // Giới hạn tỉ lệ phóng to để tránh lãng phí tài nguyên
        if (m_scale > 2.36) { // Giới hạn zoom in ở mức 2.36 lần
            m_scale = 2.36;
        }
    }
    else {
        // Zoom Out
        // Lấy kích thước ảnh hiện tại
        double currentImageWidth = bitmapSize.GetWidth() * m_scale;
        double currentImageHeight = bitmapSize.GetHeight() * m_scale;

        // Chỉ cho phép zoom out nếu ảnh lớn hơn panel
        if (currentImageWidth > panelSize.GetWidth() || currentImageHeight > panelSize.GetHeight()) {
            m_scale /= 1.1;
        }

        // Cố định tỉ lệ zoom để không nhỏ hơn mức full panel
        double minScaleX = (double)panelSize.x / bitmapSize.x;
        double minScaleY = (double)panelSize.y / bitmapSize.y;
        double minScale = std::min(minScaleX, minScaleY);
        if (m_scale < minScale) {
            m_scale = minScale;
        }
    }

    UpdateScaledBitmap();
    Refresh();
}

void MapPanel::OnLeftDown(wxMouseEvent& event)
{
    m_isDragging = true;
    m_dragStart = event.GetPosition();
    CaptureMouse();
}

void MapPanel::OnMouseMove(wxMouseEvent& event)
{
    if (m_isDragging) {
        wxPoint currentPos = event.GetPosition();
        m_offset.x += (currentPos.x - m_dragStart.x);
        m_offset.y += (currentPos.y - m_dragStart.y);
        m_dragStart = currentPos;

        ClampOffset();

        Refresh();
    }
}

void MapPanel::OnLeftUp(wxMouseEvent& event)
{
    m_isDragging = false;
    if (HasCapture()) {
        ReleaseMouse();
    }
}