#include "MapPanel.h"

MapPanel::MapPanel(wxWindow* parent)
    : wxPanel(parent)
{
    // Đặt màu nền để tượng trưng cho bản đồ
    this->SetBackgroundColour(wxColour(204, 229, 255)); // Màu xanh nhạt

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* mapLabel = new wxStaticText(this, wxID_ANY, "Vietnam's Demo Map");

    wxFont font = mapLabel->GetFont();
    font.SetPointSize(24);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    mapLabel->SetFont(font);
    mapLabel->SetForegroundColour(*wxLIGHT_GREY);

    sizer->Add(mapLabel, 1, wxCENTER);
    this->SetSizer(sizer);
}