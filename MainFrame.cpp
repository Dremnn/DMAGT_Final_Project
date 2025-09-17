#include "MainFrame.h"
#include "SearchPanel.h"
#include "MapPanel.h"

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title)
{
    // Panel chính để chứa mọi thứ
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);

    // Layout chính: Sizer ngang chia cửa sổ thành 2 phần: sidebar và map
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // 1. Tạo sidebar bên trái
    SearchPanel* searchPanel = new SearchPanel(mainPanel);
    // Thêm sidebar vào sizer, không co giãn (proportion = 0)
    mainSizer->Add(searchPanel, 0, wxEXPAND | wxALL, 5);

    // 2. Tạo khu vực bản đồ bên phải
    MapPanel* mapPanel = new MapPanel(mainPanel);
    // Thêm map vào sizer, cho phép co giãn để lấp đầy không gian (proportion = 1)
    mainSizer->Add(mapPanel, 1, wxEXPAND | wxALL, 5);

    mainPanel->SetSizer(mainSizer);
}