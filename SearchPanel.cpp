#include "SearchPanel.h"
#include <wx/scrolwin.h>
#include <wx/statline.h>

SearchPanel::SearchPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(300, -1)) // Cố định chiều rộng
{
    this->SetBackgroundColour(wxColour(255, 255, 255));

    // Sizer chính cho sidebar: Sizer dọc
    wxBoxSizer* sidebarSizer = new wxBoxSizer(wxVERTICAL);

    // ---- Phần 1: Thanh tìm kiếm ----
    wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSearchCtrl* searchCtrl = new wxSearchCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxBitmapButton* searchButton = new wxBitmapButton(this, wxID_ANY, wxBitmap("search.png", wxBITMAP_TYPE_PNG));
    searchSizer->Add(searchCtrl, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
    searchSizer->Add(searchButton, 0, wxALIGN_CENTER_VERTICAL, 0);
    sidebarSizer->Add(searchSizer, 0, wxEXPAND | wxALL, 10);

    // ---- Phần 2: Các nút chức năng ----
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(new wxBitmapButton(this, wxID_ANY, wxBitmap("directions.png", wxBITMAP_TYPE_PNG)), 1, wxEXPAND | wxRIGHT, 5);
    buttonSizer->Add(new wxBitmapButton(this, wxID_ANY, wxBitmap("nearby.png", wxBITMAP_TYPE_PNG)), 1, wxEXPAND | wxRIGHT, 5);
    buttonSizer->Add(new wxBitmapButton(this, wxID_ANY, wxBitmap("save.png", wxBITMAP_TYPE_PNG)), 1, wxEXPAND, 0);
    sidebarSizer->Add(buttonSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // ---- Đường kẻ ngang phân cách ----
    sidebarSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 10);

    // ---- Phần 3: Khu vực kết quả (có thể cuộn) ----
    wxStaticText* resultsTitle = new wxStaticText(this, wxID_ANY, "Recent Places");
    wxFont font = resultsTitle->GetFont();
    font.SetPointSize(12);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    resultsTitle->SetFont(font);
    sidebarSizer->Add(resultsTitle, 0, wxLEFT | wxRIGHT, 10);

    wxScrolledWindow* scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    scrolledWindow->SetScrollRate(0, 5); // Tốc độ cuộn

    wxBoxSizer* resultsSizer = new wxBoxSizer(wxVERTICAL);
    // Danh sách cố định các địa danh nổi tiếng ở Quận 1
    static const std::vector<std::pair<wxString, wxString>> famousPlacesQ1 = {
        { "Dinh Độc Lập", "135 Nam Kỳ Khởi Nghĩa, Bến Thành, Quận 1" },
        { "Nhà Thờ Đức Bà", "01 Công Xã Paris, Bến Nghé, Quận 1" },
        { "Bưu Điện Trung Tâm", "02 Công Xã Paris, Bến Nghé, Quận 1" },
        { "Chợ Bến Thành", "Lê Lợi, Bến Thành, Quận 1" },
        { "Phố đi bộ Nguyễn Huệ", "Nguyễn Huệ, Bến Nghé, Quận 1" },
        { "Bitexco Financial Tower", "02 Hải Triều, Bến Nghé, Quận 1" },
        { "Nhà hát Thành Phố", "07 Công Trường Lam Sơn, Bến Nghé, Quận 1" },
        { "Công viên 23/9", "Phạm Ngũ Lão, Quận 1" },
        { "Bảo tàng Mỹ thuật", "97 Phó Đức Chính, Nguyễn Thái Bình, Quận 1" }
    };

    // 👉 Hàm tiện ích để hiển thị danh sách (hoặc kết quả search)
    auto showPlaces = [&](const wxString& filter = "") {
        resultsSizer->Clear(true); // Xóa nội dung cũ

        for (const auto& place : famousPlacesQ1) {
            // Nếu có filter -> chỉ hiển thị kết quả phù hợp
            if (!filter.IsEmpty()) {
                if (place.first.Lower().Find(filter.Lower()) == wxNOT_FOUND &&
                    place.second.Lower().Find(filter.Lower()) == wxNOT_FOUND) {
                    continue; // bỏ qua nếu không match
                }
            }

            wxStaticText* nameText = new wxStaticText(scrolledWindow, wxID_ANY, place.first);
            wxStaticText* addressText = new wxStaticText(scrolledWindow, wxID_ANY, place.second);

            // Định dạng font
            wxFont nameFont = nameText->GetFont();
            nameFont.SetWeight(wxFONTWEIGHT_BOLD);
            nameText->SetFont(nameFont);

            wxFont addrFont = addressText->GetFont();
            addrFont.SetStyle(wxFONTSTYLE_ITALIC);
            addressText->SetFont(addrFont);

            resultsSizer->Add(nameText, 0, wxEXPAND | wxBOTTOM, 5);
            resultsSizer->Add(addressText, 0, wxEXPAND | wxBOTTOM, 5);
            resultsSizer->Add(new wxStaticLine(scrolledWindow), 0, wxEXPAND | wxBOTTOM, 10);
        }

        scrolledWindow->Layout();
        };

    // 👉 Lần đầu load: hiển thị toàn bộ danh sách
    showPlaces();

    // 👉 Bắt sự kiện Enter trong ô search
    searchCtrl->Bind(wxEVT_TEXT_ENTER, [=](wxCommandEvent& e) {
        wxString query = e.GetString();
        showPlaces(query);
        });

    scrolledWindow->SetSizer(resultsSizer);
    sidebarSizer->Add(scrolledWindow, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);


    this->SetSizer(sidebarSizer);
}