#include "Settings.h"

Preset _parsePreset(const YAML::Node& config) { 
    Preset preset;
    if (!config["remnants"] || config["remnants"].size() == 0) {
        logger::error("Remnants are empty.");
        return preset;
    }
    for (const auto& remnantNode : config["remnants"]) {
        const auto temp_formeditorid = remnantNode["FormEditorID"] && !remnantNode["FormEditorID"].IsNull()
                                           ? remnantNode["FormEditorID"].as<std::string>()
                                           : "";
        
        const FormID temp_formid =
            temp_formeditorid.empty() ? 0 : Utilities::FunctionsSkyrim::GetFormEditorIDFromString(temp_formeditorid);
        if (!temp_formid && !temp_formeditorid.empty()) {
            logger::error("Formid could not be obtained for {}", temp_formid, temp_formeditorid);
            return Preset();
        }
        if (!remnantNode["count"] || remnantNode["count"].IsNull()) {
            logger::error("Count is null.");
            return Preset();
        }
        if (!remnantNode["bench"] || remnantNode["bench"].IsNull()) {
            logger::error("Bench is null.");
            return Preset();
        }
        
        logger::trace("Count");
        const Count temp_count = remnantNode["count"].as<Count>();
        if (temp_count == 0) {
			logger::error("Count is 0.");
			return Preset();
		}
        
        logger::trace("Bench");
        const std::uint8_t temp_bench = remnantNode["bench"].as<std::uint8_t>();
        if (temp_bench == 0) {
        	logger::error("Bench is 0.");
        	return Preset();
        }

        preset.items[temp_formid] = std::make_pair(temp_count, temp_bench);
    }
    return preset;
}

CustomPresets parsePresets() {
    CustomPresets _custom_settings;
    const auto folder_path = std::format("Data/SKSE/Plugins/{}", Utilities::mod_name) + "/presets";
    std::filesystem::create_directories(folder_path);
    logger::trace("Custom path: {}", folder_path);

    for (const auto& entry : std::filesystem::directory_iterator(folder_path)) {
        if (entry.is_regular_file() && entry.path().extension() == ".yml") {
            const auto filename = entry.path().string();
            YAML::Node config = YAML::LoadFile(filename);

            if (!config["ownerLists"]) {
                logger::trace("OwnerLists not found in {}", filename);
                continue;
            }

            for (const auto& _Node : config["ownerLists"]) {
                // we have list of owners at each node or a scalar owner
                if (_Node["owners"].IsScalar()) {
                    const auto ownerName = _Node["owners"].as<std::string>();
                    auto temp_settings = _parsePreset(_Node);
                    if (temp_settings.CheckIntegrity()) {
                        _custom_settings[std::vector<std::string>{ownerName}] = temp_settings;
                    }
                } 
                else {
                    std::vector<std::string> owners;
                    for (const auto& owner : _Node["owners"]) {
                        owners.push_back(owner.as<std::string>());
                    }
                    auto temp_settings = _parsePreset(_Node);
                    if (temp_settings.CheckIntegrity()) _custom_settings[owners] = temp_settings;
                }
            }
        }
    }
    return _custom_settings;
}

const bool Preset::CheckIntegrity() { 
    if (items.empty()) {
		logger::error("Items are empty.");
        return InitFailed();
	}
    for (const auto& [formid, count_bench] : items) {
        if (count_bench.first == 0) {
			logger::error("Count is 0.");
            return InitFailed();
		}
        if (count_bench.second == 0) {
			logger::error("Bench is 0.");
            return InitFailed();
		}

        if (!Utilities::FunctionsSkyrim::GetFormByID(formid)) {
			logger::error("Formid is null for {}", formid);
            return InitFailed();
		}
	}
	return true;
}
