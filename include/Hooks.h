#pragma once

#include "FastTravelManager.h"

namespace Hooks
{
	inline static REL::Relocation<std::uintptr_t> Can_Fast_Travel_Map_Hook{ RELOCATION_ID(52208, 53095), REL::Relocate(0x2C5, 0x328, 0x358) };
	inline static REL::Relocation<std::uintptr_t> Papyrus_Fast_Travel_Hook{ RELOCATION_ID(54824, 55457), REL::Relocate(0xA1, 0xA1, 0xA4) };
	inline static REL::Relocation<std::uintptr_t> Fast_Travel_End_VR_Hook{ REL::ID(39373), 0xA22 };

	inline bool Install() {
		if (!FastTravelManager::InstallCanFastTravelCheck()) {
			return false;
		}
		if (!FastTravelManager::InstallPapyrusFastTravelCheck()) {
			return false;
		}
		if (!FastTravelManager::InstallFastTravelEndVREvent()) {
			return false;
		}
		return true;
	}
}