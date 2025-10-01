#include "SearchPanel.h"
#include "MapPanel.h"
#include "ModernColors.h"
#include "SimpleUIHelper.h"
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/statbmp.h>
#include <wx/popupwin.h> // Thêm thư viện cho wxPopupWindow

using namespace std;

SearchPanel::SearchPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(350, -1))
{
    SetupModernUI();
}

void SearchPanel::SetupModernUI()
{
    SetBackgroundColour(ModernColors::BACKGROUND_CARD);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    CreateSearchSection(mainSizer);
    CreateButtonSection(mainSizer);
    CreateSavedPlacesSection(mainSizer);
    SetSizer(mainSizer);
}

void SearchPanel::CreateSearchSection(wxBoxSizer* mainSizer)
{
    wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);

    wxImage logoImage("grab_logo.png", wxBITMAP_TYPE_PNG);
    if (logoImage.IsOk()) {
        int newHeight = 35;
        int newWidth = logoImage.GetWidth() * newHeight / logoImage.GetHeight();
        logoImage.Rescale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);
        wxStaticBitmap* logoBitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(logoImage));
        headerSizer->Add(logoBitmap, 0, wxALIGN_CENTER_VERTICAL);
    }
    else {
        wxStaticText* title = new wxStaticText(this, wxID_ANY, _T("Grab Routes"));
        wxFont titleFont = title->GetFont();
        titleFont.SetPointSize(16);
        titleFont.SetWeight(wxFONTWEIGHT_BOLD);
        titleFont.SetFaceName("Segoe UI");
        title->SetFont(titleFont);
        title->SetForegroundColour(ModernColors::TEXT_PRIMARY);
        headerSizer->Add(title, 1, wxALIGN_CENTER_VERTICAL);
    }

    headerSizer->AddStretchSpacer(1);

    // --- BẮT ĐẦU THAY ĐỔI ---

    // Sử dụng biến thành viên m_infoButton
    m_infoButton = new wxButton(this, wxID_ANY, wxT("i"), wxDefaultPosition, wxSize(30, 30));
    m_infoButton->SetFont(m_infoButton->GetFont().Bold());
    m_infoButton->SetBackgroundColour(ModernColors::BACKGROUND_INPUT);
    m_infoButton->SetForegroundColour(ModernColors::TEXT_SECONDARY);

    // Xóa SetToolTip và thay bằng bind sự kiện hover
    // m_infoButton->SetToolTip(memberInfo); // Dòng này bị xóa
    m_infoButton->Bind(wxEVT_ENTER_WINDOW, &SearchPanel::OnInfoButtonEnter, this);
    m_infoButton->Bind(wxEVT_LEAVE_WINDOW, &SearchPanel::OnInfoButtonLeave, this);

    // --- KẾT THÚC THAY ĐỔI ---

    headerSizer->Add(m_infoButton, 0, wxALIGN_CENTER_VERTICAL);
    mainSizer->Add(headerSizer, 0, wxEXPAND | wxALL, 16);

    wxBoxSizer* inputSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* startLabel = new wxStaticText(this, wxID_ANY, _T("Điểm đi:"));
    SimpleUIHelper::SetModernFont(startLabel);
    startLabel->SetForegroundColour(ModernColors::TEXT_SECONDARY);

    m_startPointCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_startPointCtrl->SetHint(_T("Nhập điểm đi"));
    SimpleUIHelper::StyleTextCtrl(m_startPointCtrl);

    wxStaticText* endLabel = new wxStaticText(this, wxID_ANY, _T("Điểm đến:"));
    SimpleUIHelper::SetModernFont(endLabel);
    endLabel->SetForegroundColour(ModernColors::TEXT_SECONDARY);

    m_endPointCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_endPointCtrl->SetHint(_T("Nhập điểm đến"));
    SimpleUIHelper::StyleTextCtrl(m_endPointCtrl);

    inputSizer->Add(startLabel, 0, wxBOTTOM, 4);
    inputSizer->Add(m_startPointCtrl, 0, wxEXPAND | wxBOTTOM, 12);
    inputSizer->Add(endLabel, 0, wxBOTTOM, 4);
    inputSizer->Add(m_endPointCtrl, 0, wxEXPAND, 0);

    mainSizer->Add(inputSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 16);
}

// ... (Các hàm CreateButtonSection, CreateSavedPlacesSection, SetMapPanel, ... giữ nguyên)
void SearchPanel::CreateButtonSection(wxBoxSizer* mainSizer)
{
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);

    // Primary search button
    wxButton* searchButton = new wxButton(this, wxID_ANY, _T("🔍 Tìm đường đi"));
    SimpleUIHelper::StyleButton(searchButton, true); // Primary style

    // Secondary buttons
    wxButton* allPathsButton = new wxButton(this, wxID_ANY, _T("Tất cả đường đi"));
    SimpleUIHelper::StyleButton(allPathsButton, false); // Secondary style

    wxButton* clearButton = new wxButton(this, wxID_ANY, _T("Xóa đường đi"));
    clearButton->SetBackgroundColour(ModernColors::DANGER);
    clearButton->SetForegroundColour(ModernColors::TEXT_WHITE);
    SimpleUIHelper::SetModernFont(clearButton);
    clearButton->SetMinSize(wxSize(-1, 36));

    // Layout buttons
    buttonSizer->Add(searchButton, 0, wxEXPAND | wxBOTTOM, 8);
    buttonSizer->Add(allPathsButton, 0, wxEXPAND | wxBOTTOM, 8);
    buttonSizer->Add(clearButton, 0, wxEXPAND, 0);

    // Add divider line
    wxStaticLine* divider = new wxStaticLine(this, wxID_ANY, wxDefaultPosition,
        wxSize(-1, 1), wxLI_HORIZONTAL);

    // Add to main sizer
    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 16);
    mainSizer->Add(divider, 0, wxEXPAND | wxLEFT | wxRIGHT, 16);

    // Bind button events
    searchButton->Bind(wxEVT_BUTTON, &SearchPanel::OnSearchClicked, this);
    allPathsButton->Bind(wxEVT_BUTTON, &SearchPanel::OnShowAllPathsClicked, this);
    clearButton->Bind(wxEVT_BUTTON, &SearchPanel::OnClearClicked, this);
}

void SearchPanel::CreateSavedPlacesSection(wxBoxSizer* mainSizer)
{
    // Section title
    wxStaticText* placesTitle = new wxStaticText(this, wxID_ANY, _T("Địa điểm đã lưu"));
    wxFont titleFont = placesTitle->GetFont();
    titleFont.SetPointSize(12);
    titleFont.SetWeight(wxFONTWEIGHT_BOLD);
    titleFont.SetFaceName("Segoe UI");
    placesTitle->SetFont(titleFont);
    placesTitle->SetForegroundColour(ModernColors::TEXT_PRIMARY);

    // Scrolled window for places list
    wxScrolledWindow* scrolledWindow = new wxScrolledWindow(this, wxID_ANY,
        wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    scrolledWindow->SetScrollRate(0, 5);
    scrolledWindow->SetBackgroundColour(ModernColors::BACKGROUND_CARD);

    // Container for saved places
    wxBoxSizer* placesSizer = new wxBoxSizer(wxVERTICAL);

    // Sample saved places data
    vector<pair<wxString, wxString>> savedPlaces = {
        {_T("Dinh Độc Lập"), _T("135 Nam Kỳ Khởi Nghĩa, Bến Thành, Quận 1")},
        {_T("Nhà Thờ Đức Bà"), _T("01 Công Xã Paris, Bến Nghé, Quận 1")},
        {_T("Bưu Điện Trung Tâm"), _T("02 Công Xã Paris, Bến Nghé, Quận 1")},
        {_T("Chợ Bến Thành"), _T("Đ. Lê Lợi, Bến Thành, Quận 1")},
        {_T("Phố Đi Bộ Nguyễn Huệ"), _T("Nguyễn Huệ, Bến Nghé, Quận 1")},
        {_T("Bitexco Financial Tower"), _T("02 Hải Triều, Bến Nghé, Quận 1")}
    };

    // Create place items with modern styling
    for (const auto& place : savedPlaces) {
        // Container panel for each place
        wxPanel* placePanel = new wxPanel(scrolledWindow);
        placePanel->SetBackgroundColour(ModernColors::BACKGROUND_INPUT);

        // Place name (bold)
        wxStaticText* nameText = new wxStaticText(placePanel, wxID_ANY, place.first);
        wxFont nameFont = nameText->GetFont();
        nameFont.SetWeight(wxFONTWEIGHT_BOLD);
        nameFont.SetFaceName("Segoe UI");
        nameText->SetFont(nameFont);
        nameText->SetForegroundColour(ModernColors::TEXT_PRIMARY);

        // Place address (regular)
        wxStaticText* addressText = new wxStaticText(placePanel, wxID_ANY, place.second);
        wxFont addressFont = addressText->GetFont();
        addressFont.SetFaceName("Segoe UI");
        addressFont.SetPointSize(addressFont.GetPointSize() - 1);
        addressText->SetFont(addressFont);
        addressText->SetForegroundColour(ModernColors::TEXT_SECONDARY);

        // Layout for place item
        wxBoxSizer* placeSizer = new wxBoxSizer(wxVERTICAL);
        placeSizer->Add(nameText, 0, wxEXPAND | wxALL, 8);
        placeSizer->Add(addressText, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);
        placePanel->SetSizer(placeSizer);

        // Add to places list
        placesSizer->Add(placePanel, 0, wxEXPAND | wxBOTTOM, 4);

        // Add subtle separator except for last item
        if (&place != &savedPlaces.back()) {
            wxStaticLine* separator = new wxStaticLine(scrolledWindow, wxID_ANY,
                wxDefaultPosition, wxSize(-1, 1));
            placesSizer->Add(separator, 0, wxEXPAND | wxBOTTOM, 4);
        }
    }

    scrolledWindow->SetSizer(placesSizer);

    // Add to main sizer
    mainSizer->Add(placesTitle, 0, wxALL, 16);
    mainSizer->Add(scrolledWindow, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 16);
}

void SearchPanel::SetMapPanel(MapPanel* mapPanel)
{
    m_mapPanel = mapPanel;
}

void SearchPanel::OnSearchClicked(wxCommandEvent& event)
{
    // Clear all the paths before searching
    m_mapPanel->ClearAllPaths();

    if (!m_mapPanel) {
        wxLogError(_T("Lỗi: MapPanel chưa được gán."));
        return;
    }

    // Get input values
    wxString startName = m_startPointCtrl->GetValue();
    wxString endName = m_endPointCtrl->GetValue();

    // Validate inputs
    if (startName.IsEmpty() || endName.IsEmpty()) {
        wxMessageBox(_T("Vui lòng nhập cả điểm đi và điểm đến."),
            _T("Thông tin không đầy đủ"), wxOK | wxICON_WARNING);
        return;
    }

    // Find node indices
    int startIndex = m_mapPanel->FindNodeIndexByName(startName);
    int endIndex = m_mapPanel->FindNodeIndexByName(endName);

    if (startIndex == -1 || endIndex == -1) {
        wxString message = _T("Không tìm thấy địa điểm:\n");
        if (startIndex == -1) message += _T("- ") + startName + _T("\n");
        if (endIndex == -1) message += _T("- ") + endName + _T("\n");
        message += _T("\nVui lòng kiểm tra lại tên địa điểm.");

        wxMessageBox(message, _T("Địa điểm không tồn tại"), wxOK | wxICON_ERROR);
        return;
    }

    // Find and display shortest path
    m_mapPanel->FindAndDrawNewPath(startIndex, endIndex);
}

void SearchPanel::OnShowAllPathsClicked(wxCommandEvent& event)
{
    if (!m_mapPanel) {
        wxLogError(_T("Lỗi: MapPanel chưa được gán."));
        return;
    }

    wxString startName = m_startPointCtrl->GetValue();
    wxString endName = m_endPointCtrl->GetValue();

    if (startName.IsEmpty() || endName.IsEmpty()) {
        wxMessageBox(_T("Vui lòng nhập cả điểm đi và điểm đến."),
            _T("Thông tin không đầy đủ"), wxOK | wxICON_WARNING);
        return;
    }

    int startIndex = m_mapPanel->FindNodeIndexByName(startName);
    int endIndex = m_mapPanel->FindNodeIndexByName(endName);

    if (startIndex == -1 || endIndex == -1) {
        wxString message = _T("Không tìm thấy địa điểm:\n");
        if (startIndex == -1) message += _T("- ") + startName + _T("\n");
        if (endIndex == -1) message += _T("- ") + endName + _T("\n");
        message += _T("\nVui lòng kiểm tra lại tên địa điểm.");

        wxMessageBox(message, _T("Địa điểm không tồn tại"), wxOK | wxICON_ERROR);
        return;
    }

    // Find and display all paths
    m_mapPanel->FindAndDrawAllPaths(startIndex, endIndex);
}

void SearchPanel::OnClearClicked(wxCommandEvent& event)
{
    if (m_mapPanel) {
        m_mapPanel->ClearAllPaths();
    }

    // Clear input fields
    m_startPointCtrl->Clear();
    m_endPointCtrl->Clear();
}

// --- CÁC HÀM XỬ LÝ POPUP MỚI ---
void SearchPanel::OnInfoButtonEnter(wxMouseEvent& event)
{
    // Hủy popup cũ nếu có để tránh trùng lặp
    if (m_infoPopup) {
        m_infoPopup->Destroy();
        m_infoPopup = nullptr;
    }

    // Tạo cửa sổ popup mới
    m_infoPopup = new wxPopupWindow(this, wxBORDER_SIMPLE);
    m_infoPopup->SetBackgroundColour(ModernColors::BACKGROUND_CARD);

    // Tạo nội dung text cho popup
    wxString memberInfo =
        _T("THÀNH VIÊN NHÓM\n")
        _T("1. Lý Trần Gia Khang - MSSV: 24110098\n")
        _T("2. Đoàn Trọng Trung - MSSV: 24110140\n")
        _T("3. Khổng Đình Tú - MSSV: 24110145");

    wxStaticText* infoText = new wxStaticText(m_infoPopup, wxID_ANY, memberInfo);
    infoText->SetForegroundColour(ModernColors::TEXT_PRIMARY);

    // **QUAN TRỌNG: Set font Unicode để hiển thị tiếng Việt**
    wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Segoe UI");
    infoText->SetFont(font);

    // Tự động điều chỉnh kích thước popup cho vừa với nội dung
    wxSize bestSize = infoText->GetBestSize();
    m_infoPopup->SetSize(bestSize.GetWidth() + 20, bestSize.GetHeight() + 20); // Thêm padding

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(infoText, 1, wxEXPAND | wxALL, 10);
    m_infoPopup->SetSizer(sizer);

    // Tính toán vị trí hiển thị popup (bên phải nút "i")
    wxPoint pos = m_infoButton->ClientToScreen(wxPoint(0, 0));
    wxSize btnSize = m_infoButton->GetSize();
    pos.x += btnSize.GetWidth() + 5; // Cách nút 5px
    pos.y -= (m_infoPopup->GetSize().GetHeight() - btnSize.GetHeight()) / 2; // Canh giữa theo chiều dọc

    m_infoPopup->SetPosition(pos);
    m_infoPopup->Show();
}

void SearchPanel::OnInfoButtonLeave(wxMouseEvent& event)
{
    // Nếu có popup đang hiển thị thì hủy nó đi
    if (m_infoPopup) {
        m_infoPopup->Destroy();
        m_infoPopup = nullptr;
    }
}