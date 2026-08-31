#include "PCH.h"
#include "Settings.h"
#include "InputHandler.h"
#include "WardManager.h"

#include <fstream>
#include <thread>
#include <chrono>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>

namespace {
    void InitializeLogging() {
        auto path = SKSE::log::log_directory();
        if (!path) {
            SKSE::stl::report_and_fail("Unable to lookup SKSE logs directory.");
        }
        *path /= fmt::format("{}.log", SKSE::PluginDeclaration::GetSingleton()->GetName());

        std::shared_ptr<spdlog::logger> log;
        if (IsDebuggerPresent()) {
            log = std::make_shared<spdlog::logger>("Global", std::make_shared<spdlog::sinks::msvc_sink_mt>());
        } else {
            log = std::make_shared<spdlog::logger>(
                "Global", std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true));
        }
        log->set_level(spdlog::level::info);
        log->flush_on(spdlog::level::info);

        spdlog::set_default_logger(std::move(log));
        spdlog::set_pattern("[%H:%M:%S] [%l] %v");
    }

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
    // TEMPORARY DEBUG — hapus semua ini setelah masalah ketemu
    try {
        std::ofstream f("wardanytime_debug.txt");  // relative ke CWD proses game
        f << "SKSEPluginLoad called - relative path\n";
    } catch (const std::exception& e) {
        OutputDebugStringA(("wardanytime relative write failed: "s + e.what()).c_str());
    }
    try {
        std::ofstream f("C:\\wardanytime_debug.txt");
        f << "SKSEPluginLoad called - C root\n";
    } catch (const std::exception& e) {
        OutputDebugStringA(("wardanytime C:\\ write failed: "s + e.what()).c_str());
    }
    OutputDebugStringA("WardAnytime: SKSEPluginLoad entered\n");

    SKSE::Init(a_skse);
    InitializeLogging();
    logger::info("WardAnytime loaded.");

    SKSE::GetMessagingInterface()->RegisterListener(MessageHandler);
    return true;
}