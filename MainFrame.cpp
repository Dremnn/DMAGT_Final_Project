#include "MainFrame.h"
#include "SearchPanel.h"
#include "MapPanel.h"

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title)
{
    // Panel chính để chứa mọi thứ
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY);

    // Layout chính: Sizer ngang chia cửa sổ thành 2 phần
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // 1. Tạo sidebar bên trái (CHỈ TẠO 1 LẦN)
    SearchPanel* searchPanel = new SearchPanel(mainPanel);

    // 2. Tạo khu vực bản đồ bên phải (CHỈ TẠO 1 LẦN)
    MapPanel* mapPanel = new MapPanel(mainPanel);

    // Liên kết hai panel
    searchPanel->SetMapPanel(mapPanel);

    // Thêm vào sizer
    mainSizer->Add(searchPanel, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(mapPanel, 1, wxEXPAND | wxALL, 5);

    mainPanel->SetSizer(mainSizer);
}