#pragma once
#include <include.h>
#include <src/engine/sdk.h>
#include <thirdparty/imgui/imgui.h>
#include <thirdparty/imgui/imgui_impl_win32.h>
#include <thirdparty/imgui/imgui_impl_dx11.h>
#include <thirdparty/imgui/imgui_internal.h>
#include <src/aimbot/aimbot.h>
#include <mutex>
#include <atomic>
#include <vector>

inline ImFont* interFont;
inline ImFont* icon;


class item {
public:
    uintptr_t
        Actor;

    std::string
        Name,
        isVehicle,
        isChest,
        isLamma,
        isPickup,
        isAmmoBox;
    float
        distance;
};
inline std::vector<item> item_pawns;

namespace updater {

    void run( );
    void env_run( );
    void CacheLevels();
} 

namespace aimbot {
    inline bool enabled = true;
    inline bool visible_check = true;
    inline bool prediction = false;
    inline int aim_fov = 100;
    inline int rainbowSpeed = 4;
    inline int smooth = 1;
    inline int hitbox = 0;
    inline bool fov_circle = true;
    inline bool RGB = true;
    inline int aimkey = VK_RBUTTON;

}

namespace visuals {
    void draw_actors( ImDrawList* drawlist );
    void WorldLoop(ImDrawList* drawlist);
    void leveldrawing(ImDrawList* drawlist);
    inline bool boxes = true;
    inline bool corner_boxes = false;
    inline int thickness = 1;
    inline int MaxRenderDistance = 278;

    inline bool outline = false;
    inline bool distance = true;
    inline bool snapline = false;
    inline bool Spectatorlist = false;
    inline bool ranks = false;
    inline bool platform = false;
    inline bool username = false;
    inline bool skeleton = false;
    inline bool curve = false;
    inline bool spaghetti = false;
    inline bool filledarrow = false;
    inline bool arrow = false;
    inline bool radaresp = true;
    inline bool ShowRadar = true;
    inline bool rect_radar = true;
    inline int radar_position_x = 10;
    inline int radar_position_y =  350;
    inline int radar_size = 450;
    inline int RadarDistance = 200;
    inline int radarsize = 300;

}

namespace world
{
    inline bool uncommon_loot = false;
    inline bool common_loot = false;
    inline bool llamas = false;
    inline bool rare_loot = false;
    inline bool foodbox = false;
    inline bool epic_loot = false;
    inline bool legendary_loot = false;
    inline bool mythic_loot = false;
    inline bool chests = false;
    inline bool ammobox = false;
    inline bool vehicle = false;
    inline bool spirits = false;
    inline int render_distance_loot = 350;
    inline int render_distance_ammo = 350;
    inline int render_distance_vehicle = 350;
    inline int render_distance_spirits = 350;
    inline int render_distance_llama = 350;
    inline int render_distance_chest = 350;
    inline bool enableOutlinesContainers = false;
    inline bool enableOutlinesLoot = false;
}

namespace misc {
    inline bool vsync = false;
}
namespace colors
{
    inline float box_visible[4] = { 0.f, 1.f, 1.f, 1.f };
    inline float box_invisible[4] = { 1.f, 0.f, 0.f, 1.f };
}
