#include <polyhook2/Detour/x64Detour.hpp>
#include "GameClasses.hpp"

class MethodInfo;
class ItemDropCheck_o;

bool BananaClickedDrop = false;
ItemDropCheck_o* itemDropThis = nullptr;
MethodInfo* itemDropMethod = nullptr;

bool BananaClickedAddMoney = false;
UpdateMoneys_o* addMoneyThis = nullptr;
MethodInfo* addMoneyMethod = nullptr;

uint64_t ItemDropCheck_TriggerItemsO = 0;
uint64_t UpdateMoneys_TickSystemO = 0;

void __stdcall UpdateMoneys_TickSystem(UpdateMoneys_o* this_, MethodInfo* method)
{
	addMoneyThis = this_;
	addMoneyMethod = method;
	BananaClickedAddMoney = true;

	return PLH::FnCast(UpdateMoneys_TickSystemO, UpdateMoneys_TickSystem)(this_, method);
}

void __stdcall ItemDropCheck_TriggerItems(ItemDropCheck_o* this_, MethodInfo* method)
{
	itemDropThis = this_;
	itemDropMethod = method;
	BananaClickedDrop = true;

	return PLH::FnCast(ItemDropCheck_TriggerItemsO, ItemDropCheck_TriggerItems)(this_, method);
}

void bananaThread(const std::stop_token& stopToken)
{
	const auto base = (uint64_t)GetModuleHandleA("GameAssembly.dll");

	const auto dosHeader = (IMAGE_DOS_HEADER*)(base);
	const auto ntHeader = (IMAGE_NT_HEADERS64*)(base + dosHeader->e_lfanew);
	const auto moduleSize = ntHeader->OptionalHeader.SizeOfImage;

	const auto ItemDropCheck_TriggerItemsP = PLH::findPattern(base, moduleSize, "40 53 48 83 EC 20 48 8B D9 33 D2 B9");
	const auto UpdateMoneys_TickSystemP = PLH::findPattern(base, moduleSize,
	                                                       "40 53 48 83 EC 60 80 3D ?? ?? ?? ?? ?? 48 8B D9 0F 29 74 24 ?? 75 ?? 48 8D 0D");
	const auto UpdateMoneys_AddMoneyP = PLH::findPattern(base, moduleSize,
	                                                     "48 89 5C 24 ?? 57 48 83 EC 20 80 3D ?? ?? ?? ?? ?? 48 8B F9 75 13 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 48 8B 47 30");

	PLH::x64Detour TriggerItemsHook(ItemDropCheck_TriggerItemsP, (uint64_t)ItemDropCheck_TriggerItems,
	                                &ItemDropCheck_TriggerItemsO);
	PLH::x64Detour TickSystemHook(UpdateMoneys_TickSystemP, (uint64_t)UpdateMoneys_TickSystem,
	                              &UpdateMoneys_TickSystemO);

	if (UpdateMoneys_AddMoneyP == 0)
		MessageBoxA(nullptr, "Failed To Find UpdateMoney::AddMoney", "SASAVN HACK", 0);

	if (!TriggerItemsHook.hook())
		MessageBoxA(nullptr, "Failed To Hook ItemDropCheck::TriggerItems", "SASAVN HACK", 0);

	if (!TickSystemHook.hook())
		MessageBoxA(nullptr, "Failed To Hook UpdateMoneys::TickSystem", "SASAVN HACK", 0);

	MessageBoxA(nullptr, "Press to banana once to start ItemDropChecks", "SASAVN HACK", 0);

	auto lastDropTime = std::chrono::high_resolution_clock::now() + 50ms;

	while (!stopToken.stop_requested())
	{
		if (BananaClickedAddMoney)
		{
			addMoneyThis->fields.money++;

			auto randomValue = rand() * 100000 + rand();
			if (randomValue < 0)
				randomValue *= -1;

			addMoneyThis->fields.timesClickedPerSecond = randomValue;
		}

		if (lastDropTime < std::chrono::high_resolution_clock::now())
		{
			if (BananaClickedDrop)
				ItemDropCheck_TriggerItems(itemDropThis, itemDropMethod);

			if (BananaClickedAddMoney)
				PLH::FnCast(UpdateMoneys_AddMoneyP, UpdateMoneys_TickSystem)(addMoneyThis, addMoneyMethod);

			lastDropTime = std::chrono::high_resolution_clock::now() + 50ms;
		}
	}

	TickSystemHook.unHook();
	TriggerItemsHook.unHook();
}

BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		std::jthread thrd{bananaThread};
		thrd.detach();
	}

	return TRUE;
}
