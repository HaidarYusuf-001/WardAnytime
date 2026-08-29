#include "PCH.h"
#include "Settings.h"
#include "InputHandler.h"
#include "WardManager.h"

#include <thread>
#include <chrono>

namespace {
    void OnDataLoaded() {
        Settings::Load();
        auto* inputMgr = RE::BSInputDeviceManager::GetSingleton();
        inputMgr->AddEventSink(InputHandler::GetSingleton());
    }

    void MessageHandler(SKSE::MessagingInterface::Message* a_msg) {
        if (a_msg->type == SKSE::MessagingInterface::kDataLoaded) {
            OnDataLoaded();

            std::thread([]() {
                using namespace std::chrono_literals;
                while (true) {
                    std::this_thread::sleep_for(15ms);
                    SKSE::GetTaskInterface()->AddTask([]() {
                        WardManager::GetSingleton()->Update(0.015f);
                    });
                }
            }).detach();
        }
    }
}

SKSEPluginLoad(const SKSE::LoadInterface* a_skse) {
    SKSE::Init(a_skse);
    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);
    return true;
}