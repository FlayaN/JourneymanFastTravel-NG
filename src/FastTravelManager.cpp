#include "FastTravelManager.h"
#include "Settings.h"

RE::BSEventNotifyControl FastTravelManager::PostFastTravel()
{
	auto settings = Settings::GetSingleton();
	if (settings->EnableOnlyOnSM == true && settings->Survival_ModeEnabledShared == 0.0f) {
		settings->menuFastTravel = false;
		return RE::BSEventNotifyControl::kContinue;
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
	return RE::BSEventNotifyControl::kContinue;
}
