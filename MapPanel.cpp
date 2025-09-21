#include "MapPanel.h"
#include "FindShortestPath.h"
#include "FindAllPaths.h"
#include "NormalizeString.h"
#include <wx/dcclient.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>

using namespace std;

MapPanel::MapPanel(wxWindow* parent)
    : wxPanel(parent)
{
    // Tải hình ảnh bản đồ từ file
    if (!m_mapBitmap.LoadFile("MapQuan1.png", wxBITMAP_TYPE_PNG)) {
        wxLogError("Không thể tải ảnh ");
    }

    // Khởi tạo các node với loại node
    m_nodes.push_back({ wxPoint(100, 100), true, _T("Dinh Thống Nhất") }); // Node 0 
    m_nodes.push_back({ wxPoint(300, 150), false, wxEmptyString }); // Node 1
    m_nodes.push_back({ wxPoint(150, 300), false, wxEmptyString }); // Node 2
    m_nodes.push_back({ wxPoint(400, 250), false, wxEmptyString }); // Node 3
    m_nodes.push_back({ wxPoint(250, 450), false, wxEmptyString }); // Node 4
    m_nodes.push_back({ wxPoint(500, 400), false, wxEmptyString }); // Node 5
    m_nodes.push_back({ wxPoint(450, 600), true, _T("Nhà Thờ Đức Bà") }); // Node 6

    // Tạo đồ thị từ các node
    CreateGraph();

    // Đường đi sẽ được lưu vào m_path
    //m_path = Dijkstra(3, 6, m_nodes, m_adajacentList);

    // Liên kết các sự kiện chuột với các hàm xử lý
    Bind(wxEVT_PAINT, &MapPanel::OnPaint, this);
    Bind(wxEVT_MOUSEWHEEL, &MapPanel::OnMouseWheel, this);
    Bind(wxEVT_LEFT_DOWN, &MapPanel::OnLeftDown, this);
    Bind(wxEVT_MOTION, &MapPanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_UP, &MapPanel::OnLeftUp, this);

    // Cập nhật bitmap đã co giãn lần đầu
    UpdateScaledBitmap();
}

void MapPanel::OnPaint(wxPaintEvent& event)
{
    wxBufferedPaintDC dc(this);
    dc.Clear();

    if (!m_scaledBitmap.IsOk()) return;

    // 1. Vẽ bản đồ nền đã được zoom/pan
    dc.DrawBitmap(m_scaledBitmap, m_offset.x, m_offset.y, false);

    // 2. Vẽ tất cả đường đi với màu khác nhau (nếu có)
    if (!m_allPaths.empty()) {
        // Danh sách màu cho các đường đi
        wxColor pathColors[] = {
            wxColor(255, 0, 255),    // Magenta
            wxColor(0, 255, 255),    // Cyan
            wxColor(255, 165, 0),    // Orange
            wxColor(128, 0, 128),    // Purple
            wxColor(255, 192, 203),  // Pink
            wxColor(0, 128, 0),      // Green
            wxColor(165, 42, 42),    // Brown
            wxColor(128, 128, 128),  // Gray
            wxColor(255, 255, 0),    // Yellow
            wxColor(255, 20, 147),   // Deep Pink
            wxColor(0, 191, 255),    // Deep Sky Blue
            wxColor(50, 205, 50)     // Lime Green
        };

        int colorIndex = 0;
        for (const auto& simplePath : m_allPaths) {
            wxColor color = pathColors[colorIndex % 12];
            dc.SetPen(wxPen(color, 2, wxPENSTYLE_SOLID));

            // Vẽ các cạnh của đường đi này
            for (const auto& edge : simplePath.edges) {
                wxPoint startNodePos = m_nodes[edge.first].pos;
                wxPoint endNodePos = m_nodes[edge.second].pos;

                wxPoint startPos = wxPoint(
                    startNodePos.x * m_scale + m_offset.x,
                    startNodePos.y * m_scale + m_offset.y
                );
                wxPoint endPos = wxPoint(
                    endNodePos.x * m_scale + m_offset.x,
                    endNodePos.y * m_scale + m_offset.y
                );

                dc.DrawLine(startPos, endPos);
            }
            colorIndex++;
        }
    }

    // 3. Vẽ đường đi ngắn nhất (đè lên trên) với màu xanh dương đậm
    dc.SetPen(wxPen(*wxBLUE, 4, wxPENSTYLE_SOLID));
    for (const auto& edge : m_path) {
        wxPoint startNodePos = m_nodes[edge.first].pos;
        wxPoint endNodePos = m_nodes[edge.second].pos;

        wxPoint startPos = wxPoint(
            startNodePos.x * m_scale + m_offset.x,
            startNodePos.y * m_scale + m_offset.y
        );
        wxPoint endPos = wxPoint(
            endNodePos.x * m_scale + m_offset.x,
            endNodePos.y * m_scale + m_offset.y
        );

        dc.DrawLine(startPos, endPos);
    }

    // Vẽ các node
    if (m_showAllNodes) {
        // Vẽ tất cả nodes
        for (const auto& node : m_nodes) {
            DrawSingleNode(dc, node);
        }
    }
    else {
        // Chỉ vẽ những node active
        for (int nodeIndex : m_activeNodes) {
            if (nodeIndex < static_cast<int>(m_nodes.size())) {
                DrawSingleNode(dc, m_nodes[nodeIndex]);
            }
        }
    }
}

// Các hàm để vẽ đường đi
int MapPanel::FindNodeIndexByName(const wxString& name) const
{
    wxString normalizedName = NormalizeString(name);
    for (size_t i = 0; i < m_nodes.size(); ++i) {
        if (m_nodes[i].isBigNode) {
            wxString normalizedNodeName = NormalizeString(m_nodes[i].name);
            if (normalizedNodeName == normalizedName) {
                return i;
            }
        }
    }
    return -1;
}

void MapPanel::FindAndDrawNewPath(int startIndex, int endIndex)
{
    // Tìm đường đi mới
    m_path = Dijkstra(startIndex, endIndex, m_nodes, m_adajacentList);
    // Cập nhật danh sách active nodes
    m_activeNodes.clear();
    for (const auto& edge : m_path) {
        m_activeNodes.insert(edge.first);
        m_activeNodes.insert(edge.second);
    }
    // Yêu cầu vẽ lại MapPanel
    Refresh();
}

void MapPanel::FindAndDrawAllPaths(int startIndex, int endIndex)
{
    // Tìm tất cả đường đi
    m_allPaths = FindAllPaths(startIndex, endIndex, m_nodes, m_adajacentList);

    // Tìm đường đi ngắn nhất để vẽ đè lên
    m_path = Dijkstra(startIndex, endIndex, m_nodes, m_adajacentList);

    // Cập nhật danh sách active nodes (từ tất cả đường đi)
    m_activeNodes.clear();
    for (const auto& simplePath : m_allPaths) {
        for (const auto& edge : simplePath.edges) {
            m_activeNodes.insert(edge.first);
            m_activeNodes.insert(edge.second);
        }
    }

    // Thêm nodes từ đường ngắn nhất (đề phòng)
    for (const auto& edge : m_path) {
        m_activeNodes.insert(edge.first);
        m_activeNodes.insert(edge.second);
    }

    // Yêu cầu vẽ lại
    Refresh();
}

void MapPanel::CreateGraph() {
    m_adajacentList.clear();

    // Các cạnh đã định nghĩa trước
    m_adajacentList[0][1] = Distance(m_nodes[0].pos, m_nodes[1].pos);
    m_adajacentList[1][0] = Distance(m_nodes[1].pos, m_nodes[0].pos);

    m_adajacentList[0][2] = Distance(m_nodes[0].pos, m_nodes[2].pos);
    m_adajacentList[2][0] = Distance(m_nodes[2].pos, m_nodes[0].pos);

    m_adajacentList[1][3] = Distance(m_nodes[1].pos, m_nodes[3].pos);
    m_adajacentList[3][1] = Distance(m_nodes[3].pos, m_nodes[1].pos);

    m_adajacentList[2][4] = Distance(m_nodes[2].pos, m_nodes[4].pos);
    m_adajacentList[4][2] = Distance(m_nodes[4].pos, m_nodes[2].pos);

    m_adajacentList[3][5] = Distance(m_nodes[3].pos, m_nodes[5].pos);
    m_adajacentList[5][3] = Distance(m_nodes[5].pos, m_nodes[3].pos);

    m_adajacentList[4][5] = Distance(m_nodes[4].pos, m_nodes[5].pos);
    m_adajacentList[5][4] = Distance(m_nodes[5].pos, m_nodes[4].pos);

    m_adajacentList[5][6] = Distance(m_nodes[5].pos, m_nodes[6].pos);
    m_adajacentList[6][5] = Distance(m_nodes[6].pos, m_nodes[5].pos);
}

void MapPanel::DrawSingleNode(wxDC& dc, const MapNode& node)
{
    wxPoint screenPos = wxPoint(
        static_cast<int>(node.pos.x * m_scale + m_offset.x),
        static_cast<int>(node.pos.y * m_scale + m_offset.y)
    );

    if (node.isBigNode) {
        dc.SetBrush(*wxRED_BRUSH);
        dc.DrawCircle(screenPos, 5);
    }
    else {
        dc.SetBrush(*wxBLACK_BRUSH);
        dc.DrawCircle(screenPos, 2);
    }
}

void MapPanel::ClearAllPaths()
{
    m_path.clear();
    m_allPaths.clear();
    m_activeNodes.clear();
    m_showAllNodes = false;
    Refresh();
}

// Các hàm xử lý zoom/pan
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

// Các hàm xử lý chuột
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