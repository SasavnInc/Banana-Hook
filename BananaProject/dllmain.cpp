#include "pch.h"
#include <thread>
#include <polyhook2/Detour/x64Detour.hpp>
#include "GameClasses.hpp"

class MethodInfo;
class ItemDropCheck_o;

bool BananaClickedDrop = false;
ItemDropCheck_o* itemDropThis = 0;
MethodInfo* itemDropMethod = 0;

bool BananaClickedAddMoney = false;
UpdateMoney_o* addMoneyThis = 0;
MethodInfo* addMoneyMethod = 0;

uint64_t ItemDropCheck__TriggerItemsO = 0;
uint64_t UpdateMoney__AddMoneyO = 0;

void __stdcall UpdateMoney__AddMoney(UpdateMoney_o* this_, MethodInfo* method)
{
    addMoneyThis = this_;
    addMoneyMethod = method;
    BananaClickedAddMoney = true;

    return PLH::FnCast(UpdateMoney__AddMoneyO, UpdateMoney__AddMoney)(this_, method);
}

void __stdcall ItemDropCheck__TriggerItems(ItemDropCheck_o* this_, MethodInfo* method)
{
    itemDropThis = this_;
    itemDropMethod = method;
    BananaClickedDrop = true;

    return PLH::FnCast(ItemDropCheck__TriggerItemsO, ItemDropCheck__TriggerItems)(this_, method);
}

void bananaThread()
{
    auto base = (uint64_t)GetModuleHandleA("GameAssembly.dll");

    auto dosHeader = (IMAGE_DOS_HEADER*)(base);
    auto ntHeader = (IMAGE_NT_HEADERS64*)(base + dosHeader->e_lfanew);
    auto moduleSize = ntHeader->OptionalHeader.SizeOfImage;

    auto ItemDropCheck__TriggerItemsP = PLH::findPattern(base, moduleSize, "40 53 48 83 EC 20 48 8B D9 33 D2 B9");
    //UpdateMoney__cz now
	auto UpdateMoney__AddMoneyP = PLH::findPattern(base, moduleSize, "40 53 48 83 EC 60 80 3D ?? ?? ?? ?? ?? 48 8B D9 0F 29 74 24 ?? 75 1F 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05");
    //UpdateMoney__cy
	auto UpdateMoney__UpdateCounterP = PLH::findPattern(base, moduleSize, "48 89 5C 24 ?? 57 48 83 EC 20 80 3D ?? ?? ?? ?? ?? 48 8B F9 75 13 48 8D 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? C6 05 ?? ?? ?? ?? ?? 48 8B 47 30");

    PLH::x64Detour TriggerItemsHook(ItemDropCheck__TriggerItemsP, (uint64_t)ItemDropCheck__TriggerItems, &ItemDropCheck__TriggerItemsO);
    PLH::x64Detour AddMoneyHook(UpdateMoney__AddMoneyP, (uint64_t)UpdateMoney__AddMoney, &UpdateMoney__AddMoneyO);

    if (UpdateMoney__UpdateCounterP == 0)
        MessageBoxA(0, "Failed To Find UpdateMoney::UpdateCounterP", "SASAVN HACK", 0);

    if (!TriggerItemsHook.hook())
        MessageBoxA(0, "Failed To Hook ItemDropCheck::TriggerItems", "SASAVN HACK", 0);

    if (!AddMoneyHook.hook())
        MessageBoxA(0, "Failed To Hook UpdateMoney::AddMoney", "SASAVN HACK", 0);

    MessageBoxA(0, "Press to banana once to start ItemDropChecks", "SASAVN HACK", 0);

    auto lastDropTime = std::chrono::high_resolution_clock::now() + std::chrono::seconds(1);

    while (true)
    {
        if (BananaClickedAddMoney) {
            addMoneyThis->fields.moneyAmount++; //not really needed, but let user see new values
            addMoneyThis->fields.CPS = rand() * 100000 + rand(); //let user see fun value
            if (addMoneyThis->fields.CPS < 0)
                addMoneyThis->fields.CPS *= -1;
        }

        if (lastDropTime < std::chrono::high_resolution_clock::now()) {
            if (BananaClickedDrop)
				ItemDropCheck__TriggerItems(itemDropThis, itemDropMethod);

            if (BananaClickedAddMoney)
                PLH::FnCast(UpdateMoney__UpdateCounterP, UpdateMoney__AddMoney)(addMoneyThis, addMoneyMethod);

            lastDropTime = std::chrono::high_resolution_clock::now() + std::chrono::seconds(1);
        }

    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        std::thread thrd{bananaThread};
        thrd.detach();
    }

    return TRUE;
}

