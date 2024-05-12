#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <spdlog/sinks/basic_file_sink.h>

namespace logger = SKSE::log;
using namespace std::literals;

using FormID = RE::FormID;
using RefID = RE::FormID;
using Count = float;

using Remnants = std::map<FormID, std::pair<Count, std::uint8_t>>;

const RefID player_refid = 20;