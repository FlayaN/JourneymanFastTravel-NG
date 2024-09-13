#include "FastTravelManager.h"
#include "Settings.h"
#include "Hooks.h"

bool FastTravelManager::InstallCanFastTravelCheck()
{
	auto& trampoline = SKSE::GetTrampoline();
	_CanFastTravelMap = trampoline.write_call<5>(Hooks::Can_Fast_Travel_Map_Hook.address(), CanFastTravelMap);
	logger::info("Installed ft hook");
	return true;
}

bool FastTravelManager::InstallPapyrusFastTravelCheck()
{
	auto& trampoline = SKSE::GetTrampoline();
	_GetWorldSpaceHook = trampoline.write_call<5>(Hooks::Papyrus_Fast_Travel_Hook.address(), GetWorldSpaceHook);
	logger::info("Installed papyrus ft hook");
	return true;
}

bool FastTravelManager::InstallFastTravelEndVREvent()
{
	auto& trampoline = SKSE::GetTrampoline();
	_FastTravelVREnd = trampoline.write_call<5>(Hooks::Fast_Travel_End_VR_Hook.address(), FastTravelVREnd);
	logger::info("Installed ft end event vr hook");
	return true;
}

RE::TESWorldSpace* FastTravelManager::GetWorldSpaceHook(RE::TESObjectREFR* a1)
{
	if (auto settings = Settings::GetSingleton()) {
		settings->menuFastTravel = false;
	}

	return _GetWorldSpaceHook(a1);
}

bool FastTravelManager::CanFastTravelMap(RE::Actor* a_actor, bool a_bool)
{
	auto inv = a_actor->GetInventory();
	auto settings = Settings::GetSingleton();
	auto player = RE::PlayerCharacter::GetSingleton();

	if ((!settings->EnableOnlyOnSM || settings->Survival_ModeEnabledShared == 1.0f) && !player->IsGodMode()) {
		
		if (IsOnFlyingMount(a_actor)) {
			return true;
		}

		for (const auto& [item, data] : inv) {
			if (settings->RequiredItems->HasForm(item->GetFormID())) {

				if (_CanFastTravelMap(a_actor, a_bool)) {
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
		return IsOnFlyingMount(a_actor) || _CanFastTravelMap(a_actor, a_bool);
	}

	//If you get to here you didnt have the required item/s and you can't travel
	RE::DebugNotification(settings->RequiredItemNotFoundMessage.c_str());
	return false;
}

void FastTravelManager::FastTravelVREnd(RE::AIProcess* a_process)
{
	_FastTravelVREnd(a_process);
	auto settings = Settings::GetSingleton();
	if (settings->EnableOnlyOnSM == true && settings->Survival_ModeEnabledShared == 0.0f) {
		settings->menuFastTravel = false;
		return;
	}

	auto player = RE::PlayerCharacter::GetSingleton();
	if (settings->menuFastTravel && !FastTravelManager::IsOnFlyingMount(player)) {
		auto inv = player->GetInventory();
		for (const auto& [item, data] : inv) {
			if (settings->RequiredItems->HasForm(item->GetFormID())) {

				player->RemoveItem(
					item,
					1,
					RE::ITEM_REMOVE_REASON::kRemove,
					nullptr,
					nullptr
				);

				settings->needToShowRemoveMessage = true;
				break;
			}
		}
	}
	settings->menuFastTravel = false;
}
