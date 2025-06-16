#include "index.h"
#include <src/rendering/menu/wrapper.h>
auto closest_distance = FLT_MAX;


inline void RadarRange(float* x, float* y, float range)
{

	if (fabs((*x)) > range || fabs((*y)) > range)
	{
		if ((*y) > (*x))
		{
			if ((*y) > -(*x))
			{
				(*x) = range * (*x) / (*y);
				(*y) = range;
			}
			else
			{
				(*y) = -range * (*y) / (*x);
				(*x) = -range;
			}
		}
		else
		{
			if ((*y) > -(*x))
			{
				(*y) = range * (*y) / (*x);
				(*x) = range;
			}
			else
			{
				(*x) = -range * (*x) / (*y);
				(*y) = -range;
			}
		}
	}
}

inline void CalcRadarPoint(numerics::fvector vOrigin, int& screenx, int& screeny)
{
	numerics::frotator camera_location = camera::rotation;
	numerics::frotator vAngle = camera::rotation;
	auto fYaw = vAngle.yaw * M_PI / 180.0f;
	float dx = vOrigin.x - camera::location.x;
	float dy = vOrigin.y - camera::location.y;
	float fsin_yaw = sinf(fYaw);
	float fminus_cos_yaw = -cosf(fYaw);

	float x = dy * fminus_cos_yaw + dx * fsin_yaw;
	x = -x;
	float y = dx * fminus_cos_yaw - dy * fsin_yaw;

	float range = (float)visuals::RadarDistance * 100.f;

	RadarRange(&x, &y, range);

	ImVec2 DrawPos = ImVec2(visuals::radar_position_x, visuals::radar_position_y);
	ImVec2 DrawSize = ImVec2(visuals::radarsize, visuals::radarsize);

	int rad_x = (int)DrawPos.x;
	int rad_y = (int)DrawPos.y;

	float r_siz_x = DrawSize.x;
	float r_siz_y = DrawSize.y;

	int x_max = (int)r_siz_x + rad_x - 5;
	int y_max = (int)r_siz_y + rad_y - 5;

	screenx = rad_x + ((int)r_siz_x / 2 + int(x / range * r_siz_x));
	screeny = rad_y + ((int)r_siz_y / 2 + int(y / range * r_siz_y));

	if (screenx > x_max)
		screenx = x_max;

	if (screenx < rad_x)
		screenx = rad_x;

	if (screeny > y_max)
		screeny = y_max;

	if (screeny < rad_y)
		screeny = rad_y;
}

inline void add_players_radar(numerics::fvector WorldLocation, ImU32 color) // from UC
{
	static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	ImGui::Begin(("##radar"), nullptr, flags);

	int ScreenX, ScreenY = 0;
	const auto& GetWindowDrawList = ImGui::GetWindowDrawList();
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	CalcRadarPoint(WorldLocation, ScreenX, ScreenY);
	if (visuals::rect_radar)
	{
		drawList->AddCircleFilled(ImVec2(ScreenX, ScreenY), 4, color, 12);
		//drawList->AddRectFilled(ImVec2((float)ScreenX - 4.5, (float)ScreenY - 4.5), ImVec2((float)ScreenX + 4.5, (float)ScreenY + 4.5), ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }));
	}
	else drawList->AddCircleFilled(ImVec2(ScreenX, ScreenY), 5, color, 12);

	ImGui::End();
}

inline void fortnite_radar(float x, float y, float size, bool rect)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize, ImGuiCond_Once);
	static const auto flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
	ImGui::Begin("##radar", nullptr, flags);

	float centerX = x + size / 2;
	float centerY = y + size / 2;
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	// Transparent background - no filled shape

	// Draw plus (+) lines in white with no transparency
	drawList->AddLine(ImVec2(centerX, y), ImVec2(centerX, y + size), ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }), 1.5f);
	drawList->AddLine(ImVec2(x, centerY), ImVec2(x + size, centerY), ImGui::GetColorU32({ 1.f, 1.f, 1.f, 1.f }), 1.5f);

	// Draw black edges
	drawList->AddRect(ImVec2(x, y), ImVec2(x + size, y + size), ImGui::GetColorU32({ 0.f, 0.f, 0.f, 1.f }), 0.0f, 0, 2.0f);

	// Green center circle with no transparency
	drawList->AddCircleFilled(ImVec2(centerX, centerY), 6.f, ImGui::GetColorU32({ 0.f, 1.f, 0.f, 1.f }), 16);

	ImGui::End();
}

inline void add_players_radar(numerics::fvector WorldLocation)
{
	int ScreenX, ScreenY = 0;
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	CalcRadarPoint(WorldLocation, ScreenX, ScreenY);
	drawList->AddCircleFilled(ImVec2(ScreenX, ScreenY), 5, ImGui::GetColorU32({ 1.f, 0.f, 0.f, 1.f }), 12);
}





namespace updater {

	void run( )
	{
		while ( true )
		{
			auto uworld = read<uintptr_t>(Nunflaggedbase + offsets::Uworld);
			auto owning_game_instance = read<uintptr_t>( uworld + offsets::OwningGameInstance );
			auto local_player = read<uintptr_t>( read<uintptr_t>( owning_game_instance + offsets::LocalPlayers ) );
			auto player_controller = read<uintptr_t>( local_player + offsets::PlayerController );
			auto local_pawn = read<uintptr_t>( player_controller + offsets::Acknowledgedpawn );
			auto player_state = read<uintptr_t>(local_pawn + offsets::PlayerState);
			auto root_component = read<uintptr_t>( local_pawn + offsets::RootComponent );
			auto Spectator = read<numerics::ESpectatorArray>(player_state + 0xb88 + 0x108);

			udata::world_t = {
				uworld,
				owning_game_instance,
				local_player,
				player_controller,
				player_state,
				local_pawn,
				root_component,
				Spectator.Count,
				read<int>( player_state + offsets::TeamId )
			};

			auto game_state = read<uintptr_t>( uworld + offsets::GameState );
			auto player_array = read<numerics::tarray<uintptr_t>>( game_state + offsets::PlayerArray ).get_all( );

			udata::world_t.game_state = game_state;
			udata::world_t.player_array = player_array;

			std::vector<udata::actor> actors;
			actors.reserve( player_array.size( ) );
			udata::world_t.mesh = NULL;
			for ( const auto& player_state : player_array )
			{
				if ( !player_state ) continue;

				auto team_id = read<int>( player_state + offsets::TeamId );
				if ( team_id == udata::world_t.team_id ) continue;

				auto pawn_private = read<uintptr_t>( player_state + offsets::PawnPrivate );
				if ( !pawn_private ) continue;

				if ( pawn_private == udata::world_t.local_pawn ) continue;

				auto mesh = read<uintptr_t>( pawn_private + offsets::Mesh );

				if ( !mesh ) continue;

				actors.emplace_back( player_state , pawn_private , mesh , team_id );
			}

			udata::actor_t = std::move( actors );

			Sleep( 1200 );
		}
	}
	uintptr_t RootComponent(uintptr_t actor)
	{

		return read<uintptr_t>(actor + offsets::RootComponent);
	}
	numerics::fvector GetLocation(uintptr_t actor)
	{

		return read<numerics::fvector>(RootComponent(actor) + offsets::RelativeLocation);
	}
	void CacheLevels()
	{
		for (;;)
		{
			//if (!world::uncommon_loot or !world::chests) continue;
			//if (!udata::world_t.gworld) continue;
			std::vector<item> mrxd;
			uintptr_t ItemLevels = read<uintptr_t>(udata::world_t.gworld + offsets::Levels); // https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=Uworld[censored]&member=Levels

			//std::cout << "Levels - " << ItemLevels << std::endl;

			for (int i = 0; i < read<DWORD>(udata::world_t.gworld + (offsets::Levels + sizeof(PVOID))); ++i)
			{
				uintptr_t ItemLevel = read<uintptr_t>(ItemLevels + (i * sizeof(uintptr_t)));

				//std::cout << "ItemLevel - " << ItemLevel << std::endl;

				for (int i = 0; i < read<DWORD>(ItemLevel + (0x1A0 + sizeof(PVOID))); ++i) { //Actors

					uintptr_t ItemsPawns = read<uintptr_t>(ItemLevel + 0x1A0); //Actors

					//std::cout << "ItemsPawns - " << ItemsPawns << std::endl;

					uintptr_t CurrentItemPawn = read<uintptr_t>(ItemsPawns + (i * sizeof(uintptr_t)));
					//std::cout << "CurrentItemPawn - " << CurrentItemPawn << std::endl;

					numerics::fvector ItemPosition = GetLocation(CurrentItemPawn);

					float ItemDist = camera::location.distance(ItemPosition) / 100;
					int ItemIndex = read<int>(CurrentItemPawn + 0xC);
					//std::cout << "distance - " << ItemDist << std::endl;

					auto CurrentItemPawnName = FName::ToString(ItemIndex);
					std::cout << "CurrentItemPawnName - " << CurrentItemPawnName << std::endl;
					bool shouldUpdate;
					bool bIsPickup;

					if (is_valid(CurrentItemPawn))
					{

						if (world::uncommon_loot or world::chests)
						{
							bIsPickup = true;
							shouldUpdate = true;
						}
					}

					if (shouldUpdate)
					{
						item Item{ };
						Item.Actor = CurrentItemPawn;
						Item.Name = CurrentItemPawnName;
						Item.isPickup = bIsPickup;
						Item.distance = ItemDist;
						//std::cout << "distance - " << Item.distance << std::endl;
						//std::cout << "Name - " << CurrentItemPawnName << std::endl;

						mrxd.push_back(Item);

					}
				}
			}

			item_pawns.clear();
			item_pawns = mrxd;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
} // namespace updater


namespace visuals {
	void draw_actors( ImDrawList* drawlist ) {
		ImGui::PushFont( interFont );
		camera::update_camera( );

		auto closest_actor = udata::actor( 0 );
		closest_distance = FLT_MAX;  // Reset at the beginning
		udata::world_t.mesh = NULL;  // Reset mesh before finding a new one
		udata::world_t.closeest_actor = NULL;  // Reset closeest_actor before finding a new one

		auto aspect_ratio = static_cast< float >( numerics::screen_width ) / numerics::screen_height;
		world_to_screen::inv_fov = tanf( camera::fov * static_cast< float >( std::numbers::pi ) / 360.0f ) / aspect_ratio;
		world_to_screen::rotation_matrix = matrix_operations::create_rotation_matrix( camera::rotation );
		if (aimbot::fov_circle)
		{
			player::h_custom(aimbot::aim_fov, false, aimbot::RGB, false, aimbot::rainbowSpeed);
		}
		char watermark_text[64];
		sprintf_s(watermark_text, "Agents | %.1f", ImGui::GetIO().Framerate);
		drawlist->AddText(ImVec2(10, 10), IM_COL32(0, 255, 255, 255), watermark_text);
		if (visuals::Spectatorlist)
		{
			char Spectatorlist[256];
			sprintf_s(Spectatorlist, "Spectator: %d", udata::world_t.Spectator);
			ImVec2 screenSize = ImGui::GetIO().DisplaySize;
			ImVec2 textSize = ImGui::CalcTextSize(Spectatorlist);
			DrawString(drawlist, 20, (screenSize.x - textSize.x) * 0.5f, 30, IM_COL32(0, 255, 255, 255), true, visuals::outline, Spectatorlist);
		}
		for ( auto& current_actor : udata::actor_t ) {
			auto head = bones::get_bone_position( current_actor.mesh , 110 );
			auto head_projected = world_to_screen::project_to_screen( head );
			auto root = bones::get_bone_position( current_actor.mesh , 0 );
			auto root_projected = world_to_screen::project_to_screen( root );

			int box_height = abs( head_projected.y - head_projected.y );
			int box_width = static_cast< int >( box_height * 0.50f );
			float distance = camera::location.distance( root ) / 100.0f;
			if (distance > visuals::MaxRenderDistance) {
				continue;
			}
			ImVec2 top_left( root_projected.x - box_width / 2 , head_projected.y );
			ImVec2 bottom_right( root_projected.x + box_width / 2 , root_projected.y );

			auto player_bounds = player::get_player_bounds( current_actor.mesh );

			double dx = head_projected.x - numerics::screen_width_center;
			double dy = head_projected.y - numerics::screen_height_center;
			float dist = sqrtf(dx * dx + dy * dy);
			if (dist <= aimbot::aim_fov && dist < closest_distance)
			{
				closest_distance = dist;
				udata::world_t.mesh = current_actor.mesh;
				udata::world_t.closeest_actor = current_actor.pawn_private;
			}
			static ImColor box_visible;

			if ( aimbot::visible_check )
			{
				if (player::IsVisible(current_actor.mesh))
				{
					box_visible = ImGui::GetColorU32({ colors::box_visible[0], colors::box_visible[1], colors::box_visible[2],  1.0f }); // Cyan for visible
				}
				else
				{
					box_visible = ImGui::GetColorU32({ colors::box_invisible[0], colors::box_invisible[1], colors::box_invisible[2],  1.0f }); // Red for non-visible
				}
			}
			if ( visuals::boxes ) {
				box( drawlist , player_bounds , box_visible, visuals::thickness, visuals::outline );
			}
			if ( visuals::corner_boxes ) {
				cornered_box( drawlist , player_bounds , box_visible, visuals::outline, visuals::thickness );
			}
			if ( visuals::arrow )
			{
				numerics::fvector playerWorldPosition = bones::get_bone_position(current_actor.mesh, 0);

				numerics::fvector2d playerScreenPosition = world_to_screen::project_to_screen(playerWorldPosition);

				if (playerScreenPosition.x != 0.0f && playerScreenPosition.y != 0.0f)
				{
					player::DrawPlayerArrow(numerics::fvector2d(playerScreenPosition.x, playerScreenPosition.y), visuals::filledarrow, aimbot::aim_fov);
				}
			}
			if ( visuals::distance ) {
				char distance_string [ 64 ];
				sprintf_s( distance_string , "%.fm" , distance );
				ImVec2 text_size = ImGui::CalcTextSize( distance_string );
				drawlist->AddText( ImVec2( root_projected.x - text_size.x / 2 , root_projected.y - text_size.y / 2 ) , box_visible, distance_string );
			}
			if ( visuals::ranks ) {

				ImVec2 textPosition(head_projected.x + 18, head_projected.y - 80);
				ImVec2 textPosition2(head_projected.x - 18, head_projected.y - 109);

				uintptr_t habanero = read<uintptr_t>(current_actor.player_state + 0xa48); // AFortPlayerState - habenaro_component
				int32_t RankProgress = read<int32_t>(habanero + 0xd0 + 0x10);
				ImColor rankedColor = player::GetRankColor(RankProgress);
				ID3D11ShaderResourceView* ranktype = reinterpret_cast<ID3D11ShaderResourceView*>(player::GetRank(drawlist, RankProgress, textPosition, textPosition2, rankedColor));
			}
			if ( visuals::platform ) {
				std::string platformIcon = player::platform(current_actor.player_state);
				ImVec2 iconsize = ImGui::CalcTextSize(platformIcon.c_str());
				Icon(drawlist, icon, 17, head_projected.x - (iconsize.x / 2), head_projected.y - 65, player::Plat(platformIcon), false, visuals::outline, platformIcon.c_str());
			}
			if ( visuals::snapline )
			{
				drawlist->AddLine(ImVec2(numerics::screen_width / 2, 0), ImVec2(head_projected.x, head_projected.y), box_visible, visuals::thickness);
			}
			if ( visuals::skeleton) {
				player::DrawSkeleton(drawlist, visuals::curve, visuals::spaghetti, box_visible, visuals::thickness, visuals::outline, current_actor.mesh);
			}
			if ( visuals::username ) {
				std::string username_str = player::GetPlayerName(current_actor.player_state);
				if (!username_str.empty()) {
					std::string username_with_distance = username_str;
					ImVec2 text_size = ImGui::CalcTextSize(username_with_distance.c_str());
					float x_offset = text_size.x / 2.0f;
					DrawString(drawlist, 17, head_projected.x - x_offset, head_projected.y - 39, ImColor(box_visible), false, true, username_with_distance.c_str());
				}
			}

			numerics::fvector bottom3d = bones::get_bone_position(current_actor.mesh, 0);
			if (visuals::radaresp)
			{
				float radarX = visuals::radar_position_x;
				float radarY = visuals::radar_position_y;
				float radarSize = visuals::radar_size;
				bool isRect = visuals::rect_radar;
				fortnite_radar(visuals::radar_position_x, visuals::radar_position_y, visuals::radarsize, visuals::rect_radar);
				numerics::fvector convertedPosition = numerics::fvector(root.x, root.y, root.z);
				ImU32 playerColor = ImGui::GetColorU32(ImVec4(1.0f, 0.0f, 0.0f, 1.0f)); // Rojo
				add_players_radar(bottom3d, playerColor);
			}
		}
		if ( aimbot::enabled)
		{
			//if (udata::world_t.local_pawn) {
			//	printf("udata::world_t.local_pawn assigned: 0x%llx\n", udata::world_t.local_pawn);
			//}
			//else {
			//	printf("udata::world_t.local_pawn is NULL!\n");
			//}
			if (GetAsyncKeyState(aimbot::aimkey))
			{
				inp::aimbot(udata::world_t.mesh);
			}
		}
		if (aimbot::prediction)
		{
			if (GetAsyncKeyState(aimbot::aimkey)) inp::predict(udata::world_t.mesh, udata::world_t.closeest_actor, udata::world_t.local_pawn);
		}

		ImGui::PopFont( );
	}
	void WorldLoop(ImDrawList* drawlist)
	{
		auto levelListCopy = item_pawns;

		for (auto entity : levelListCopy)
		{
			if (!entity.Actor) continue;

			if ((world::uncommon_loot || world::common_loot ||
				world::rare_loot || world::epic_loot ||
				world::legendary_loot || world::mythic_loot) &&
				(entity.Name.find("FortPickupAthena") != std::string::npos ||
					entity.Name.find("Fort_Pickup_Creative_C") != std::string::npos ||
					entity.Name.find("FortPickup") != std::string::npos))
			{
				if (entity.distance > world::render_distance_loot) continue;

				uintptr_t actor_root_component = read<uintptr_t>(entity.Actor + offsets::RootComponent);
				if (!actor_root_component) continue;

				numerics::fvector actor_relative_location = read<numerics::fvector>(actor_root_component + offsets::RelativeLocation);
				numerics::fvector2d location = world_to_screen::project_to_screen(actor_relative_location);
				float item_distance = camera::location.distance(actor_relative_location) * 0.01f;
				if (!is_in_screen(location)) continue;

				uintptr_t definition = read<uintptr_t>(entity.Actor + offsets::PrimaryPickupItemEntry + offsets::ItemDefinition);
				if (!definition) continue;

				EFortRarity tier = read<EFortRarity>(definition + offsets::Rarity);
				uint64_t ItemName = read<uint64_t>(definition + offsets::ItemName);
				uint64_t FData = read<uint64_t>(ItemName + 0x20);
				int FLength = read<int>(ItemName + 0x28);

				if (FLength > 0 && FLength < 50)
				{
					std::unique_ptr<wchar_t[]> WeaponBuffer(new wchar_t[FLength]);
					Driver::ReadPhysical(PVOID(FData), (PVOID)WeaponBuffer.get(), FLength * sizeof(wchar_t));

					std::wstring wstr_buf(WeaponBuffer.get());
					std::string weapon_name(wstr_buf.begin(), wstr_buf.end());

					std::unordered_map<EFortRarity, bool> loot_settings = {
						{EFortRarity::EFortRarity__Common, world::common_loot},
						{EFortRarity::EFortRarity__Uncommon, world::uncommon_loot},
						{EFortRarity::EFortRarity__Rare, world::rare_loot},
						{EFortRarity::EFortRarity__Epic, world::epic_loot},
						{EFortRarity::EFortRarity__Legendary, world::legendary_loot},
						{EFortRarity::EFortRarity__Mythic, world::mythic_loot},
					};

					if (loot_settings[tier]) {
						char dist[64];
						sprintf_s(dist, "%.fm", item_distance);
						std::string text = weapon_name + " (" + std::string(dist) + ")";
						ImVec2 text_size = ImGui::CalcTextSize(text.c_str());
						ImVec2 text_pos = ImVec2(location.x - text_size.x / 2, location.y - 18);

						ImColor textColor = get_loot_color(tier);
						ImColor outlineColor = ImColor(0, 0, 0, 255);

						if (world::enableOutlinesLoot) {

							draw_text(drawlist, ImGui::GetFont(), 16, ImVec2(text_pos.x - 1, text_pos.y), outlineColor, text.c_str());
							draw_text(drawlist, ImGui::GetFont(), 16, ImVec2(text_pos.x + 1, text_pos.y), outlineColor, text.c_str());
							draw_text(drawlist, ImGui::GetFont(), 16, ImVec2(text_pos.x, text_pos.y - 1), outlineColor, text.c_str());
							draw_text(drawlist, ImGui::GetFont(), 16, ImVec2(text_pos.x, text_pos.y + 1), outlineColor, text.c_str());
						}
						draw_text(drawlist, ImGui::GetFont(), 16, text_pos, textColor, text.c_str());
					}
				}
			}

			if (world::chests && strstr(entity.Name.c_str(), "Tiered_Chest") && entity.distance <= world::render_distance_chest)
			{
				if (!(read<BYTE>(entity.Actor + offsets::bAlreadySearched) >> 2 & 1))
				{
					draw_item(drawlist, "Tiered_Chest", "Chest", ImColor(255, 255, 0, 255), entity.Actor);
				}
			}

			if (world::llamas && strstr(entity.Name.c_str(), "AthenaSupplyDrop_Llama_C") && entity.distance <= world::render_distance_llama)
			{
				if (!(read<BYTE>(entity.Actor + offsets::bAlreadySearched) >> 2 & 1))
				{
					draw_item(drawlist, "AthenaSupplyDrop_Llama_C", "Llama", ImColor(204, 0, 255, 255), entity.Actor);
				}
			}

			if (world::foodbox && strstr(entity.Name.c_str(), "FoodBox_Produce_Athena_C") && entity.distance <= world::render_distance_ammo)
			{
				if (!(read<BYTE>(entity.Actor + offsets::bAlreadySearched) >> 2 & 1))
				{
					draw_item(drawlist, "FoodBox_Produce_Athena_C", "Food Box", ImColor(0, 255, 21, 255), entity.Actor);
				}
			}

			if (world::ammobox && strstr(entity.Name.c_str(), "Tiered_Ammo_Athena_C") && entity.distance <= world::render_distance_ammo)
			{
				if (!(read<BYTE>(entity.Actor + offsets::bAlreadySearched) >> 2 & 1))
				{
					draw_item(drawlist, "Tiered_Ammo_Athena_C", "Ammo Box", ImColor(0, 100, 0, 255), entity.Actor);
				}
			}

			if ((strstr(entity.Name.c_str(), "Valet_BasicSUV_Vehicle_C") ||
				strstr(entity.Name.c_str(), "Valet_SportsCar_Vehicle_C") ||
				strstr(entity.Name.c_str(), "Motorcycle_Sport_Vehicle_C"))
				&& world::vehicle
				&& entity.distance <= world::render_distance_vehicle)
			{
				draw_item(drawlist, entity.Name.c_str(), "Vehicle", ImColor(255, 0, 0, 255), entity.Actor);
			}

			if (((world::spirits && strstr(entity.Name.c_str(), "NPC_Pawn_Irwin_Simple_Sprite_Air_C")) ||
				(world::spirits && strstr(entity.Name.c_str(), "NPC_Pawn_Irwin_Simple_Sprite_Water_C")))
				&& entity.distance <= world::render_distance_spirits)
			{
				const char* itemName = strstr(entity.Name.c_str(), "Air_C") ? "Air Spirit" : "Water Spirit";
				ImColor itemColor = strstr(entity.Name.c_str(), "Air_C") ? ImColor(255, 255, 255, 255) : ImColor(0, 153, 255, 255);

				draw_item(drawlist, entity.Name.c_str(), itemName, itemColor, entity.Actor);
			}
			//ImGui::PopFont();
		}
	}
	void leveldrawing(ImDrawList* drawlist)
	{
		auto levelListCopy = item_pawns;

		for (auto entity : levelListCopy)
		{
			if (entity.Actor)
			{
				if (world::uncommon_loot && strstr(entity.Name.c_str(), ("FortPickupAthena")) or strstr(entity.Name.c_str(), ("Fort_Pickup_Creative_C")) or strstr(entity.Name.c_str(), ("FortPickup")))
				{
					numerics::fvector Location = updater::GetLocation(entity.Actor);

					numerics::fvector2d ChestPosition = world_to_screen::project_to_screen(Location);

					if (numerics::is_in_screen(ChestPosition))
					{
						std::string wtf2 = " [" + std::to_string((int)entity.distance) + ("m]");
						ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
						DrawString(drawlist, 13, ChestPosition.x - (text_size.x / 2), ChestPosition.y - 18, ImVec4(0.0f, 1.0f, 1.0f, 1.0f), false, false, wtf2.c_str());
					}
				}

				if (world::chests && strstr(entity.Name.c_str(), ("Tiered_Chest")))
				{
					numerics::fvector Location = updater::GetLocation(entity.Actor);
					numerics::fvector2d Screen = world_to_screen::project_to_screen(Location);

					if (numerics::is_in_screen(Screen))
					{
						std::string BuiltString = "Chest [" + std::to_string((int)entity.distance) + "]";
						ImVec2 text_size = ImGui::CalcTextSize(BuiltString.c_str());

						DrawString(drawlist, 13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 0), false, false, BuiltString.c_str());
					}
				}
			}
		}
	}
}