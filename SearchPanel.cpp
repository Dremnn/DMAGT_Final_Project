#include "SearchPanel.h"
#include "MapPanel.h"
#include <wx/scrolwin.h>
#include <wx/statline.h>
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/button.h>
#include <wx/stattext.h>

SearchPanel::SearchPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(350, -1))
{
    // Cài đặt màu nền
    this->SetBackgroundColour(wxColour(255, 255, 255));

    // Khởi tạo các Sizer
    wxBoxSizer* sidebarSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* mainSearchSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* inputBoxSizer = new wxBoxSizer(wxVERTICAL);

    // Tạo các TextCtrl để nhập điểm đi/đến
    m_startPointCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_startPointCtrl->SetHint(_T("Nhập điểm đi"));

    m_endPointCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_endPointCtrl->SetHint(_T("Nhập điểm đến"));

    // Thêm TextCtrl vào Sizer
    inputBoxSizer->Add(m_startPointCtrl, 1, wxEXPAND | wxBOTTOM, 5);
    inputBoxSizer->Add(m_endPointCtrl, 1, wxEXPAND, 0);

    // Tạo nút tìm kiếm
    wxBitmapButton* searchButton = new wxBitmapButton(this, wxID_ANY, wxBitmap(_T("search.png"), wxBITMAP_TYPE_PNG));

    // Thêm các thành phần tìm kiếm vào mainSizer
    mainSearchSizer->Add(inputBoxSizer, 1, wxEXPAND | wxRIGHT, 10);
    mainSearchSizer->Add(searchButton, 0, wxALIGN_CENTER_VERTICAL);
    sidebarSizer->Add(mainSearchSizer, 0, wxEXPAND | wxALL, 10);

    // Tạo Sizer cho các nút chức năng
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* allPathsButton = new wxButton(this, wxID_ANY, _T("Hiển thị tất cả đường đi"));
    wxButton* clearButton = new wxButton(this, wxID_ANY, _T("Xóa các đường đi"));

    buttonSizer->Add(allPathsButton, 1, wxEXPAND | wxRIGHT, 5);
    buttonSizer->Add(clearButton, 1, wxEXPAND | wxLEFT, 5);
    sidebarSizer->Add(buttonSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Thêm đường kẻ phân cách
    sidebarSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 10);

    // Tiêu đề cho các địa điểm đã lưu
    wxStaticText* resultsTitle = new wxStaticText(this, wxID_ANY, _T("Saved Places"));
    wxFont font = resultsTitle->GetFont();
    font.SetPointSize(12);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    resultsTitle->SetFont(font);
    sidebarSizer->Add(resultsTitle, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Tạo cửa sổ cuộn để hiển thị danh sách địa điểm
    wxScrolledWindow* scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    scrolledWindow->SetScrollRate(0, 5);

    // Sizer cho danh sách địa điểm
    wxBoxSizer* resultsSizer = new wxBoxSizer(wxVERTICAL);
    std::vector<std::pair<wxString, wxString>> savedPlaces = {
        {_T("Dinh Thống Nhất"), _T("135 Nam Kỳ Khởi Nghĩa, Bến Thành, Quận 1")},
        {_T("Nhà Thờ Đức Bà"), _T("01 Công Xã Paris, Bến Nghé, Quận 1")},
        {_T("Bưu Điện Trung Tâm"), _T("02 Công Xã Paris, Bến Nghé, Quận 1")},
        {_T("Chợ Bến Thành"), _T("Đ. Lê Lợi, Bến Thành, Quận 1")},
        {_T("Phố Đi Bộ Nguyễn Huệ"), _T("Nguyễn Huệ, Bến Nghé, Quận 1")},
        {_T("Bitexco Financial Tower"), _T("02 Hải Triều, Bến Nghé, Quận 1")}
    };

    for (const auto& place : savedPlaces) {
        wxStaticText* nameText = new wxStaticText(scrolledWindow, wxID_ANY, place.first);
        wxStaticText* addressText = new wxStaticText(scrolledWindow, wxID_ANY, place.second);
        wxFont nameFont = nameText->GetFont();
        nameFont.SetWeight(wxFONTWEIGHT_BOLD);
        nameText->SetFont(nameFont);
        resultsSizer->Add(nameText, 0, wxEXPAND | wxBOTTOM, 2);
        resultsSizer->Add(addressText, 0, wxEXPAND | wxBOTTOM, 8);
        resultsSizer->Add(new wxStaticLine(scrolledWindow), 0, wxEXPAND | wxBOTTOM, 8);
    }
    scrolledWindow->SetSizer(resultsSizer);
    sidebarSizer->Add(scrolledWindow, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);
    this->SetSizer(sidebarSizer);

    // Gắn sự kiện cho các nút
    searchButton->Bind(wxEVT_BUTTON, &SearchPanel::OnSearchClicked, this);
    allPathsButton->Bind(wxEVT_BUTTON, &SearchPanel::OnShowAllPathsClicked, this);
    clearButton->Bind(wxEVT_BUTTON, &SearchPanel::OnClearClicked, this);
}

// Hàm để gán con trỏ MapPanel
void SearchPanel::SetMapPanel(MapPanel* mapPanel)
{
    m_mapPanel = mapPanel;
}

void SearchPanel::OnSearchClicked(wxCommandEvent& event)
{
    // Kiểm tra xem MapPanel đã được gán chưa
    if (!m_mapPanel) {
        wxLogError("Lỗi: MapPanel chưa được gán.");
        return;
    }

    // Lấy tên địa điểm từ TextCtrl
    wxString startName = m_startPointCtrl->GetValue();
    wxString endName = m_endPointCtrl->GetValue();

    // Tìm chỉ số node tương ứng với tên địa điểm
    int startIndex = m_mapPanel->FindNodeIndexByName(startName);
    int endIndex = m_mapPanel->FindNodeIndexByName(endName);

    // Kiểm tra tính hợp lệ của input
    if (startIndex == -1 || endIndex == -1) {
        wxMessageBox(_T("Vui lòng nhập một hoặc cả hai địa điểm hợp lệ."));
        return;
    }

    // Gọi hàm tìm và vẽ đường đi ngắn nhất trên MapPanel
    m_mapPanel->FindAndDrawNewPath(startIndex, endIndex);
}

void SearchPanel::OnShowAllPathsClicked(wxCommandEvent& event)
{
    if (!m_mapPanel) {
        wxLogError("Lỗi: MapPanel chưa được gán.");
        return;
    }

    wxString startName = m_startPointCtrl->GetValue();
    wxString endName = m_endPointCtrl->GetValue();

    int startIndex = m_mapPanel->FindNodeIndexByName(startName);
    int endIndex = m_mapPanel->FindNodeIndexByName(endName);

    if (startIndex == -1 || endIndex == -1) {
        wxMessageBox(_T("Vui lòng nhập một hoặc cả hai địa điểm hợp lệ."));
        return;
    }

    // Gọi hàm tìm và vẽ tất cả đường đi trên MapPanel
    m_mapPanel->FindAndDrawAllPaths(startIndex, endIndex);
}

void SearchPanel::OnClearClicked(wxCommandEvent& event)
{
    if (m_mapPanel) {
        m_mapPanel->ClearAllPaths();
    }
}