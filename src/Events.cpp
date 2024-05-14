#include "Events.h"

RE::BSEventNotifyControl ourEventSink::ProcessEvent(const RE::TESContainerChangedEvent* event,
                                                    RE::BSTEventSource<RE::TESContainerChangedEvent>*) {
    
    if (!event) return RE::BSEventNotifyControl::kContinue;
    if (bench == 0) return RE::BSEventNotifyControl::kContinue;
    if (event->oldContainer!=player_refid) return RE::BSEventNotifyControl::kContinue;

    const auto remnants = M->GetRemnants(event->baseObj);
    if (remnants.empty()) return RE::BSEventNotifyControl::kContinue;

    for (const auto& [remnant, count_bench] : remnants) {
        const auto bench_no = count_bench.second;
        if (bench_no != bench) continue;
        const auto count = count_bench.first * event->itemCount;
        if (counts.contains(remnant)) counts[remnant] += count;
        else counts[remnant] = count;
	}

    logger::trace("Adding Items to player");

    const auto player = RE::PlayerCharacter::GetSingleton();
    for (const auto& [remnant, count] : counts) {
		logger::trace("remnant: {}, count: {}", remnant, count);
        const auto count_to_add = static_cast<std::uint32_t>(std::floor(count));
        if (count_to_add == 0) continue;
        const auto remnant_item = Utilities::FunctionsSkyrim::GetFormByID<RE::TESBoundObject>(remnant);
        if (!remnant_item) continue;
        player->AddObjectToContainer(remnant_item, nullptr, count_to_add, nullptr);
        counts[remnant] -= count_to_add;
	}

    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ourEventSink::ProcessEvent(const RE::TESFurnitureEvent* event,
                                                    RE::BSTEventSource<RE::TESFurnitureEvent>*) {

    logger::trace("Furniture Event");
    
    if (!event) return RE::BSEventNotifyControl::kContinue;
    if (!event->actor) return RE::BSEventNotifyControl::kContinue;
    if (!event->actor->IsPlayerRef()) return RE::BSEventNotifyControl::kContinue;
    if (bench!=0 && event->type == RE::TESFurnitureEvent::FurnitureEventType::kEnter) {
        return RE::BSEventNotifyControl::kContinue;
    }
    if (bench == 0 && event->type != RE::TESFurnitureEvent::FurnitureEventType::kEnter) {
        return RE::BSEventNotifyControl::kContinue;
    }
    if (bench != 0 && event->type == RE::TESFurnitureEvent::FurnitureEventType::kExit) {
        BenchExit();
		return RE::BSEventNotifyControl::kContinue;
	}

    if (!event->targetFurniture) return RE::BSEventNotifyControl::kContinue;
    
    const auto furn_base = event->targetFurniture->GetBaseObject();
    if (!furn_base) return RE::BSEventNotifyControl::kContinue;
    if (furn_base->formType.underlying() != 40) return RE::BSEventNotifyControl::kContinue;

    const auto temp_bench = furn_base->As<RE::TESFurniture>();
    if (!temp_bench) return RE::BSEventNotifyControl::kContinue;
    auto bench_no = static_cast<std::uint8_t>(temp_bench->workBenchData.benchType.get());
    
    BenchEnter(bench_no);
    
    return RE::BSEventNotifyControl::kContinue;
}

RE::BSEventNotifyControl ourEventSink::ProcessEvent(const RE::BGSActorEvent* event,
                                                    RE::BSTEventSource<RE::BGSActorEvent>*) {
    return RE::BSEventNotifyControl::kContinue;
}

void ourEventSink::BenchEnter(const std::uint8_t bench_no) {
    if (bench_no == 0) return;
	if (!M->IsBenchSupported(bench_no)) return;
	bench = bench_no;
    counts.clear();
}

void ourEventSink::BenchExit() {
	bench = 0;
	counts.clear();
}


auto BSAnimationGraphEventHandler::GetSingleton() -> BSAnimationGraphEventHandler* {
    logger::trace("BSAnimationGraphEventHandler::GetSingleton");
    static BSAnimationGraphEventHandler singleton;
    return std::addressof(singleton);
}

auto BSAnimationGraphEventHandler::ProcessEvent(const RE::BSAnimationGraphEvent* a_event,
                                                RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource)
    -> EventResult {
    if (!a_event) return EventResult::kContinue;
    logger::info("BSAnimationGraphEventHandler::ProcessEvent");

    logger::info("BSAnimationGraphEventHandler::ProcessEvent: holder: {}", a_event->holder->GetName());
    logger::info("BSAnimationGraphEventHandler::ProcessEvent: tag: {}", a_event->tag);

    return EventResult::kContinue;
}

bool AnimationGraphEventHandler(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink) {
    const auto player = RE::PlayerCharacter::GetSingleton();
    RE::BSAnimationGraphManagerPtr graphManager;
    player->GetAnimationGraphManager(graphManager);
    if (graphManager) {
        auto sinked = false;
        for (auto& animationGraph : graphManager->graphs) {
            if (sinked) {
                break;
            }

            auto eventSource = animationGraph->GetEventSource<RE::BSAnimationGraphEvent>();
            for (auto& sink : eventSource->sinks) {
                if (sink == a_sink) {
                    sinked = true;
                    break;
                }
            }
        }

        if (!sinked) {
            graphManager->graphs.front()->GetEventSource<RE::BSAnimationGraphEvent>()->AddEventSink(a_sink);
            logger::info("AnimationGraphEventHandler: Sink added");
            return true;
        }
    }
    return false;
}


void AnimGraphSinkDelegate::Run() { AnimationGraphEventHandler(BSAnimationGraphEventHandler::GetSingleton()); }

void AnimGraphSinkDelegate::Dispose() { delete this; }
