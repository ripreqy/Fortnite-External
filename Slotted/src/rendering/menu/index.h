#ifndef INDEX_H
#define INDEX_H
#include <include.h>
#include <src/cache/index.h>
#include <thirdparty/imgui/fonts.h>
#include "icons/rank.h"
#include "icons/platicon.h"

namespace overlay {
	static inline int keystatus = 0;
	static inline int realkey = 0;
	static inline ImFont* mainfont = nullptr;
	static inline ImFont* titlefont = nullptr;
	static inline ImFont* icons = nullptr;
	static inline ImFont* tabfont = nullptr;
	static inline ImFont* childtitle = nullptr;
	static inline ImFont* arrow = nullptr;
	static inline DWORD picker_flags = ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreview;

	static inline ImVec2 pos;
	static inline int tabs = 0;
	static inline int subtabs = 0;
	static inline bool menu_active = true;

	struct resources
	{
		HWND hwnd{ nullptr };
		ID3D11Device* device{ nullptr };
		ID3D11DeviceContext* device_context{ nullptr };
		ID3D11RenderTargetView* render_target_view{ nullptr };
		IDXGISwapChain* swap_chain{ nullptr };
	} static inline resources_t;
	static const char* keyNames[] =
	{

		"Keybind",
		"Left Mouse",
		"Right Mouse",
		"Cancel",
		"Middle Mouse",
		"Mouse 5",
		"Mouse 4",
		"",
		"Backspace",
		"Tab",
		"",
		"",
		"Clear",
		"Enter",
		"",
		"",
		"Shift",
		"Control",
		"Alt",
		"Pause",
		"Caps",
		"",
		"",
		"",
		"",
		"",
		"",
		"Escape",
		"",
		"",
		"",
		"",
		"Space",
		"Page Up",
		"Page Down",
		"End",
		"Home",
		"Left",
		"Up",
		"Right",
		"Down",
		"",
		"",
		"",
		"Print",
		"Insert",
		"Delete",
		"",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"A",
		"B",
		"C",
		"D",
		"E",
		"F",
		"G",
		"H",
		"I",
		"J",
		"K",
		"L",
		"M",
		"N",
		"O",
		"P",
		"Q",
		"R",
		"S",
		"T",
		"U",
		"V",
		"W",
		"X",
		"Y",
		"Z",
		"",
		"",
		"",
		"",
		"",
		"Numpad 0",
		"Numpad 1",
		"Numpad 2",
		"Numpad 3",
		"Numpad 4",
		"Numpad 5",
		"Numpad 6",
		"Numpad 7",
		"Numpad 8",
		"Numpad 9",
		"Multiply",
		"Add",
		"",
		"Subtract",
		"Decimal",
		"Divide",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
	};
	struct menu_resources
	{
		ImFont* mainfont;
		ImFont* mainfont_smaller;
		ImFont* icons;
		ImFont* icons_smaller;
		ImFont* logo;
	} static inline menu_resources_t;

	static inline bool GetKey(int key)
	{
		realkey = key;
		return true;
	}
	static inline void ChangeKey(void* blank)
	{
		keystatus = 1;
		while (true)
		{
			for (int i = 0; i < 0x87; i++)
			{
				if (GetAsyncKeyState(i) & 0x8000)
				{
					aimbot::aimkey = i;
					keystatus = 0;
					return;
				}
			}
		}
	}
	inline static bool Items_ArrayGetter(void* data, int idx, const char** out_text)
	{
		const char* const* items = (const char* const*)data;
		if (out_text)
			*out_text = items[idx];
		return true;
	}
	inline void HotkeyButton(int aimkey, void* changekey, int status)
	{
		const char* preview_value = NULL;
		if (aimkey >= 0 && aimkey < IM_ARRAYSIZE(keyNames))
			Items_ArrayGetter(keyNames, aimkey, &preview_value);
		std::string aimkeys;
		if (preview_value == NULL)
			aimkeys = (("Select Key"));
		else
			aimkeys = preview_value;

		if (status == 1)
		{

			aimkeys = (("Press the Key"));
		}
		if (ImGui::Button(aimkeys.c_str(), ImVec2(155, 40)))
		{
			if (status == 0)
			{
				CreateThread(0, 0, (LPTHREAD_START_ROUTINE)changekey, nullptr, 0, nullptr);
			}
		}
	}

	static inline ImVec2 center_text( ImVec2 min , ImVec2 max , const char* text )
	{
		return min + ( max - min ) / 2 - ImGui::CalcTextSize( text ) / 2;
	}
	static inline bool hijack()
	{
		WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
			0,
			DefWindowProcA,
			0,
			0,
			nullptr,
			LoadIcon(nullptr, IDI_APPLICATION),
			LoadCursor(nullptr, IDC_ARROW),
			nullptr,
			nullptr,
			("fortnite"),
			LoadIcon(nullptr, IDI_APPLICATION)
		};

		RECT Rect;
		GetWindowRect(GetDesktopWindow(), &Rect);

		RegisterClassExA(&wcex);

		resources_t.hwnd = CreateWindowExA(NULL, ("fortnite"), ("lol"), WS_POPUP, Rect.left, Rect.top, Rect.right, Rect.bottom, NULL, NULL, wcex.hInstance, NULL);


		SetWindowLong(resources_t.hwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW | WS_EX_LAYERED);
		MARGINS margin = { -1 };
		DwmExtendFrameIntoClientArea(resources_t.hwnd, &margin);
		ShowWindow(resources_t.hwnd, SW_SHOW);
		SetWindowPos(resources_t.hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetLayeredWindowAttributes(resources_t.hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
		UpdateWindow(resources_t.hwnd);

		return true;
	}

	static inline bool hijack_window( )
	{
		resources_t.hwnd = FindWindowA( ( "MedalOverlayClass" ) , ( "MedalOverlay" ) ); // please dont use this, its only for this shit ass post
		if ( !resources_t.hwnd )
		{
			MessageBoxA( 0 , ( "Couldn't find overlay." ) , 0 , 0 );
			return false;
		}

		MARGINS window_margin{ -1 };

		DwmExtendFrameIntoClientArea( resources_t.hwnd , &window_margin );
		SetLayeredWindowAttributes( resources_t.hwnd , 0 , 255 , LWA_ALPHA );

		UpdateWindow( resources_t.hwnd );
		ShowWindow( resources_t.hwnd , SW_SHOW );

		return true;
	}


	static inline bool setup_d3d( )
	{
		DXGI_SWAP_CHAIN_DESC swap_chain_desc = {};
		ZeroMemory( &swap_chain_desc , sizeof( swap_chain_desc ) );
		swap_chain_desc.BufferCount = 2;
		swap_chain_desc.BufferDesc.Width = 0;
		swap_chain_desc.BufferDesc.Height = 0;
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
		swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swap_chain_desc.OutputWindow = resources_t.hwnd;
		swap_chain_desc.SampleDesc.Count = 1;
		swap_chain_desc.SampleDesc.Quality = 0;
		swap_chain_desc.Windowed = TRUE;
		swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		const D3D_FEATURE_LEVEL feature_levels [ ] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
		D3D_FEATURE_LEVEL selected_feature_level;

		if ( FAILED( D3D11CreateDeviceAndSwapChain(
			nullptr , D3D_DRIVER_TYPE_HARDWARE , nullptr , 0 ,
			feature_levels , _countof( feature_levels ) ,
			D3D11_SDK_VERSION , &swap_chain_desc ,
			&resources_t.swap_chain , &resources_t.device ,
			&selected_feature_level , &resources_t.device_context ) ) )
			return false;

		ID3D11Texture2D* back_buffer = nullptr;
		if ( FAILED( resources_t.swap_chain->GetBuffer( 0 , IID_PPV_ARGS( &back_buffer ) ) ) || !back_buffer )
		{
			return false;
		}

		if ( FAILED( resources_t.device->CreateRenderTargetView( back_buffer , nullptr , &resources_t.render_target_view ) ) )
		{
			return false;
		}

		back_buffer->Release( );

		ImGui::CreateContext( );

		ImGui::StyleColorsDark( );

		{
			auto& io = ImGui::GetIO( );

			mainfont = io.Fonts->AddFontFromMemoryTTF( &mainfonthxd , sizeof mainfonthxd , 18 , NULL , io.Fonts->GetGlyphRangesCyrillic( ) );
			titlefont = io.Fonts->AddFontFromMemoryTTF( &titlefonthxd , sizeof titlefonthxd , 24 , NULL , io.Fonts->GetGlyphRangesCyrillic( ) );
			icons = io.Fonts->AddFontFromMemoryTTF( &iconshxd , sizeof iconshxd , 18 , NULL , io.Fonts->GetGlyphRangesCyrillic( ) );
			childtitle = io.Fonts->AddFontFromMemoryTTF( &titlefonthxd , sizeof titlefonthxd , 18 , NULL , io.Fonts->GetGlyphRangesCyrillic( ) );
			arrow = io.Fonts->AddFontFromMemoryTTF( &arrowhxd , sizeof arrowhxd , 18 , NULL , io.Fonts->GetGlyphRangesCyrillic( ) );
			icon = io.Fonts->AddFontFromMemoryTTF(&platicon, sizeof(platicon), 28, NULL, io.Fonts->GetGlyphRangesCyrillic());
			//interFont = io.Fonts->AddFontFromFileTTF( "C:\\Users\\admin\\inter.ttf", 13, NULL );
			interFont = io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\consolab.ttf", 13, NULL );
		}
		{
			D3DX11_IMAGE_LOAD_INFO image; ID3DX11ThreadPump* pump{ nullptr };
			if (rankesp::Bronze1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &bronze1, sizeof(bronze1), &image, pump, &rankesp::Bronze1, 0);
			if (rankesp::Bronze2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &bronze2, sizeof(bronze2), &image, pump, &rankesp::Bronze2, 0);
			if (rankesp::Bronze3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &bronze3, sizeof(bronze3), &image, pump, &rankesp::Bronze3, 0);
			if (rankesp::Silver1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &silver1, sizeof(silver1), &image, pump, &rankesp::Silver1, 0);
			if (rankesp::Silver2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &silver2, sizeof(silver2), &image, pump, &rankesp::Silver2, 0);
			if (rankesp::Silver3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &silver3, sizeof(silver3), &image, pump, &rankesp::Silver3, 0);
			if (rankesp::Gold1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &gold1, sizeof(gold1), &image, pump, &rankesp::Gold1, 0);
			if (rankesp::Gold2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &gold2, sizeof(gold2), &image, pump, &rankesp::Gold2, 0);
			if (rankesp::Gold3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &gold3, sizeof(gold3), &image, pump, &rankesp::Gold3, 0);
			if (rankesp::Plat1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &platin1, sizeof(platin1), &image, pump, &rankesp::Plat1, 0);
			if (rankesp::Plat2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &platin2, sizeof(platin2), &image, pump, &rankesp::Plat2, 0);
			if (rankesp::Plat3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &platin3, sizeof(platin3), &image, pump, &rankesp::Plat3, 0);
			if (rankesp::Diamond1 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &diamond1, sizeof(diamond1), &image, pump, &rankesp::Diamond1, 0);
			if (rankesp::Diamond2 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &diamond2, sizeof(diamond2), &image, pump, &rankesp::Diamond2, 0);
			if (rankesp::Diamond3 == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &diamond3, sizeof(diamond3), &image, pump, &rankesp::Diamond3, 0);
			if (rankesp::Elite == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &elite, sizeof(elite), &image, pump, &rankesp::Elite, 0);
			if (rankesp::Champion == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &champion, sizeof(champion), &image, pump, &rankesp::Champion, 0);
			if (rankesp::Unreal == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &unreal, sizeof(unreal), &image, pump, &rankesp::Unreal, 0);
			if (rankesp::Unranked == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &unranked, sizeof(unranked), &image, pump, &rankesp::Unranked, 0);
			if (rankesp::cat == nullptr) D3DX11CreateShaderResourceViewFromMemory(resources_t.device, &carr, sizeof(carr), &image, pump, &rankesp::cat, 0);
		}

		ImGui_ImplWin32_Init( resources_t.hwnd );
		ImGui_ImplDX11_Init( resources_t.device , resources_t.device_context );

		resources_t.device->Release( );

		Sleep( 250 );

		return true;
	}

	static inline void menu( ImDrawList* drawlist ) {
		if ( !menu_active ) return;
		ImGui::Begin("##menu", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
		ImGui::SetWindowSize(ImVec2(600, 400));
		drawlist = ImGui::GetWindowDrawList( );
		if (ImGui::BeginTabBar("MainTabs")) {
			if (ImGui::BeginTabItem("Aimbot")) {
				ImGui::Checkbox("Enable Aimbot", &aimbot::enabled);
				ImGui::Checkbox("Enable Fov", &aimbot::fov_circle);
				ImGui::Checkbox("Enable Fov RGB", &aimbot::RGB);
				ImGui::Checkbox("Enable Fov Waring Arrows", &visuals::arrow);
				if (visuals::arrow)
				{
					ImGui::Checkbox("Enable Filled Arrows", &visuals::filledarrow);
				}
				ImGui::Checkbox("Enable Visible Check", &aimbot::visible_check);
				ImGui::Checkbox("Enable Prediction", &aimbot::prediction);
				ImGui::SliderInt("Smooth", &aimbot::smooth, 1, 10);
				ImGui::SliderInt("FOV", &aimbot::aim_fov, 50, 450);
				ImGui::SliderInt("RGB Speed", &aimbot::rainbowSpeed, 1, 10, NULL, NULL);
				HotkeyButton(aimbot::aimkey, ChangeKey, keystatus);
				const char* hitboxOptions[] = { "Head", "Neck", "Chest", "Pelvis" };
				static int currentHitbox = aimbot::hitbox;
				if (ImGui::Combo("Hitbox", &currentHitbox, hitboxOptions, IM_ARRAYSIZE(hitboxOptions))) {
					aimbot::hitbox = currentHitbox;
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Visuals")) {
				ImGui::Checkbox("Box", &visuals::boxes);
				ImGui::Checkbox("Cornered", &visuals::corner_boxes);
				ImGui::Checkbox("Skeleton", &visuals::skeleton);
				if (visuals::skeleton) {
					ImGui::Checkbox("Curve", &visuals::curve);
					ImGui::Checkbox("Spaghetti", &visuals::spaghetti);
				}
				ImGui::Checkbox("Outline", &visuals::outline);
				ImGui::Checkbox("Distance", &visuals::distance);
				ImGui::Checkbox("Snapline", &visuals::snapline);
				ImGui::Checkbox("Draw Rank", &visuals::ranks);
				ImGui::Checkbox("Draw Platform", &visuals::platform);
				ImGui::Checkbox("Draw username", &visuals::username);
				ImGui::Checkbox("Spectator list", &visuals::Spectatorlist);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Misc")) {
				ImGui::Text("Miscellaneous Settings");
				ImGui::ColorEdit4("Visible Color", colors::box_visible, picker_flags);
				ImGui::ColorEdit4("Invisible Color", colors::box_invisible, picker_flags);
				ImGui::SliderInt("Thickness", &visuals::thickness, 1, 10);
				ImGui::SliderInt("RenderDistance", &visuals::MaxRenderDistance, 100, 2000);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Radar")) {
				ImGui::Checkbox("Enable Radar", &visuals::radaresp);
				ImGui::SliderInt("Radar Size", &visuals::radarsize, 50, 350);
				ImGui::SliderInt("Radar X", &visuals::radar_position_x , 5, 200);
				ImGui::SliderInt("Radar Y", &visuals::radar_position_y , 200 , 500);
				ImGui::EndTabItem();
			}


			if (ImGui::BeginTabItem("World"))
			{
				ImGui::Checkbox("Enable uncommon_loot", &world::uncommon_loot);
				ImGui::Checkbox("Enable common_loot", &world::common_loot);
				ImGui::Checkbox("Enable llamas", &world::llamas);
				ImGui::Checkbox("Enable rare_loot", &world::rare_loot);
				ImGui::Checkbox("Enable foodbox", &world::foodbox);
				ImGui::Checkbox("Enable epic_loot", &world::epic_loot);
				ImGui::Checkbox("Enable legendary_loot", &world::legendary_loot);
				ImGui::Checkbox("Enable mythic_loot", &world::mythic_loot);
				ImGui::Checkbox("Enable chests", &world::chests);
				ImGui::Checkbox("Enable ammobox", &world::ammobox);
				ImGui::Checkbox("Enable vehicle", &world::vehicle);
				ImGui::Checkbox("Enable spirits", &world::spirits);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}

	static inline void loop( )
	{
		constexpr float clear_color [ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		MSG msg{};

		while ( msg.message != WM_QUIT )
		{
			while ( PeekMessage( &msg , nullptr , 0 , 0 , PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}

			auto& io = ImGui::GetIO( );
			io.DeltaTime = 1.0f / 60.0f;

			{
				if ( GetAsyncKeyState( VK_INSERT ) & 1 )
				{
					menu_active = !menu_active;
				}
			}

			if ( menu_active )
			{
				POINT cursor_pos;
				GetCursorPos( &cursor_pos );
				io.MousePos = ImVec2( static_cast< float >( cursor_pos.x ) , static_cast< float >( cursor_pos.y ) );
				io.MouseDown [ 0 ] = ( GetAsyncKeyState( VK_LBUTTON ) & 0x8000 ) != 0;
			}

			ImGui_ImplDX11_NewFrame( );
			ImGui_ImplWin32_NewFrame( );

			ImGui::NewFrame( );
			{
				auto drawlist = ImGui::GetBackgroundDrawList( );

				visuals::draw_actors( drawlist );
				visuals::WorldLoop( drawlist );
				//visuals::leveldrawing( drawlist );
				menu( drawlist );
			}
			ImGui::Render( );

			resources_t.device_context->OMSetRenderTargets( 1 , &resources_t.render_target_view , nullptr );
			resources_t.device_context->ClearRenderTargetView( resources_t.render_target_view , clear_color );

			ImGui_ImplDX11_RenderDrawData( ImGui::GetDrawData( ) );
			resources_t.swap_chain->Present( 1 , 0 );
		}

		ImGui_ImplDX11_Shutdown( );
		ImGui_ImplWin32_Shutdown( );
		ImGui::DestroyContext( );
	}

	static inline bool run( ) {
		if ( !hijack( ) )
		{
			return false;
		}

		if ( !setup_d3d( ) )
		{
			return false;
		}

		std::cout << ( "initiated." ) << std::endl;

		loop( );
		return true;
	}

} // namespace overlay

#endif // !INDEX_H
