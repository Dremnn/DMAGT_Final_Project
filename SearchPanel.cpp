#include "SearchPanel.h"
#include "MapPanel.h"
#include "ModernColors.h"
#include "SimpleUIHelper.h"
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/statbmp.h>
#include <wx/popupwin.h>
using namespace std;

//Constructor
SearchPanel::SearchPanel(wxWindow* p) : wxPanel(p, wxID_ANY, wxDefaultPosition, wxSize(350, -1)) {
    SetBackgroundColour(ModernColors::BACKGROUND_CARD);
    wxBoxSizer* m_sizer = new wxBoxSizer(wxVERTICAL);
    CreateSearchSection(m_sizer);
    CreateButtonSection(m_sizer);
    CreateSavedPlacesSection(m_sizer);
    SetSizer(m_sizer);
}

//Create Searchbox (logo + input)
void SearchPanel::CreateSearchSection(wxBoxSizer* m_sizer) {
    wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    wxImage logoImg("grab_logo.png", wxBITMAP_TYPE_PNG);
    if (logoImg.IsOk()) {
        int nHeight = 100;
        int nWidth = logoImg.GetWidth() * nHeight / logoImg.GetHeight();
        logoImg.Rescale(nWidth, nHeight, wxIMAGE_QUALITY_HIGH);
        wxStaticBitmap* logoBitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(logoImg));
        headerSizer->Add(logoBitmap, 0, wxALIGN_CENTER_VERTICAL);
    }else {
        wxStaticText* title = new wxStaticText(this, wxID_ANY, _T("Grab Routes"));

        //Styling title
        wxFont title_font = title->GetFont();
        title_font.SetPointSize(16);
        title_font.SetWeight(wxFONTWEIGHT_BOLD);
        title_font.SetFaceName("Segoe UI");
        title->SetFont(title_font);
        title->SetForegroundColour(ModernColors::TEXT_PRIMARY);
        headerSizer->Add(title, 1, wxALIGN_CENTER_VERTICAL);
    }

    headerSizer->AddStretchSpacer(1);

    //Info Button
    m_infoButton = new wxButton(this, wxID_ANY, wxT("i"), wxDefaultPosition, wxSize(30, 30));
    m_infoButton->SetFont(m_infoButton->GetFont().Bold());
    m_infoButton->SetBackgroundColour(ModernColors::BACKGROUND_INPUT);
    m_infoButton->SetForegroundColour(ModernColors::TEXT_SECONDARY);

	//Hover mouse events
    m_infoButton->Bind(wxEVT_ENTER_WINDOW, &SearchPanel::OnInfoButtonEnter, this);
    m_infoButton->Bind(wxEVT_LEAVE_WINDOW, &SearchPanel::OnInfoButtonLeave, this);

    headerSizer->Add(m_infoButton, 0, wxALIGN_CENTER_VERTICAL);
    m_sizer->Add(headerSizer, 0, wxEXPAND | wxALL, 16);

    //Input
    wxBoxSizer* input_size = new wxBoxSizer(wxVERTICAL);

    //Label Điểm đi
    wxStaticText* startLabel = new wxStaticText(this, wxID_ANY, _T("Điểm đi:"));
    SimpleUIHelper::SetModernFont(startLabel);
    startLabel->SetForegroundColour(ModernColors::TEXT_SECONDARY);

	//TextCtrl Điểm đi
    m_startPointCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_startPointCtrl->SetHint(_T("Nhập điểm đi"));
    SimpleUIHelper::StyleTextCtrl(m_startPointCtrl);

	//Label Điểm đến
    wxStaticText* endLabel = new wxStaticText(this, wxID_ANY, _T("Điểm đến:"));
    SimpleUIHelper::SetModernFont(endLabel);
    endLabel->SetForegroundColour(ModernColors::TEXT_SECONDARY);

	//TextCtrl Điểm đến
    m_endPointCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    m_endPointCtrl->SetHint(_T("Nhập điểm đến"));
    SimpleUIHelper::StyleTextCtrl(m_endPointCtrl);

    //Add controls to input sizer
    input_size->Add(startLabel, 0, wxBOTTOM, 4);
    input_size->Add(m_startPointCtrl, 0, wxEXPAND | wxBOTTOM, 12);
    input_size->Add(endLabel, 0, wxBOTTOM, 4);
    input_size->Add(m_endPointCtrl, 0, wxEXPAND, 0);
    m_sizer->Add(input_size, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 16);
}

//Hover the info button
void SearchPanel::OnInfoButtonEnter(wxMouseEvent& event) {
    if (m_infoPopup) {
        m_infoPopup->Destroy();
        m_infoPopup = nullptr;
    }

    m_infoPopup = new wxPopupWindow(this, wxBORDER_SIMPLE);
    m_infoPopup->SetBackgroundColour(ModernColors::BACKGROUND_CARD);

    wxString TeamMembers =
        _T("THÀNH VIÊN NHÓM\n")
        _T("1. Lý Trần Gia Khang - MSSV: 24110098\n")
        _T("2. Đoàn Trọng Trung - MSSV: 24110140\n")
        _T("3. Khổng Đình Tú - MSSV: 24110145");

    wxStaticText* infoText = new wxStaticText(m_infoPopup, wxID_ANY, TeamMembers);
    infoText->SetForegroundColour(ModernColors::TEXT_PRIMARY);

    wxFont font(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, "Segoe UI");
    infoText->SetFont(font);

    wxSize maxSize = infoText->GetBestSize();
    m_infoPopup->SetSize(maxSize.GetWidth() + 20, maxSize.GetHeight() + 20);

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(infoText, 1, wxEXPAND | wxALL, 10);
    m_infoPopup->SetSizer(sizer);

    wxPoint coor = m_infoButton->ClientToScreen(wxPoint(0, 0));
    wxSize button_size = m_infoButton->GetSize();
    long long find_x = button_size.GetWidth() + 5;
    long long find_y = (m_infoPopup->GetSize().GetHeight() - button_size.GetHeight()) / 2;
    coor.x += find_x;
    coor.y -= find_y;

    m_infoPopup->SetPosition(coor);
    m_infoPopup->Show();
}

//Move out the info button
void SearchPanel::OnInfoButtonLeave(wxMouseEvent& event) {
    if (m_infoPopup) {
        m_infoPopup->Destroy();
        m_infoPopup = nullptr;
    }
}

//Create Button
void SearchPanel::CreateButtonSection(wxBoxSizer* m_sizer)
{
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);

    wxButton* searchButton = new wxButton(this, wxID_ANY, _T("🔍 Tìm đường đi"));
    SimpleUIHelper::StyleButton(searchButton, true);

    wxButton* allPathsButton = new wxButton(this, wxID_ANY, _T("Tất cả đường đi"));
    SimpleUIHelper::StyleButton(allPathsButton, false);

    wxButton* clearButton = new wxButton(this, wxID_ANY, _T("Xóa đường đi"));
    clearButton->SetBackgroundColour(ModernColors::DANGER);
    clearButton->SetForegroundColour(ModernColors::TEXT_WHITE);
    SimpleUIHelper::SetModernFont(clearButton);
    clearButton->SetMinSize(wxSize(-1, 36));

    buttonSizer->Add(searchButton, 0, wxEXPAND | wxBOTTOM, 8);
    buttonSizer->Add(allPathsButton, 0, wxEXPAND | wxBOTTOM, 8);
    buttonSizer->Add(clearButton, 0, wxEXPAND, 0);

    wxStaticLine* divider = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 1), wxLI_HORIZONTAL);

    m_sizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 16);
    m_sizer->Add(divider, 0, wxEXPAND | wxLEFT | wxRIGHT, 16);

    //Bind events
    searchButton->Bind(wxEVT_BUTTON, &SearchPanel::OnSearchClicked, this);
    allPathsButton->Bind(wxEVT_BUTTON, &SearchPanel::OnShowAllPathsClicked, this);
    clearButton->Bind(wxEVT_BUTTON, &SearchPanel::OnClearClicked, this);
}

//Create Saved Places
void SearchPanel::CreateSavedPlacesSection(wxBoxSizer* m_sizer)
{
    //Header title
    wxStaticText* placesTitle = new wxStaticText(this, wxID_ANY, _T("Địa điểm đã lưu"));
    wxFont title_font = placesTitle->GetFont();
    title_font.SetPointSize(12);
    title_font.SetWeight(wxFONTWEIGHT_BOLD);
    title_font.SetFaceName("Segoe UI");
    placesTitle->SetFont(title_font);
    placesTitle->SetForegroundColour(ModernColors::TEXT_PRIMARY);
    
    //Scrolled panel
    wxScrolledWindow* scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    scrolledWindow->SetScrollRate(0, 5);
    scrolledWindow->SetBackgroundColour(ModernColors::BACKGROUND_CARD);

    wxBoxSizer* placesSizer = new wxBoxSizer(wxVERTICAL);

    //List of places
    vector<pair<wxString, wxString>> savedDirections = 
    {
        {_T("Dinh Độc Lập"), _T("135 Nam Kỳ Khởi Nghĩa, Bến Thành, Quận 1")},
        {_T("Nhà Thờ Đức Bà"), _T("01 Công Xã Paris, Bến Nghé, Quận 1")},
        {_T("Bưu Điện Trung Tâm"), _T("02 Công Xã Paris, Bến Nghé, Quận 1")},
        {_T("Chợ Bến Thành"), _T("Đ. Lê Lợi, Bến Thành, Quận 1")},
        {_T("Phố Đi Bộ Nguyễn Huệ"), _T("Nguyễn Huệ, Bến Nghé, Quận 1")},
        {_T("Bitexco Financial Tower"), _T("02 Hải Triều, Bến Nghé, Quận 1")}
    };

    for (const auto& place : savedDirections) {
        wxPanel* placePanel = new wxPanel(scrolledWindow);
        placePanel->SetBackgroundColour(ModernColors::BACKGROUND_INPUT);

        wxStaticText* nameText = new wxStaticText(placePanel, wxID_ANY, place.first);
        wxFont nameFont = nameText->GetFont();
        nameFont.SetWeight(wxFONTWEIGHT_BOLD);
        nameFont.SetFaceName("Segoe UI");
        nameText->SetFont(nameFont);
        nameText->SetForegroundColour(ModernColors::TEXT_PRIMARY);

        wxStaticText* addressText = new wxStaticText(placePanel, wxID_ANY, place.second);
        wxFont addressFont = addressText->GetFont();
        addressFont.SetFaceName("Segoe UI");
        addressFont.SetPointSize(addressFont.GetPointSize() - 1);
        addressText->SetFont(addressFont);
        addressText->SetForegroundColour(ModernColors::TEXT_SECONDARY);

        wxBoxSizer* placeSizer = new wxBoxSizer(wxVERTICAL);
        placeSizer->Add(nameText, 0, wxEXPAND | wxALL, 8);
        placeSizer->Add(addressText, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 8);
        placePanel->SetSizer(placeSizer);

        placesSizer->Add(placePanel, 0, wxEXPAND | wxBOTTOM, 4);

        if (&place != &savedDirections.back()) {
            wxStaticLine* sep = new wxStaticLine(scrolledWindow, wxID_ANY, wxDefaultPosition, wxSize(-1, 1));
            placesSizer->Add(sep, 0, wxEXPAND | wxBOTTOM, 4);
        }
    }
    scrolledWindow->SetSizer(placesSizer);

    m_sizer->Add(placesTitle, 0, wxALL, 16);
    m_sizer->Add(scrolledWindow, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 16);
}

//Connect MapPanel
void SearchPanel::SetMapPanel(MapPanel* mapPanel) {m_mapPanel = mapPanel;}

//Process when clicked "Tìm đường đi"
void SearchPanel::OnSearchClicked(wxCommandEvent& event){
    m_mapPanel->ClearAllPaths();
    if (!m_mapPanel) {
        wxLogError(_T("Lỗi: MapPanel chưa được thêm vào."));
        return;
    }

    wxString firstName = m_startPointCtrl->GetValue();
    wxString lastName = m_endPointCtrl->GetValue();

    if (firstName.IsEmpty() || lastName.IsEmpty()) {
        wxMessageBox(_T("Vui lòng nhập cả điểm đi và điểm đến."), _T("Thông tin không đầy đủ"), wxOK | wxICON_WARNING);
        return;
    }

    int start_idx = m_mapPanel->FindNodeIndexByName(firstName);
    int end_idx = m_mapPanel->FindNodeIndexByName(lastName);

    if (start_idx == -1 || end_idx == -1) {
        wxString msg = _T("Không tìm thấy địa điểm:\n");
        if (start_idx == -1) msg += _T("- ") + firstName + _T("\n");
        if (end_idx == -1) msg += _T("- ") + lastName + _T("\n");
        msg += _T("\nVui lòng kiểm tra lại tên địa điểm.");

        wxMessageBox(msg, _T("Địa điểm không tồn tại trên hệ thống!"), wxOK | wxICON_ERROR);
        return;
    }

    m_mapPanel->FindAndDrawNewPath(start_idx, end_idx);
}

//Process when clicked "Tìm tất cả đường đi"
void SearchPanel::OnShowAllPathsClicked(wxCommandEvent& event) {
    if (!m_mapPanel) {
        wxLogError(_T("Lỗi: MapPanel chưa được gán."));
        return;
    }

    wxString firstName = m_startPointCtrl->GetValue();
    wxString lastName = m_endPointCtrl->GetValue();

    if (firstName.IsEmpty() || lastName.IsEmpty()) {
        wxMessageBox(_T("Vui lòng nhập cả điểm đi/điểm đến."), _T("Thông tin không đầy đủ"), wxOK | wxICON_WARNING);
        return;
    }

    int start_idx = m_mapPanel->FindNodeIndexByName(firstName);
    int end_idx = m_mapPanel->FindNodeIndexByName(lastName);

    if (start_idx == -1 || end_idx == -1) {
        wxString msg = _T("Không tìm thấy địa điểm:\n");
        if (start_idx == -1) msg += _T("- ") + firstName + _T("\n");
        if (end_idx == -1) msg += _T("- ") + lastName + _T("\n");
        msg += _T("\nVui lòng kiểm tra lại tên địa điểm.");

        wxMessageBox(msg, _T("Địa điểm không tồn tại"), wxOK | wxICON_ERROR);
        return;
    }

    m_mapPanel->FindAndDrawAllPaths(start_idx, end_idx);
}

//Process when clicked "Xóa đường đi"
void SearchPanel::OnClearClicked(wxCommandEvent& event)
{
    if (m_mapPanel) m_mapPanel->ClearAllPaths();
    m_startPointCtrl->Clear();
    m_endPointCtrl->Clear();
}