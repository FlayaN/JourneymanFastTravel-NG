#include "Hooks.h"
#include "Settings.h"
#include "FastTravelManager.h"

namespace Hooks {

	struct CanFastTravel
	{
		static bool thunk(RE::Actor* a_actor, bool a_bool)
		{
			auto inv = a_actor->GetInventory();
			auto settings = Settings::GetSingleton();
			auto player = RE::PlayerCharacter::GetSingleton();

			if ((!settings->EnableOnlyOnSM || settings->Survival_ModeEnabledShared == 1.0f) && !player->IsGodMode()) {

				if (FastTravelManager::IsOnFlyingMount(a_actor)) {
					return true;
				}

				for (const auto& [item, data] : inv) {
					if (settings->RequiredItems->HasForm(item->GetFormID())) {

						if (func(a_actor, a_bool)) {
							settings->menuFastTravel = true;
							return true;
						}
						else {
							return false;
						}
					}
				}
			}
			else {
				return FastTravelManager::IsOnFlyingMount(a_actor) || func(a_actor, a_bool);
			}

			//If you get to here you didnt have the required item/s and you can't travel
			RE::DebugNotification(settings->RequiredItemNotFoundMessage.c_str());
			return false;
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static bool thunkVR(RE::Actor* a_actor, char** a_errorText)
		{
			auto inv = a_actor->GetInventory();
			auto settings = Settings::GetSingleton();
			auto player = RE::PlayerCharacter::GetSingleton();

			if ((!settings->EnableOnlyOnSM || settings->Survival_ModeEnabledShared == 1.0f) && !player->IsGodMode()) {

				if (FastTravelManager::IsOnFlyingMount(a_actor)) {
					return true;
				}

				for (const auto& [item, data] : inv) {
					if (settings->RequiredItems->HasForm(item->GetFormID())) {

						if (funcVR(a_actor, a_errorText)) {
							settings->menuFastTravel = true;
							return true;
						}
						else {
							return false;
						}
					}
				}
			}
			else {
				return FastTravelManager::IsOnFlyingMount(a_actor) || funcVR(a_actor, a_errorText);
			}

			//If you get to here you didnt have the required item/s and you can't travel
			if (a_errorText) {
				RE::DebugMessageBox(settings->RequiredItemNotFoundMessage.c_str());
			}
			else {
				RE::DebugNotification(settings->RequiredItemNotFoundMessage.c_str());
			}
			return false;
		}
		static inline REL::Relocation<decltype(thunkVR)> funcVR;

		static void Install()
		{
			static REL::Relocation<std::uintptr_t> Can_Fast_Travel_Map_Hook{ REL::RelocationID(52208, 53095), REL::Relocate(0x2C5, 0x328, 0x358) };

			auto& trampoline = SKSE::GetTrampoline();
			if (REL::Module::IsVR())
			{
				funcVR = trampoline.write_call<5>(Can_Fast_Travel_Map_Hook.address(), thunkVR);
			}
			else {
				func = trampoline.write_call<5>(Can_Fast_Travel_Map_Hook.address(), thunk);
			}
			logger::info("Installed CanFastTravel hook");
		}
	};

	struct PapyrusFastTravelCheck
	{
		static RE::TESWorldSpace* thunk(RE::TESObjectREFR* a1)
		{
			if (auto settings = Settings::GetSingleton()) {
				settings->menuFastTravel = false;
			}
			return func(a1);
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			static REL::Relocation<std::uintptr_t> Papyrus_Fast_Travel_Hook{ REL::RelocationID(54824, 55457), REL::Relocate(0xA1, 0xA1, 0xA4) };
			auto& trampoline = SKSE::GetTrampoline();
			func = trampoline.write_call<5>(Papyrus_Fast_Travel_Hook.address(), thunk);
			logger::info("Installed papyrus ft hook");
		}
	};

	struct FastTravelEndVREvent
	{
		static void thunk(RE::AIProcess* a_process)
		{
			func(a_process);
			FastTravelManager::PostFastTravel();
		}
		static inline REL::Relocation<decltype(thunk)> func;

		static void Install()
		{
			static REL::Relocation<std::uintptr_t> Fast_Travel_End_VR_Hook{ REL::ID(39373), 0xA22 };
			auto& trampoline = SKSE::GetTrampoline();
			func = trampoline.write_call<5>(Fast_Travel_End_VR_Hook.address(), thunk);
			logger::info("Installed ft end event vr hook");
		}
	};

	void Install()
	{
		CanFastTravel::Install();
		PapyrusFastTravelCheck::Install();
		if (REL::Module::IsVR())
		{
			FastTravelEndVREvent::Install();
		}
	}
}
