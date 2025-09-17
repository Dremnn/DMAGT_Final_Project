#include "App.h"
#include "MainFrame.h"

// Macro để wxWidgets biết đâu là điểm khởi đầu
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    // Cần thêm các handler cho loại ảnh bạn muốn dùng
    wxImage::AddHandler(new wxPNGHandler);

    MainFrame* frame = new MainFrame("Google Maps UI Mock-up");
    frame->SetSize(1200, 800); // Đặt kích thước cửa sổ ban đầu
    frame->Center();
    frame->Show(true);
    return true;
}