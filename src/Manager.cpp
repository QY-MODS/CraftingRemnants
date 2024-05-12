#include "Manager.h"

const Remnants Manager::_MakeSource(const FormID item, const Remnants& remnants) { 
    if (sources.contains(item)){
        logger::error("Manager::_MakeSource: item already exists in sources");
    } else sources[item] = remnants;
    return sources.at(item);
}

const bool Manager::IsBenchSupported(const std::uint8_t bench) const { 
	return benches.contains(bench); 
}

const Remnants Manager::GetRemnants(const FormID used_item_formid) {
    Remnants remnants = {};
	const auto used_item = Utilities::FunctionsSkyrim::GetFormByID<RE::TESBoundObject>(used_item_formid);
    if (!used_item) return remnants;

    if (sources.contains(used_item_formid)) return sources.at(used_item_formid);

	for (auto& [names, sttng] : presets) {
        if (!sttng.IsHealthy()) continue;
        logger::trace("GetRemnants: custom settings owner names {}", Utilities::Functions::String::join(names, ", "));
        for (auto& name : names) {
            const FormID temp_cstm_formid = Utilities::FunctionsSkyrim::GetFormEditorIDFromString(name);
            if (temp_cstm_formid <= 0) continue;
            if (const auto temp_cstm_form = Utilities::FunctionsSkyrim::GetFormByID(temp_cstm_formid, name)) {
                if (temp_cstm_form->GetFormID() == used_item_formid) return _MakeSource(used_item_formid, sttng.items);
            }
        }
        if (Utilities::Functions::String::includesWord(used_item->GetName(), names)) {
            return _MakeSource(used_item_formid, sttng.items);
        }
    }
    return remnants;
}

void Manager::Init() {
	presets = parsePresets();
    for (const auto& [ownerlist,preset] : presets) {
		for (const auto& [formid, count_bench] : preset.items) {
            benches.insert(count_bench.second);
		}
	}
};

void Manager::UnInstall() {}
