#include "NormalizeString.h"

wxString NormalizeString(const wxString& input){
    wxString result = input;

    // Sử dụng map để dễ quản lý, bao gồm cả chữ hoa và chữ thường
    std::map<wxString, wxString> charMap = {
        {wxT("à"), wxT("a")}, {wxT("á"), wxT("a")}, {wxT("ạ"), wxT("a")}, {wxT("ả"), wxT("a")}, {wxT("ã"), wxT("a")},
        {wxT("â"), wxT("a")}, {wxT("ầ"), wxT("a")}, {wxT("ấ"), wxT("a")}, {wxT("ậ"), wxT("a")}, {wxT("ẩ"), wxT("a")}, {wxT("ẫ"), wxT("a")},
        {wxT("ă"), wxT("a")}, {wxT("ằ"), wxT("a")}, {wxT("ắ"), wxT("a")}, {wxT("ặ"), wxT("a")}, {wxT("ẳ"), wxT("a")}, {wxT("ẵ"), wxT("a")},
        {wxT("À"), wxT("a")}, {wxT("Á"), wxT("a")}, {wxT("Ạ"), wxT("a")}, {wxT("Ả"), wxT("a")}, {wxT("Ã"), wxT("a")},
        {wxT("Â"), wxT("a")}, {wxT("Ầ"), wxT("a")}, {wxT("Ấ"), wxT("a")}, {wxT("Ậ"), wxT("a")}, {wxT("Ẩ"), wxT("a")}, {wxT("Ẫ"), wxT("a")},
        {wxT("Ă"), wxT("a")}, {wxT("Ằ"), wxT("a")}, {wxT("Ắ"), wxT("a")}, {wxT("Ặ"), wxT("a")}, {wxT("Ẳ"), wxT("a")}, {wxT("Ẵ"), wxT("a")},

        {wxT("è"), wxT("e")}, {wxT("é"), wxT("e")}, {wxT("ẹ"), wxT("e")}, {wxT("ẻ"), wxT("e")}, {wxT("ẽ"), wxT("e")},
        {wxT("ê"), wxT("e")}, {wxT("ề"), wxT("e")}, {wxT("ế"), wxT("e")}, {wxT("ệ"), wxT("e")}, {wxT("ể"), wxT("e")}, {wxT("ễ"), wxT("e")},
        {wxT("È"), wxT("e")}, {wxT("É"), wxT("e")}, {wxT("Ẹ"), wxT("e")}, {wxT("Ẻ"), wxT("e")}, {wxT("Ẽ"), wxT("e")},
        {wxT("Ê"), wxT("e")}, {wxT("Ề"), wxT("e")}, {wxT("Ế"), wxT("e")}, {wxT("Ệ"), wxT("e")}, {wxT("Ể"), wxT("e")}, {wxT("Ễ"), wxT("e")},

        {wxT("ì"), wxT("i")}, {wxT("í"), wxT("i")}, {wxT("ị"), wxT("i")}, {wxT("ỉ"), wxT("i")}, {wxT("ĩ"), wxT("i")},
        {wxT("Ì"), wxT("i")}, {wxT("Í"), wxT("i")}, {wxT("Ị"), wxT("i")}, {wxT("Ỉ"), wxT("i")}, {wxT("Ĩ"), wxT("i")},

        {wxT("ò"), wxT("o")}, {wxT("ó"), wxT("o")}, {wxT("ọ"), wxT("o")}, {wxT("ỏ"), wxT("o")}, {wxT("õ"), wxT("o")},
        {wxT("ô"), wxT("o")}, {wxT("ồ"), wxT("o")}, {wxT("ố"), wxT("o")}, {wxT("ộ"), wxT("o")}, {wxT("ổ"), wxT("o")}, {wxT("ỗ"), wxT("o")},
        {wxT("ơ"), wxT("o")}, {wxT("ờ"), wxT("o")}, {wxT("ớ"), wxT("o")}, {wxT("ợ"), wxT("o")}, {wxT("ở"), wxT("o")}, {wxT("ỡ"), wxT("o")},
        {wxT("Ò"), wxT("o")}, {wxT("Ó"), wxT("o")}, {wxT("Ọ"), wxT("o")}, {wxT("Ỏ"), wxT("o")}, {wxT("Õ"), wxT("o")},
        {wxT("Ô"), wxT("o")}, {wxT("Ồ"), wxT("o")}, {wxT("Ố"), wxT("o")}, {wxT("Ộ"), wxT("o")}, {wxT("Ổ"), wxT("o")}, {wxT("Ỗ"), wxT("o")},
        {wxT("Ơ"), wxT("o")}, {wxT("Ờ"), wxT("o")}, {wxT("Ớ"), wxT("o")}, {wxT("Ợ"), wxT("o")}, {wxT("Ở"), wxT("o")}, {wxT("Ỡ"), wxT("o")},

        {wxT("ù"), wxT("u")}, {wxT("ú"), wxT("u")}, {wxT("ụ"), wxT("u")}, {wxT("ủ"), wxT("u")}, {wxT("ũ"), wxT("u")},
        {wxT("ư"), wxT("u")}, {wxT("ừ"), wxT("u")}, {wxT("ứ"), wxT("u")}, {wxT("ự"), wxT("u")}, {wxT("ử"), wxT("u")}, {wxT("ữ"), wxT("u")},
        {wxT("Ù"), wxT("u")}, {wxT("Ú"), wxT("u")}, {wxT("Ụ"), wxT("u")}, {wxT("Ủ"), wxT("u")}, {wxT("Ũ"), wxT("u")},
        {wxT("Ư"), wxT("u")}, {wxT("Ừ"), wxT("u")}, {wxT("Ứ"), wxT("u")}, {wxT("Ự"), wxT("u")}, {wxT("Ử"), wxT("u")}, {wxT("Ữ"), wxT("u")},

        {wxT("ý"), wxT("y")}, {wxT("ỳ"), wxT("y")}, {wxT("ỷ"), wxT("y")}, {wxT("ỵ"), wxT("y")}, {wxT("ỹ"), wxT("y")},
        {wxT("Ý"), wxT("y")}, {wxT("Ỳ"), wxT("y")}, {wxT("Ỷ"), wxT("y")}, {wxT("Ỵ"), wxT("y")}, {wxT("Ỹ"), wxT("y")},

        {wxT("đ"), wxT("d")}, {wxT("Đ"), wxT("d")}
    };

    for (const auto& pair : charMap) {
        result.Replace(pair.first, pair.second);
    }

    // Cuối cùng, chuyển tất cả thành chữ thường sau khi đã loại bỏ dấu
    result = result.Lower();

    // Loại bỏ khoảng trắng thừa
    result.Trim(true).Trim(false);
    while (result.Contains(wxT("  "))) {
        result.Replace(wxT("  "), wxT(" "));
    }

    return result;
}
