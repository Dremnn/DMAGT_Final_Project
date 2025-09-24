#include "MapPanel.h"
#include "FindShortestPath.h"
#include "FindAllPaths.h"
#include "NormalizeString.h"
#include "ModernColors.h"
#include "SimpleUIHelper.h"
#include <wx/dcclient.h>
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <wx/statline.h>

using namespace std;

MapPanel::MapPanel(wxWindow* parent)
    : wxPanel(parent)
{
    // Tải hình ảnh bản đồ từ file
    if (!m_mapBitmap.LoadFile("MapQuan1.png", wxBITMAP_TYPE_PNG)) {
        wxLogError("Không thể tải ảnh ");
    }

    // Khởi tạo các node với loại node
    m_nodes.push_back({ wxPoint(472, 327), true, _T("Dinh Độc Lập") }); // Node 0
    m_nodes.push_back({ wxPoint(518, 313), true, _T("Nhà Thờ Đức Bà") }); // Node 1
    m_nodes.push_back({ wxPoint(520, 302), true,_T("Bưu Điện Trung Tâm") }); // Node 2
    m_nodes.push_back({ wxPoint(490, 424), true,_T("Chợ Bến Thành") }); // Node 3
	m_nodes.push_back({ wxPoint(600, 425), true,_T("Bitexco Financial Tower") }); // Node 4
	m_nodes.push_back({ wxPoint(560, 375), true,_T("Phố đi bộ Bùi Viện") }); // Node 5
	//Dinh Doc Lap -> Nha Tho Duc Ba
    //R1
    m_nodes.push_back({ wxPoint(498, 301), false, wxEmptyString }); // Node 6
    m_nodes.push_back({ wxPoint(507, 302), false, wxEmptyString }); // Node 7
    ////R2
    m_nodes.push_back({ wxPoint(514, 366), false, wxEmptyString }); // Node 8
    m_nodes.push_back({ wxPoint(527, 350), false, wxEmptyString }); // Node 9
    m_nodes.push_back({ wxPoint(495, 320), false, wxEmptyString }); // Node 10 
    m_nodes.push_back({ wxPoint(509, 304), false, wxEmptyString }); // Node 11




    // Tạo đồ thị từ các node
    CreateGraph();

    // Khởi tạo slide timer cho popup
    m_slideTimer = new wxTimer(this, wxID_ANY);
    Bind(wxEVT_TIMER, &MapPanel::OnSlideTimer, this);

    // Tạo route info panel
    CreateRouteInfoPanel();

    // Bind resize event
    Bind(wxEVT_SIZE, &MapPanel::OnSize, this);

    // Liên kết các sự kiện chuột với các hàm xử lý
    Bind(wxEVT_PAINT, &MapPanel::OnPaint, this);
    Bind(wxEVT_MOUSEWHEEL, &MapPanel::OnMouseWheel, this);
    Bind(wxEVT_LEFT_DOWN, &MapPanel::OnLeftDown, this);
    Bind(wxEVT_MOTION, &MapPanel::OnMouseMove, this);
    Bind(wxEVT_LEFT_UP, &MapPanel::OnLeftUp, this);

    // Cập nhật bitmap đã co giãn lần đầu
    UpdateScaledBitmap();
}

// Các hàm để vẽ đường đi

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
    m_path = Dijkstra(startIndex, endIndex, m_nodes, m_adajacentList);

    // Tính khoảng cách
    double pathLength = 0.0;
    for (const auto& edge : m_path) {
        wxPoint p1 = m_nodes[edge.first].pos;
        wxPoint p2 = m_nodes[edge.second].pos;
        double pixelDist = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
        pathLength += pixelDist * 6.565; // meters
    }

    // Hiển thị popup với thông tin
    ShowRouteInfo(pathLength);

    // Cập nhật active nodes
    m_activeNodes.clear();
    for (const auto& edge : m_path) {
        m_activeNodes.insert(edge.first);
        m_activeNodes.insert(edge.second);
    }

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

// Tạo đồ thị từ các node đã định nghĩa
void MapPanel::CreateGraph() {
    m_adajacentList.clear();

    // Các cạnh đã định nghĩa trước
    // Dinh Độc Lập -> Nhà Thờ Đức Bà
    //Route 1: Djikstra
    m_adajacentList[0][6] = Distance(m_nodes[0].pos, m_nodes[6].pos);
    m_adajacentList[6][7] = Distance(m_nodes[6].pos, m_nodes[7].pos);
    m_adajacentList[7][1] = Distance(m_nodes[7].pos, m_nodes[1].pos);
    //Route 2:
    m_adajacentList[0][8] = Distance(m_nodes[0].pos, m_nodes[8].pos);
    m_adajacentList[8][9] = Distance(m_nodes[8].pos, m_nodes[9].pos);
    m_adajacentList[9][10] = Distance(m_nodes[9].pos, m_nodes[10].pos);
    m_adajacentList[10][11] = Distance(m_nodes[10].pos, m_nodes[11].pos);
    m_adajacentList[11][1] = Distance(m_nodes[11].pos, m_nodes[1].pos);
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
        dc.DrawCircle(screenPos, 0);
    }
}

void MapPanel::ClearAllPaths()
{
    m_path.clear();
    m_allPaths.clear();
    m_activeNodes.clear();
    m_showAllNodes = false;
    HideRouteInfo();
    Refresh();
}

// Các hàm xử lý zoom/pan

void MapPanel::UpdateScaledBitmap()
{
    if (!m_mapBitmap.IsOk()) return;

    wxSize originalSize = m_mapBitmap.GetSize();
    int scaledWidth = static_cast<int>(originalSize.GetWidth() * m_scale);
    int scaledHeight = static_cast<int>(originalSize.GetHeight() * m_scale);

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
    wxPoint mousePos = event.GetPosition();
    double oldScale = m_scale;

    // Zoom factor nhỏ để mượt hơn
    double zoomFactor = rotation > 0 ? 1.08 : 0.926;  // 1/1.08 ≈ 0.926
    m_scale *= zoomFactor;

    // Giới hạn scale
    wxSize panelSize = GetSize();
    wxSize bitmapSize = m_mapBitmap.GetSize();
    if (m_scale > 2.36) m_scale = 2.36;

    double minScale = min(static_cast<double>(panelSize.x) / bitmapSize.x,
        static_cast<double>(panelSize.y) / bitmapSize.y);
    if (m_scale < minScale) m_scale = minScale;

    // Zoom tại điểm chuột
    wxPoint worldPos = wxPoint(
        static_cast<int>((mousePos.x - m_offset.x) / oldScale),
        static_cast<int>((mousePos.y - m_offset.y) / oldScale)
    );

    m_offset.x = mousePos.x - worldPos.x * m_scale;
    m_offset.y = mousePos.y - worldPos.y * m_scale;

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


// Các hàm xử lý popup route info

void MapPanel::CreateRouteInfoPanel()
{
    m_routeInfoPanel = new wxPanel(this, wxID_ANY);
    m_routeInfoPanel->SetBackgroundColour(ModernColors::BACKGROUND_CARD);

    // Tạo sizer chính
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Tạo nút toggle với mũi tên 
    m_toggleButton = new wxButton(m_routeInfoPanel, wxID_ANY, wxT("▼ Thông tin lộ trình"),
        wxDefaultPosition, wxSize(-1, m_collapsedHeight));
    m_toggleButton->SetBackgroundColour(ModernColors::PRIMARY_GREEN);
    m_toggleButton->SetForegroundColour(ModernColors::TEXT_WHITE);
    wxFont buttonFont = m_toggleButton->GetFont();
    buttonFont.SetWeight(wxFONTWEIGHT_BOLD);
    buttonFont.SetFaceName(wxT("Segoe UI"));
    buttonFont.SetPointSize(10);
    m_toggleButton->SetFont(buttonFont);
    m_toggleButton->Bind(wxEVT_BUTTON, &MapPanel::OnTogglePopup, this);

    // Panel chứa nội dung chi tiết 
    m_routeContentPanel = new wxPanel(m_routeInfoPanel, wxID_ANY);
    m_routeContentPanel->SetBackgroundColour(ModernColors::BACKGROUND_CARD);

    // Tạo sizer cho nội dung
    wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);

    // Cột 1: Khoảng cách 
    wxBoxSizer* col1Sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* distTitle = new wxStaticText(m_routeContentPanel, wxID_ANY, _T("Khoảng cách"));
    distTitle->SetForegroundColour(ModernColors::TEXT_SECONDARY);
    wxFont titleFont = distTitle->GetFont();
    titleFont.SetFaceName(wxT("Segoe UI"));
    titleFont.SetPointSize(9);
    titleFont.SetWeight(wxFONTWEIGHT_MEDIUM);
    distTitle->SetFont(titleFont);

    m_distanceLabel = new wxStaticText(m_routeContentPanel, wxID_ANY, _T("0 km"));
    wxFont distFont = m_distanceLabel->GetFont();
    distFont.SetWeight(wxFONTWEIGHT_BOLD);
    distFont.SetPointSize(14);
    distFont.SetFaceName(wxT("Segoe UI"));
    m_distanceLabel->SetFont(distFont);
    m_distanceLabel->SetForegroundColour(ModernColors::PRIMARY_GREEN);

    col1Sizer->Add(distTitle, 0, wxBOTTOM, 2);
    col1Sizer->Add(m_distanceLabel, 0, 0, 0);

    // Cột 2: Giá Grab 
    wxBoxSizer* col2Sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* priceTitle = new wxStaticText(m_routeContentPanel, wxID_ANY, _T("Giá Grab"));
    priceTitle->SetForegroundColour(ModernColors::TEXT_SECONDARY);
    priceTitle->SetFont(titleFont);

    m_grabPriceLabel = new wxStaticText(m_routeContentPanel, wxID_ANY, _T("0 VNĐ"));
    wxFont priceFont = m_grabPriceLabel->GetFont();
    priceFont.SetWeight(wxFONTWEIGHT_BOLD);
    priceFont.SetPointSize(14);
    priceFont.SetFaceName(wxT("Segoe UI"));
    m_grabPriceLabel->SetFont(priceFont);
    m_grabPriceLabel->SetForegroundColour(ModernColors::ACCENT_GREEN);

    col2Sizer->Add(priceTitle, 0, wxBOTTOM, 2);
    col2Sizer->Add(m_grabPriceLabel, 0, 0, 0);

    // Cột 3: Thời gian (ước tính) 
    wxBoxSizer* col3Sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* timeTitle = new wxStaticText(m_routeContentPanel, wxID_ANY, _T("Thời gian"));
    timeTitle->SetForegroundColour(ModernColors::TEXT_SECONDARY);
    timeTitle->SetFont(titleFont);

    m_timeLabel = new wxStaticText(m_routeContentPanel, wxID_ANY, _T("0 phút"));
    wxFont timeFont = m_timeLabel->GetFont();
    timeFont.SetWeight(wxFONTWEIGHT_BOLD);
    timeFont.SetPointSize(14);
    timeFont.SetFaceName(wxT("Segoe UI"));
    m_timeLabel->SetFont(timeFont);
    m_timeLabel->SetForegroundColour(ModernColors::DARK_GREEN);

    col3Sizer->Add(timeTitle, 0, wxBOTTOM, 2);
    col3Sizer->Add(m_timeLabel, 0, 0, 0);

    contentSizer->Add(col1Sizer, 1, wxALL, 12);
    contentSizer->Add(col2Sizer, 1, wxTOP | wxBOTTOM | wxRIGHT, 12);
    contentSizer->Add(col3Sizer, 1, wxTOP | wxBOTTOM | wxRIGHT, 12);

    m_routeContentPanel->SetSizer(contentSizer);

    // Thêm border line để phân cách
    wxStaticLine* borderLine = new wxStaticLine(m_routeInfoPanel, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    borderLine->SetBackgroundColour(ModernColors::BORDER_COLOR);

    // Thêm vào sizer chính
    mainSizer->Add(m_toggleButton, 0, wxEXPAND | wxALL, 2);
    mainSizer->Add(borderLine, 0, wxEXPAND | wxLEFT | wxRIGHT, 2);
    mainSizer->Add(m_routeContentPanel, 1, wxEXPAND | wxALL, 2);

    m_routeInfoPanel->SetSizer(mainSizer);

    m_routeInfoPanel->Hide();
    m_routeContentPanel->Hide(); 
    m_showRouteInfo = false;     
    m_isExpanded = false;       
    m_currentHeight = m_collapsedHeight;
    m_targetHeight = m_collapsedHeight;

    m_popupHeight = 120;
}

// Hiển thị popup với thông tin mới
void MapPanel::ShowRouteInfo(double distanceMeters)
{
    if (!m_routeInfoPanel) return;

    // Tính toán thông tin
    double distanceKm = distanceMeters / 1000.0;
    double basePrice = 15000;
    double pricePerKm = 12000;
    double estimatedPrice = basePrice + (distanceKm * pricePerKm);
    double estimatedTimeMinutes = (distanceKm / 20.0) * 60.0;

    // Cập nhật text
    if (distanceKm >= 1.0) {
        m_distanceLabel->SetLabel(wxString::Format(wxT("%.2f km"), distanceKm));
    }
    else {
        m_distanceLabel->SetLabel(wxString::Format(wxT("%.0f m"), distanceMeters));
    }

    m_grabPriceLabel->SetLabel(wxString::Format(wxT("%.0f VNĐ"), estimatedPrice));
    m_timeLabel->SetLabel(wxString::Format(wxT("%.0f phút"), estimatedTimeMinutes));

    // Hiển thị popup ở trạng thái mở rộng từ đầu
    m_isExpanded = true;
    m_currentHeight = m_popupHeight;
    m_targetHeight = m_popupHeight;
    m_routeContentPanel->Show(); // Hiện content ngay

    UpdateToggleButtonText();
    UpdateRouteInfoPosition();
    m_routeInfoPanel->Show();
    m_showRouteInfo = true;
}

// Ẩn popup hoàn toàn
void MapPanel::HideRouteInfo()
{
    if (!m_showRouteInfo) return;

    // Ẩn Panel
    m_routeInfoPanel->Hide();
    m_showRouteInfo = false;

    // Dừng Timer nếu đang chạy
    if (m_slideTimer && m_slideTimer->IsRunning()) {
        m_slideTimer->Stop();
    }

    // Đặt lại trạng thái
    m_isExpanded = false;
    m_currentHeight = m_collapsedHeight;
    m_targetHeight = m_collapsedHeight;
}

// Xử lý nút toggle
void MapPanel::OnTogglePopup(wxCommandEvent& event)
{
    if (m_isExpanded) {
        CollapsePopup();
    }
    else {
        ExpandPopup();
    }
}

// Mở rộng popup
void MapPanel::ExpandPopup()
{
    m_isExpanded = true;
    m_targetHeight = m_popupHeight;
    UpdateToggleButtonText();

    // Hiện content panel
    m_routeContentPanel->Show();

    // Bắt đầu animation
    m_slideTimer->Start(16);
}

// Thu gọn popup
void MapPanel::CollapsePopup()
{
    m_isExpanded = false;
    m_targetHeight = m_collapsedHeight;
    UpdateToggleButtonText();

    // Bắt đầu animation
    m_slideTimer->Start(16);
}

// Cập nhật text và mũi tên của nút toggle
void MapPanel::UpdateToggleButtonText()
{
    if (m_toggleButton) {
        if (m_isExpanded) {
            m_toggleButton->SetLabel(wxT("▲ Thu gọn"));
            m_toggleButton->SetBackgroundColour(ModernColors::DARK_GREEN);
        }
        else {
            m_toggleButton->SetLabel(wxT("▼ Thông tin lộ trình"));
            m_toggleButton->SetBackgroundColour(ModernColors::PRIMARY_GREEN);
        }
        m_toggleButton->Refresh();
    }
}

// Animation timer 
void MapPanel::OnSlideTimer(wxTimerEvent& event)
{
    bool animationDone = false;

    // Tính toán chiều cao mới
    if (m_currentHeight < m_targetHeight) {
        m_currentHeight += 6; // Tốc độ mở rộng
        if (m_currentHeight >= m_targetHeight) {
            m_currentHeight = m_targetHeight;
            animationDone = true;
        }
    }
    else if (m_currentHeight > m_targetHeight) {
        m_currentHeight -= 6; // Tốc độ thu gọn
        if (m_currentHeight <= m_targetHeight) {
            m_currentHeight = m_targetHeight;
            animationDone = true;
        }
    }
    else {
        animationDone = true;
    }

    // Cập nhật kích thước panel
    UpdateRouteInfoPosition();

    // Ẩn content panel sau khi thu gọn xong
    if (animationDone) {
        if (!m_isExpanded) {
            m_routeContentPanel->Hide();
        }
        m_slideTimer->Stop();
    }

    // Refresh để vẽ lại
    m_routeInfoPanel->Refresh();
}

// Cập nhật vị trí popup 
void MapPanel::UpdateRouteInfoPosition()
{
    if (!m_routeInfoPanel) return;

    wxSize panelSize = GetSize();
    int panelWidth = 400;
    int panelX = (panelSize.x - panelWidth) / 2;

    // Vị trí Y cố định ở dưới màn hình
    m_popupY = panelSize.y - m_currentHeight - 0;

    m_routeInfoPanel->SetSize(panelX, m_popupY, panelWidth, m_currentHeight);
}

// Xử lý resize event 
void MapPanel::OnSize(wxSizeEvent& event)
{
    if (m_showRouteInfo) {
        UpdateRouteInfoPosition();
    }

    // Cập nhật scaled bitmap khi resize
    UpdateScaledBitmap();

    event.Skip();
}