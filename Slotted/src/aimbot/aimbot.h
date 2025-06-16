#pragma once
#include "../engine/sdk.h"
#include "../cache/index.h"



namespace inp {
	void aimbot(uintptr_t target_mesh);
	void predict(uintptr_t target_mesh, uintptr_t target_actor, uintptr_t localpawn);
}