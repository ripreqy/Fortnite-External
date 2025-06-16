#pragma once
#include <thirdparty/imgui/imgui.h>
#include <thirdparty/imgui/imgui_impl_win32.h>
#include <thirdparty/imgui/imgui_impl_dx11.h>
#include <thirdparty/imgui/imgui_internal.h>
#include <src/engine/sdk.h>


inline std::wstring MBytesToWString(const char* lpcszString)
{
	int len = strlen(lpcszString);
	int unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, NULL, 0);
	wchar_t* pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_ACP, 0, lpcszString, -1, (LPWSTR)pUnicode, unicodeLen);
	std::wstring wString = (wchar_t*)pUnicode;
	delete[] pUnicode;
	return wString;
}
inline std::string WStringToUTF8(const wchar_t* lpwcszWString)
{
	char* pElementText;
	int iTextLen = ::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, NULL, 0, NULL, NULL);
	pElementText = new char[iTextLen + 1];
	memset((void*)pElementText, 0, (iTextLen + 1) * sizeof(char));
	::WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)lpwcszWString, -1, pElementText, iTextLen, NULL, NULL);
	std::string strReturn(pElementText);
	delete[] pElementText;
	return strReturn;
}

inline void box( ImDrawList* drawlist , const bounds& player_bounds , const ImColor& col , float thickness, bool outlined)
{
	const float box_width = player_bounds.max_x - player_bounds.min_x;
	const float box_height = player_bounds.max_y - player_bounds.min_y;
	ImColor outline_color = ImColor(0, 0, 0, 255); // Black outline
	if (outlined) {
		drawlist->AddRect(ImVec2(player_bounds.min_x - 1, player_bounds.min_y - 1),
			ImVec2(player_bounds.min_x + box_width + 1, player_bounds.min_y + box_height + 1),
			outline_color, 0, 0, thickness * 2);

		drawlist->AddRect(ImVec2(player_bounds.min_x + 1, player_bounds.min_y + 1),
			ImVec2(player_bounds.min_x + box_width - 1, player_bounds.min_y + box_height - 1),
			outline_color, 0, 0, thickness * 2);
	}

	drawlist->AddRect( ImVec2( player_bounds.min_x , player_bounds.min_y ) , ImVec2( player_bounds.min_x + box_width , player_bounds.min_y + box_height ) , col , 0 , 0 , thickness );

}

inline void cornered_box( ImDrawList* drawlist , const bounds& player_bounds , const ImColor& col , bool outlined , float thickness )
{
	const float box_width = player_bounds.max_x - player_bounds.min_x;
	const float box_height = player_bounds.max_y - player_bounds.min_y;
	const float corner_size = box_width * 0.2f;

	ImColor outline_color = ImColor( 0 , 0 , 0 , 255 );

	if ( outlined ) {
		drawlist->AddLine( ImVec2( player_bounds.min_x - 1 , player_bounds.min_y - 1 ) , ImVec2( player_bounds.min_x + corner_size + 1 , player_bounds.min_y - 1 ) , outline_color , thickness * 2 );
		drawlist->AddLine( ImVec2( player_bounds.min_x - 1 , player_bounds.min_y - 1 ) , ImVec2( player_bounds.min_x - 1 , player_bounds.min_y + corner_size + 1 ) , outline_color , thickness * 2 );

		drawlist->AddLine( ImVec2( player_bounds.max_x + 1 , player_bounds.min_y - 1 ) , ImVec2( player_bounds.max_x - corner_size - 1 , player_bounds.min_y - 1 ) , outline_color , thickness * 2 );
		drawlist->AddLine( ImVec2( player_bounds.max_x + 1 , player_bounds.min_y - 1 ) , ImVec2( player_bounds.max_x + 1 , player_bounds.min_y + corner_size + 1 ) , outline_color , thickness * 2 );

		drawlist->AddLine( ImVec2( player_bounds.min_x - 1 , player_bounds.max_y + 1 ) , ImVec2( player_bounds.min_x + corner_size + 1 , player_bounds.max_y + 1 ) , outline_color , thickness * 2 );
		drawlist->AddLine( ImVec2( player_bounds.min_x - 1 , player_bounds.max_y + 1 ) , ImVec2( player_bounds.min_x - 1 , player_bounds.max_y - corner_size - 1 ) , outline_color , thickness * 2 );

		drawlist->AddLine( ImVec2( player_bounds.max_x + 1 , player_bounds.max_y + 1 ) , ImVec2( player_bounds.max_x - corner_size - 1 , player_bounds.max_y + 1 ) , outline_color , thickness * 2 );
		drawlist->AddLine( ImVec2( player_bounds.max_x + 1 , player_bounds.max_y + 1 ) , ImVec2( player_bounds.max_x + 1 , player_bounds.max_y - corner_size - 1 ) , outline_color , thickness * 2 );
	}

	drawlist->AddLine( ImVec2( player_bounds.min_x , player_bounds.min_y ) , ImVec2( player_bounds.min_x + corner_size , player_bounds.min_y ) , col , thickness );
	drawlist->AddLine( ImVec2( player_bounds.min_x , player_bounds.min_y ) , ImVec2( player_bounds.min_x , player_bounds.min_y + corner_size ) , col , thickness );

	drawlist->AddLine( ImVec2( player_bounds.max_x , player_bounds.min_y ) , ImVec2( player_bounds.max_x - corner_size , player_bounds.min_y ) , col , thickness );
	drawlist->AddLine( ImVec2( player_bounds.max_x , player_bounds.min_y ) , ImVec2( player_bounds.max_x , player_bounds.min_y + corner_size ) , col , thickness );

	drawlist->AddLine( ImVec2( player_bounds.min_x , player_bounds.max_y ) , ImVec2( player_bounds.min_x + corner_size , player_bounds.max_y ) , col , thickness );
	drawlist->AddLine( ImVec2( player_bounds.min_x , player_bounds.max_y ) , ImVec2( player_bounds.min_x , player_bounds.max_y - corner_size ) , col , thickness );

	drawlist->AddLine( ImVec2( player_bounds.max_x , player_bounds.max_y ) , ImVec2( player_bounds.max_x - corner_size , player_bounds.max_y ) , col , thickness );
	drawlist->AddLine( ImVec2( player_bounds.max_x , player_bounds.max_y ) , ImVec2( player_bounds.max_x , player_bounds.max_y - corner_size ) , col , thickness );
}
inline void DrawString(ImDrawList* drawlist, float fontSize, int x, int y, ImColor color, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[128] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		drawlist->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		drawlist->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		drawlist->AddText(ImGui::GetFont(), fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		drawlist->AddText(ImGui::GetFont(), fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	drawlist->AddText(ImGui::GetFont(), fontSize, ImVec2(x, y), ImColor(color), text.c_str());
}
inline void Icon(ImDrawList* drawlist, ImFont* font, float fontSize, int x, int y, ImColor color, bool bCenter, bool stroke, const char* pText, ...)
{
	va_list va_alist;
	char buf[128] = { 0 };
	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);
	std::string text = WStringToUTF8(MBytesToWString(buf).c_str());
	if (bCenter)
	{
		ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
		x = x - textSize.x / 4;
		y = y - textSize.y;
	}
	if (stroke)
	{
		drawlist->AddText(font, fontSize, ImVec2(x + 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		drawlist->AddText(font, fontSize, ImVec2(x - 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		drawlist->AddText(font, fontSize, ImVec2(x + 1, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
		drawlist->AddText(font, fontSize, ImVec2(x - 1, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), text.c_str());
	}
	drawlist->AddText(font, fontSize, ImVec2(x, y), ImColor(color), text.c_str());


}

inline void draw_text(ImDrawList* drawlist, ImFont* font, float fontSize, ImVec2 position, ImColor color, const char* text)
{
	drawlist->AddText(font, fontSize, ImVec2(position.x - 1, position.y - 1), ImColor(0, 0, 0), text);
	drawlist->AddText(font, fontSize, ImVec2(position.x + 1, position.y - 1), ImColor(0, 0, 0), text);
	drawlist->AddText(font, fontSize, ImVec2(position.x - 1, position.y + 1), ImColor(0, 0, 0), text);
	drawlist->AddText(font, fontSize, ImVec2(position.x + 1, position.y + 1), ImColor(0, 0, 0), text);
	drawlist->AddText(font, fontSize, position, color, text);
}

inline ImColor get_loot_color(EFortRarity tier)
{
	switch (tier)
	{
	case EFortRarity::EFortRarity__Common: return ImColor(123, 123, 123, 255);
	case EFortRarity::EFortRarity__Uncommon: return ImColor(58, 121, 19, 255);
	case EFortRarity::EFortRarity__Rare: return ImColor(18, 88, 162, 255);
	case EFortRarity::EFortRarity__Epic: return ImColor(189, 63, 250, 255);
	case EFortRarity::EFortRarity__Legendary: return ImColor(255, 118, 5, 255);
	case EFortRarity::EFortRarity__Mythic: return ImColor(220, 160, 30, 255);
	default: return ImColor(123, 123, 123, 255);
	}
}
inline void draw_item(ImDrawList* drawlist, const char* name, const char* display_name, const ImColor& color, uintptr_t actor) {

	uintptr_t actor_root_component = read<uintptr_t>(actor + offsets::RootComponent);
	numerics::fvector actor_relative_location = read<numerics::fvector>(actor_root_component + offsets::RelativeLocation);
	numerics::fvector2d location = world_to_screen::project_to_screen(actor_relative_location);

	float item_distance = camera::location.distance(actor_relative_location) * 0.01f;

	if (!numerics::is_in_screen(location)) return;

	char dist[64];
	sprintf_s(dist, "%.fm", item_distance);
	std::string text = std::string(display_name) + " [ " + std::string(dist) + " ]";
	ImVec2 text_size = ImGui::CalcTextSize(text.c_str());

	if (visuals::outline) {

		ImColor outlineColor = ImColor(0, 0, 0, 255);
		draw_text(drawlist, ImGui::GetFont(), 16, ImVec2(location.x - text_size.x / 2 - 1, location.y - 18), outlineColor, text.c_str());
		draw_text(drawlist, ImGui::GetFont(), 16, ImVec2(location.x - text_size.x / 2 + 1, location.y - 18), outlineColor, text.c_str());
		draw_text(drawlist, ImGui::GetFont(), 16, ImVec2(location.x - text_size.x / 2, location.y - 18 - 1), outlineColor, text.c_str());
		draw_text(drawlist, ImGui::GetFont(), 16, ImVec2(location.x - text_size.x / 2, location.y - 18 + 1), outlineColor, text.c_str());
	}

	draw_text(drawlist, ImGui::GetFont(), 16, ImVec2(location.x - text_size.x / 2, location.y - 18), color, text.c_str());
}
