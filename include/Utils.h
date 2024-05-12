#pragma once

// #include <chrono>
#include <windows.h>
#include <yaml-cpp/yaml.h>
#include <ClibUtil/editorID.hpp>
#include <algorithm>
#include <codecvt>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "SimpleIni.h"

namespace Utilities {

    const auto mod_name = static_cast<std::string>(SKSE::PluginDeclaration::GetSingleton()->GetName());

    constexpr auto po3path = "Data/SKSE/Plugins/po3_Tweaks.dll";
    bool IsPo3Installed();
    const auto po3_err_msgbox = std::format(
        "{}: If you are trying to use Editor IDs, but you must have powerofthree's Tweaks "
        "installed. See mod page for further instructions.",
        mod_name);

    namespace MsgBoxesNotifs {
        namespace Windows {

            int Po3ErrMsg();
        }
    };


    namespace Functions {

		[[nodiscard]] bool isValidHexWithLength7or8(const char* input);

        namespace String {

            template <typename T>
            std::string join(const T& container, const std::string_view& delimiter) {
                std::ostringstream oss;
                auto iter = container.begin();

                if (iter != container.end()) {
                    oss << *iter;
                    ++iter;
                }

                for (; iter != container.end(); ++iter) {
                    oss << delimiter << *iter;
                }

                return oss.str();
            }

            std::string trim(const std::string& str);

            std::string toLowercase(const std::string& str);

            std::string replaceLineBreaksWithSpace(const std::string& input);

            bool includesString(const std::string& input, const std::vector<std::string>& strings);

            bool includesWord(const std::string& input, const std::vector<std::string>& strings);
        };

	}

    namespace FunctionsSkyrim {

        RE::TESForm* GetFormByID(const RE::FormID id, const std::string& editor_id = "");

        template <class T = RE::TESForm>
        static T* GetFormByID(const RE::FormID id, const std::string& editor_id = "") {
            if (!editor_id.empty()) {
                auto* form = RE::TESForm::LookupByEditorID<T>(editor_id);
                if (form) return form;
            }
            T* form = RE::TESForm::LookupByID<T>(id);
            if (form) return form;
            return nullptr;
        };

        [[nodiscard]] FormID GetFormEditorIDFromString(const std::string& formEditorId);
    }
};