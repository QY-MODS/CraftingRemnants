#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <spdlog/sinks/basic_file_sink.h>

namespace logger = SKSE::log;
using namespace std::literals;

using FormID = RE::FormID;
using RefID = RE::FormID;
using Count = float;

using Remnants = std::map<std::pair<FormID, std::uint8_t>, Count>;

const RefID player_refid = 20;