#include "SearchPanel.h"
#include "MapPanel.h"
#include <wx/scrolwin.h>
#include <wx/statline.h>

SearchPanel::SearchPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(350, -1))
{
    this->SetBackgroundColour(wxColour(255, 255, 255));

    wxBoxSizer* sidebarSizer = new wxBoxSizer(wxVERTICAL);

    // 1. Sizer chính (ngang) để chứa cụm ô nhập và nút search
    wxBoxSizer* mainSearchSizer = new wxBoxSizer(wxHORIZONTAL);

    // 2. Sizer dọc, chỉ để chứa 2 ô nhập liệu
    wxBoxSizer* inputBoxSizer = new wxBoxSizer(wxVERTICAL);

    // Khởi tạo các biến thành viên để lưu trữ con trỏ đến các ô nhập liệu
    m_startPointCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_startPointCtrl->SetHint(_T("Nhập điểm đi của bạn"));

    m_endPointCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_endPointCtrl->SetHint(_T("Nhập điểm đến"));

    inputBoxSizer->Add(m_startPointCtrl, 1, wxEXPAND | wxBOTTOM, 5);
    inputBoxSizer->Add(m_endPointCtrl, 1, wxEXPAND, 0);

    // 3. Nút tìm kiếm bằng icon
    wxBitmapButton* searchButton = new wxBitmapButton(this, wxID_ANY, wxBitmap(_T("search.png"), wxBITMAP_TYPE_PNG));

    mainSearchSizer->Add(inputBoxSizer, 1, wxEXPAND | wxRIGHT, 10);
    mainSearchSizer->Add(searchButton, 0, wxALIGN_CENTER_VERTICAL);

    sidebarSizer->Add(mainSearchSizer, 0, wxEXPAND | wxALL, 10);
    sidebarSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 10);

    wxStaticText* resultsTitle = new wxStaticText(this, wxID_ANY, _T("Saved Places"));
    wxFont font = resultsTitle->GetFont();
    font.SetPointSize(12);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    resultsTitle->SetFont(font);
    sidebarSizer->Add(resultsTitle, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

    wxScrolledWindow* scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    scrolledWindow->SetScrollRate(0, 5);

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

    // Binding the button event to the handler function
    searchButton->Bind(wxEVT_BUTTON, &SearchPanel::OnSearchClicked, this);
}

// Hàm để gán con trỏ MapPanel
void SearchPanel::SetMapPanel(MapPanel* mapPanel)
{
    m_mapPanel = mapPanel;
}

// Hàm xử lý khi người dùng nhấn nút tìm kiếm
void SearchPanel::OnSearchClicked(wxCommandEvent& event)
{
    if (!m_mapPanel) {
        wxLogError("Lỗi: MapPanel chưa được gán.");
        return;
    }

    wxString startName = m_startPointCtrl->GetValue();
    wxString endName = m_endPointCtrl->GetValue();

    // Tìm chỉ số của các node dựa trên tên
    int startIndex = m_mapPanel->FindNodeIndexByName(startName);
    int endIndex = m_mapPanel->FindNodeIndexByName(endName);

    if (startIndex == -1 || endIndex == -1) {
        wxMessageBox(_T("Vui lòng nhập một hoặc cả hai địa điểm không hợp lệ. Chỉ chấp nhận các địa điểm lớn."));
        return;
    }

    // Gọi hàm trong MapPanel để tìm và vẽ đường đi mới
    m_mapPanel->FindAndDrawNewPath(startIndex, endIndex);
}