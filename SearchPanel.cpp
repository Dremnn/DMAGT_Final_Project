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
    for (int i = 1; i <= 15; ++i) {
        wxString placeName = wxString::Format("Dinh Doc Lap, District %d", i);
        wxString placeAddress = "135 Nam Ky Khoi Nghia, Ben Thanh";

        wxStaticText* nameText = new wxStaticText(scrolledWindow, wxID_ANY, placeName);
        wxStaticText* addressText = new wxStaticText(scrolledWindow, wxID_ANY, placeAddress);

        wxFont nameFont = nameText->GetFont();
        nameFont.SetWeight(wxFONTWEIGHT_BOLD);
        nameText->SetFont(nameFont);

        wxFont addressFont = addressText->GetFont();
        addressFont.SetStyle(wxFONTSTYLE_ITALIC);
        addressText->SetFont(addressFont);

        resultsSizer->Add(nameText, 0, wxEXPAND | wxBOTTOM, 5);
        resultsSizer->Add(addressText, 0, wxEXPAND | wxBOTTOM, 5);
        resultsSizer->Add(new wxStaticLine(scrolledWindow), 0, wxEXPAND | wxBOTTOM, 10);
    }
    scrolledWindow->SetSizer(resultsSizer);
    sidebarSizer->Add(scrolledWindow, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);


    this->SetSizer(sidebarSizer);
}