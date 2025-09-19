#pragma once
#include <wx/wx.h>
#include <wx/srchctrl.h> // Giữ lại hoặc xóa cũng được, nhưng để cho an toàn

class SearchPanel : public wxPanel
{
public:
    SearchPanel(wxWindow* parent);
};