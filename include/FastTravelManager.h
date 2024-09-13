#pragma once

class FastTravelManager
{
public:

	static bool InstallCanFastTravelCheck();
	static bool InstallPapyrusFastTravelCheck();
	static bool InstallFastTravelEndVREvent();

	static bool IsOnFlyingMount(RE::Actor* a_actor)
	{
		using func_t = decltype(&FastTravelManager::IsOnFlyingMount);
		static REL::Relocation<func_t> func{ RELOCATION_ID(36877, 37901) };
		return func(a_actor);
	}

	static bool CanFastTravel(RE::Actor* a_actor, bool a_arg2)
	{
		using func_t = decltype(&FastTravelManager::CanFastTravel);
		static REL::Relocation<func_t> func{ RELOCATION_ID(39372, 40444) };
		return func(a_actor, a_arg2);
	}

	static bool IsFastTravelEnabled()
	{
		using func_t = decltype(&FastTravelManager::IsFastTravelEnabled);
		REL::Relocation<func_t> func{ RELOCATION_ID(54848, 55481) };
		return func();
	}

	static void EnableFastTravelSE(void* a1, void* a2, void* a3, bool a_enable)
	{
		using func_t = decltype(&FastTravelManager::EnableFastTravelSE);
		REL::Relocation<func_t> func{ REL::ID(54946) };
		return func(a1, a2, a3, a_enable);
	}

	static void EnableFastTravelAE(bool a_enable)
	{
		using func_t = decltype(&FastTravelManager::EnableFastTravelAE);
		REL::Relocation<func_t> func{ REL::ID(55563) };
		return func(a_enable);
	}

	inline static RE::TESWorldSpace* GetWorldSpaceHook(RE::TESObjectREFR* a1);
	inline static REL::Relocation<decltype(GetWorldSpaceHook)> _GetWorldSpaceHook;

	inline static bool CanFastTravelMap(RE::Actor* actor, bool a_bool);
	inline static REL::Relocation<decltype(CanFastTravelMap)> _CanFastTravelMap;

	inline static void FastTravelVREnd(RE::AIProcess* a_process);
	inline static REL::Relocation<decltype(FastTravelVREnd)> _FastTravelVREnd;
};