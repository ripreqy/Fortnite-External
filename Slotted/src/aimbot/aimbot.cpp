#include "aimbot.h"
void inp::aimbot(uintptr_t target_mesh)
{
	if (aimbot::visible_check)
	{
		if (!target_mesh || !player::IsVisible(target_mesh)) 
			return;
	}
	numerics::fvector head3d;
	switch (aimbot::hitbox) {
	case 0:

		head3d = bones::get_bone_position(target_mesh, 110);; //head
		break;
	case 1:

		head3d = bones::get_bone_position(target_mesh, 67);; //Neck
		break;
	case 2:

		head3d = bones::get_bone_position(target_mesh, 66);; //Chest
		break;
	case 3:

		head3d = bones::get_bone_position(target_mesh, 2);; //Pelvis
		break;
	}
	numerics::fvector2d head2d = world_to_screen::project_to_screen(head3d);
	numerics::fvector2d target{};
	if (head2d.x != 0)
	{
		if (head2d.x > numerics::screen_width_center) //x
		{
			target.x = -(numerics::screen_width_center - head2d.x);
			target.x /= aimbot::smooth;
			if (target.x + numerics::screen_width_center > numerics::screen_width_center * 2) target.x = 0;
		}
		if (head2d.x < numerics::screen_width_center)
		{
			target.x = head2d.x - numerics::screen_width_center;
			target.x /= aimbot::smooth;
			if (target.x + numerics::screen_width_center < 0) target.x = 0;
		}
	}
	if (head2d.y != 0)
	{
		if (head2d.y > numerics::screen_height_center)
		{
			target.y = -(numerics::screen_height_center - head2d.y);
			target.y /= aimbot::smooth;
			if (target.y + numerics::screen_height_center > numerics::screen_height_center * 2) target.y = 0;
		}
		if (head2d.y < numerics::screen_height_center)
		{
			target.y = head2d.y - numerics::screen_height_center;
			target.y /= aimbot::smooth;
			if (target.y + numerics::screen_height_center < 0) target.y = 0;
		}
	}
	Driver::MoveMouse(target.x, target.y, Mouse_NONE);
}

void inp::predict(uintptr_t target_mesh, uintptr_t target_actor, uintptr_t localpawn)
{
    if (!target_mesh) return;
    if (!target_mesh || !player::IsVisible(target_mesh))
        return;

    numerics::fvector head3d = bones::get_bone_position(target_mesh, 110); //head
    numerics::fvector2d rootHeadOut;
    if (aimbot::prediction)
    {

        uint64_t player_weapon = read<uint64_t>(localpawn + 0xa80);//currentweapon
        if (player_weapon) {
            uint64_t weapon_data = read<uint64_t>(player_weapon + 0x570);//weapondata
            if (weapon_data) {
                uint64_t ftext_ptr = read<uint64_t>(weapon_data + 0x40);
                if (ftext_ptr) {
                    uint64_t ftext_data = read<uint64_t>(ftext_ptr + 0x20);
                    int ftext_length = read<int>(ftext_ptr + 0x28);
                    if (ftext_length > 0 && ftext_length < 50) {
                        wchar_t* ftext_buf = new wchar_t[ftext_length];
                        Driver::ReadPhysical((PVOID)(uintptr_t)ftext_data, (PVOID)ftext_buf, ftext_length * sizeof(wchar_t));
                        /*float Speed = read<float>(player_weapon + 0x1ad0);
                        float Gravity = read<float>(player_weapon + 0x1d5c);*/
                        float Speed = read<float>(player_weapon + 0x1e64);
                        float Gravity = read<float>(player_weapon + 0x1e68);

                        //std::cout << "Speed: " << Speed << std::endl;
                        //std::cout << "Gravity: " << Gravity << std::endl;

                        if (wcsstr(ftext_buf, L"Heavy Sniper Rifle") || wcsstr(ftext_buf, L"Hunter Bolt-Action Sniper")) {
                           // Speed = 70000.f;
                            //Gravity = 2.5f;
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Hunting Rifle")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Veiled Precision SMG") || wcsstr(ftext_buf, L"Surgefire SMG")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Suppressed Pistol")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Collateral Damage Assault Rifle")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Twister Assault Rifle") || wcsstr(ftext_buf, L"Enhanced Holo Twister Assault Rifle")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Pulse Rifle")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Falcon Eye Sniper")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Pulse Rifle")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Spectre Pistol")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Phantom Sniper Rifle")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Reaper Sniper Rifle") || wcsstr(ftext_buf, L"Rifle de francotirador Segador")) {
                            //Speed = 70000.f;
                            //Gravity = 3.5f;
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Vector SMG") || wcsstr(ftext_buf, L"SMG Vector")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Striker AR") || wcsstr(ftext_buf, L"Striker Assault Rifle")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        if (wcsstr(ftext_buf, L"Ranger Pistol")) {
                            auto currentnigga = read<uintptr_t>(localpawn + 0xa80);
                            Speed = read<float>(currentnigga + 0x1e64);
                            Gravity = read<float>(currentnigga + 0x1e68);
                        }
                        //std::cout << "Speed: " << Speed << std::endl;
                        //std::cout << "Gravity: " << Gravity << std::endl;

                        if (Speed > 0.1 && Gravity > 0.1) {
                            float distance = camera::location.distance(head3d);
                            auto root_component = read<uintptr_t>(target_actor + 0x1B0);//root
                            numerics::fvector Veloctiy = read<numerics::fvector>(root_component + 0x180);
                            head3d = player::PredictLocation(head3d, Veloctiy, Speed, Gravity, camera::location.distance(head3d));//https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=USceneComponent&member=ComponentVelocity
                            rootHeadOut = world_to_screen::project_to_screen(head3d);
                        }
                    }
                }

            }
        }


    }
    else
    {
        rootHeadOut = world_to_screen::project_to_screen(head3d);
    }
    numerics::fvector2d target{};
    if (rootHeadOut.x != 0)
    {
        if (rootHeadOut.x > numerics::screen_width_center) //x
        {
            target.x = -(numerics::screen_width_center - rootHeadOut.x);
            target.x /= aimbot::smooth;
            if (target.x + numerics::screen_width_center > numerics::screen_width_center * 2) target.x = 0;
        }
        if (rootHeadOut.x < numerics::screen_width_center)
        {
            target.x = rootHeadOut.x - numerics::screen_width_center;
            target.x /= aimbot::smooth;
            if (target.x + numerics::screen_width_center < 0) target.x = 0;
        }
    }
    if (rootHeadOut.y != 0)
    {
        if (rootHeadOut.y > numerics::screen_height_center)
        {
            target.y = -(numerics::screen_height_center - rootHeadOut.y);
            target.y /= aimbot::smooth;
            if (target.y + numerics::screen_height_center > numerics::screen_height_center * 2) target.y = 0;
        }
        if (rootHeadOut.y < numerics::screen_height_center)
        {
            target.y = rootHeadOut.y - numerics::screen_height_center;
            target.y /= aimbot::smooth;
            if (target.y + numerics::screen_height_center < 0) target.y = 0;
        }
    }
    Driver::MoveMouse(target.x, target.y, Mouse_NONE);
}
