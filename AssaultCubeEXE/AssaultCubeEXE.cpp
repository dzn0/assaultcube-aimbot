#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <iomanip>
#include <cmath>

// --- ESTRUTURAS ---
struct player {
    char pad_00[4];
    float x;             // 0x4 
    float z;             // 0x8 
    float y;             // 0xC  
    char pad_01[0x24];
    float yaw;           // 0x34
    float pitch;         // 0x38
    char pad_02[0xB0];
    int health;          // 0xEC
    char pad_03[0x50];
    int ammo;            // 0x140
};

// --- FUNÇÕES DE AUXÍLIO ---
uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName) {
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry)) {
            do {
                if (!_stricmp(modEntry.szModule, modName)) {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

// --- LOGICA PRINCIPAL ---
int main() {
    std::cout << "by: @dzn | discord: d.zn." << std::endl;
    std::cout << "---------------------------" << std::endl;
    std::cout << "Abra o script enquanto vivo!" << std::endl;
    std::cout << "Cuidado com seus aliados!" << std::endl;

    HWND hWindow = FindWindowA(NULL, "AssaultCube");
    if (!hWindow) { std::cout << "Jogo nao encontrado!" << std::endl; Sleep(2000); return 0; }

    DWORD pID; GetWindowThreadProcessId(hWindow, &pID);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    uintptr_t baseAddr = GetModuleBaseAddress(pID, "ac_client.exe");

    uintptr_t playerPtr = baseAddr + 0x17E0A8;
    uintptr_t entityPtr = baseAddr + 0x18AC04;
    uintptr_t playersPtr = baseAddr + 0x18AC0C;

    DWORD playerAddr; ReadProcessMemory(hProcess, (LPCVOID)playerPtr, &playerAddr, sizeof(playerAddr), NULL);
    DWORD entityAddr; ReadProcessMemory(hProcess, (LPCVOID)entityPtr, &entityAddr, 4, NULL);

    while (true) {
        player localPlayer;
        ReadProcessMemory(hProcess, (LPCVOID)playerAddr, &localPlayer, sizeof(localPlayer), NULL);

        int numPlayers = 0;
        ReadProcessMemory(hProcess, (LPCVOID)playersPtr, &numPlayers, sizeof(numPlayers), NULL);

        float menorFov = 999999.0f;
        int idAlvoMaisProximo = -1;
        float targetYaw = 0, targetPitch = 0;

        for (int i = 0; i < numPlayers; i++) {
            DWORD enemyPtr = 0;
            ReadProcessMemory(hProcess, (LPCVOID)(entityAddr + (i * 4)), &enemyPtr, sizeof(enemyPtr), NULL);

            if (!enemyPtr || enemyPtr == playerAddr) continue;

            player enemy;
            ReadProcessMemory(hProcess, (LPCVOID)enemyPtr, &enemy, sizeof(player), NULL);

            if (enemy.health > 0 && enemy.health <= 100) {
                float relX = enemy.x - localPlayer.x;
                float relZ = enemy.z - localPlayer.z;
                float relY = enemy.y - localPlayer.y;
                float dist2D = sqrtf(relX * relX + relZ * relZ);

                float tmpYaw = atan2f(-relX, relZ) * (180.0f / 3.1415926535f) + 180.0f;
                float tmpPitch = atan2f(relY, dist2D) * (180.0f / 3.1415926535f);

                float diffYaw = tmpYaw - localPlayer.yaw;
                float diffPitch = tmpPitch - localPlayer.pitch;

                if (diffYaw > 180) diffYaw -= 360;
                if (diffYaw < -180) diffYaw += 360;

                float fovDist = sqrtf(diffYaw * diffYaw + diffPitch * diffPitch);

                if (fovDist < menorFov) {
                    menorFov = fovDist;
                    idAlvoMaisProximo = i;
                    targetYaw = tmpYaw;
                    targetPitch = tmpPitch;
                }
            }
        }

        if (idAlvoMaisProximo != -1 && GetAsyncKeyState(VK_RBUTTON)) {
            if (menorFov < 40.0f) {
                float smoothValue = 1.0f; // Aimbot Speed

                float deltaYaw = targetYaw - localPlayer.yaw;
                float deltaPitch = targetPitch - localPlayer.pitch;

                if (deltaYaw > 180) deltaYaw -= 360;
                if (deltaYaw < -180) deltaYaw += 360;

                float smoothYaw = localPlayer.yaw + (deltaYaw / smoothValue);
                float smoothPitch = localPlayer.pitch + (deltaPitch / smoothValue);

                WriteProcessMemory(hProcess, (LPVOID)(playerAddr + 0x34), &smoothYaw, sizeof(float), NULL);
                WriteProcessMemory(hProcess, (LPVOID)(playerAddr + 0x38), &smoothPitch, sizeof(float), NULL);
            }
        }

        std::cout << "\rTarget: " << idAlvoMaisProximo << " | FOV: " << std::fixed << std::setprecision(1) << menorFov << "    " << std::flush;

        if (GetAsyncKeyState(VK_END)) break;
        Sleep(1);
    }
    CloseHandle(hProcess);
    return 0;
}