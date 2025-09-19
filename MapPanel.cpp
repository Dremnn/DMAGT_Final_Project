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

    // --- PHẦN THÊM MỚI ---
    // Khởi tạo dữ liệu tọa độ gốc cho đường đi
    // !!! QUAN TRỌNG: Hãy thay các con số này bằng tọa độ thật trên file MapQuan1.png của bạn
    m_routePointsGoc = {
        wxPoint(0, 207), wxPoint(227, 330), wxPoint(231, 337), wxPoint(359,402),
        wxPoint(432, 324)
    };
    // --- KẾT THÚC PHẦN THÊM MỚI ---

    // Liên kết các sự kiện chuột với các hàm xử lý (giữ nguyên)
    Bind(wxEVT_PAINT, &MapPanel::OnPaint, this);
    Bind(wxEVT_MOUSEWHEEL, &MapPanel::OnMouseWheel, this);
    Bind(wxEVT_LEFT_DOWN, &MapPanel::OnLeftDown, this);
    Bind(wxEVT_MOTION, &MapPanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_UP, &MapPanel::OnLeftUp, this);

    // Cập nhật bitmap đã co giãn lần đầu (giữ nguyên)
    UpdateScaledBitmap();
}

// Hàm UpdateScaledBitmap giữ nguyên
void MapPanel::UpdateScaledBitmap()
{
    if (!m_mapBitmap.IsOk()) return;

    wxSize originalSize = m_mapBitmap.GetSize();
    int scaledWidth = originalSize.GetWidth() * m_scale;
    int scaledHeight = originalSize.GetHeight() * m_scale;

    wxImage originalImage = m_mapBitmap.ConvertToImage();
    wxImage scaledImage = originalImage.Scale(scaledWidth, scaledHeight, wxIMAGE_QUALITY_HIGH);

    m_scaledBitmap = wxBitmap(scaledImage);
    ClampOffset();
}

// Hàm ClampOffset giữ nguyên
void MapPanel::ClampOffset()
{
    if (!m_scaledBitmap.IsOk()) return;
    wxSize panelSize = GetSize();
    wxSize scaledSize = m_scaledBitmap.GetSize();

    if (scaledSize.GetWidth() > panelSize.GetWidth()) {
        int minX = panelSize.GetWidth() - scaledSize.GetWidth();
        if (m_offset.x > 0) m_offset.x = 0;
        if (m_offset.x < minX) m_offset.x = minX;
    }
    else {
        m_offset.x = (panelSize.GetWidth() - scaledSize.GetWidth()) / 2;
    }

    if (scaledSize.GetHeight() > panelSize.GetHeight()) {
        int minY = panelSize.GetHeight() - scaledSize.GetHeight();
        if (m_offset.y > 0) m_offset.y = 0;
        if (m_offset.y < minY) m_offset.y = minY;
    }
    else {
        m_offset.y = (panelSize.GetHeight() - scaledSize.GetHeight()) / 2;
    }
}

// ---- HÀM OnPaint ĐƯỢC CẬP NHẬT ----
void MapPanel::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);
    dc.Clear();

    if (!m_scaledBitmap.IsOk()) return;

    // 1. Vẽ bản đồ nền đã được zoom/pan
    dc.DrawBitmap(m_scaledBitmap, m_offset.x, m_offset.y, false);

    // 2. Biến đổi tọa độ các điểm trên đường đi dựa trên scale và offset hiện tại
    std::vector<wxPoint> routePointsVe;
    for (const auto& ptGoc : m_routePointsGoc) {
        int x_moi = (ptGoc.x * m_scale) + m_offset.x;
        int y_moi = (ptGoc.y * m_scale) + m_offset.y;
        routePointsVe.push_back(wxPoint(x_moi, y_moi));
    }

    // 3. Vẽ đường đi và các dấu chấm lên trên bản đồ
    if (routePointsVe.size() > 1) {
        dc.SetPen(wxPen(*wxBLUE, 3, wxPENSTYLE_SOLID));
        dc.DrawLines(routePointsVe.size(), routePointsVe.data());
    }

    if (!routePointsVe.empty()) {
        dc.SetBrush(*wxGREEN_BRUSH);
        dc.DrawCircle(routePointsVe.front(), 4); // Điểm bắt đầu
        dc.SetBrush(*wxRED_BRUSH);
        dc.DrawCircle(routePointsVe.back(), 4); // Điểm kết thúc
    }
}

// Các hàm xử lý chuột còn lại giữ nguyên
void MapPanel::OnMouseWheel(wxMouseEvent& event)
{
    int rotation = event.GetWheelRotation();
    wxSize panelSize = GetSize();
    wxSize bitmapSize = m_mapBitmap.GetSize();

    if (rotation > 0) {
        m_scale *= 1.1;
        if (m_scale > 2.36) {
            m_scale = 2.36;
        }
    }
    else {
        double currentImageWidth = bitmapSize.GetWidth() * m_scale;
        double currentImageHeight = bitmapSize.GetHeight() * m_scale;

        if (currentImageWidth > panelSize.GetWidth() || currentImageHeight > panelSize.GetHeight()) {
            m_scale /= 1.1;
        }

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