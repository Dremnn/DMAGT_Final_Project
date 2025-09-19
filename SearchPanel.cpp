#include "SearchPanel.h"
#include <wx/scrolwin.h>
#include <wx/statline.h>

SearchPanel::SearchPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(350, -1))
{
    this->SetBackgroundColour(wxColour(255, 255, 255));

    wxBoxSizer* sidebarSizer = new wxBoxSizer(wxVERTICAL);

    // --- PHẦN TÌM KIẾM ĐÃ ĐƯỢC THIẾT KẾ LẠI ---

    // 1. Sizer chính (ngang) để chứa cụm ô nhập và nút search
    wxBoxSizer* mainSearchSizer = new wxBoxSizer(wxHORIZONTAL);

    // 2. Sizer dọc, chỉ để chứa 2 ô nhập liệu
    wxBoxSizer* inputBoxSizer = new wxBoxSizer(wxVERTICAL);

    // Ô nhập điểm đi (Start)
    wxTextCtrl* startPointCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    startPointCtrl->SetHint(_T("Nhập điểm đi của bạn"));

    // Ô nhập điểm đến (End)
    wxTextCtrl* endPointCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    endPointCtrl->SetHint(_T("Nhập điểm đến"));

    // Thêm 2 ô nhập vào sizer dọc của chúng
    // Dùng proportion = 1 để chúng có chiều cao bằng nhau và lấp đầy không gian
    inputBoxSizer->Add(startPointCtrl, 1, wxEXPAND | wxBOTTOM, 5);
    inputBoxSizer->Add(endPointCtrl, 1, wxEXPAND, 0);

    // 3. Nút tìm kiếm bằng icon
    wxBitmapButton* searchButton = new wxBitmapButton(this, wxID_ANY, wxBitmap(_T("search.png"), wxBITMAP_TYPE_PNG));

    // 4. Thêm sizer chứa ô nhập và nút search vào sizer chính
    // Cho cụm ô nhập co giãn theo chiều rộng
    mainSearchSizer->Add(inputBoxSizer, 1, wxEXPAND | wxRIGHT, 10);
    // Căn nút search vào giữa theo chiều dọc
    mainSearchSizer->Add(searchButton, 0, wxALIGN_CENTER_VERTICAL);

    sidebarSizer->Add(mainSearchSizer, 0, wxEXPAND | wxALL, 10);

    // --- KẾT THÚC PHẦN THAY ĐỔI ---

    // Các phần còn lại của sidebar giữ nguyên
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
}