#pragma once

#include <SimpleIni.h>

class Settings
{
public:
	static Settings* GetSingleton()
	{
		static Settings playerStatus;
		return &playerStatus;
	}

	void LoadSettings() 
	{
		logger::info("Loading settings");
		auto settings = Settings::GetSingleton();

		CSimpleIniA ini;
		ini.SetUnicode();
		ini.LoadFile(R"(.\Data\SKSE\Plugins\Journeyman.ini)");

		std::string ft(ini.GetValue("","sNoTravelPackMessage",""));
		std::string tpr(ini.GetValue("","sTravelPackRemoveMessage","FILL TRAVEL PACK MESSAGE IN INI"));

		EnableOnlyOnSM = ini.GetBoolValue("", "bEnableOnlyOnSurvivalEnabled");

		RequiredItemNotFoundMessage = ft;
		TravelPackRemoveMessage = tpr;
		logger::info("Message loaded");

		if (settings->RequiredItemNotFoundMessage == "") {
			logger::info("MESSAGE NOT FOUND");
		}

		RequiredItems = RE::TESDataHandler::GetSingleton()->LookupForm(RE::FormID(0x800), "Journeyman.esp")->As<RE::BGSListForm>();
		MAG_IsFeatureLocked = RE::TESDataHandler::GetSingleton()->LookupForm(RE::FormID(0x80A), "Journeyman.esp")->As<RE::TESGlobal>();
		UpdateFeatureLocked();
		if(REL::Module::IsVR())
		{
			Survival_ModeEnabledShared = EnableOnlyOnSM;
		}
		else
		{
			Survival_ModeEnabledShared = RE::TESDataHandler::GetSingleton()->LookupForm(RE::FormID(0x314A), "Update.esm")->As<RE::TESGlobal>()->value;
		}
		logger::info("Settings loaded");
	}

	void UpdateFeatureLocked()
	{
		MAG_IsFeatureLocked->value = EnableOnlyOnSM;
	}

	RE::BGSListForm* RequiredItems;
	float Survival_ModeEnabledShared;
	RE::TESGlobal* MAG_IsFeatureLocked;

	bool needToShowRemoveMessage = false;
	bool menuFastTravel = false;
	std::string RequiredItemNotFoundMessage = "";
	std::string TravelPackRemoveMessage = "";
	bool EnableOnlyOnSM = false;
};