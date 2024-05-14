
#pragma once
#include "Manager.h"

class ourEventSink : public RE::BSTEventSink<RE::TESContainerChangedEvent>,
                     public RE::BSTEventSink<RE::TESFurnitureEvent> ,
                     public RE::BSTEventSink<RE::BGSActorEvent> {

	ourEventSink() = default;
    ourEventSink(const ourEventSink&) = delete;
    ourEventSink(ourEventSink&&) = delete;
    ourEventSink& operator=(const ourEventSink&) = delete;
    ourEventSink& operator=(ourEventSink&&) = delete;

    
    RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent* event,
                                          RE::BSTEventSource<RE::TESContainerChangedEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::TESFurnitureEvent* event,
                                          RE::BSTEventSource<RE::TESFurnitureEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::BGSActorEvent* event, RE::BSTEventSource<RE::BGSActorEvent>*);

    void BenchEnter(const std::uint8_t bench_no);

    void BenchExit();

    std::uint8_t bench = 0;
    std::map<FormID, Count> counts;


public:
    static ourEventSink* GetSingleton() {
        static ourEventSink singleton;
        return &singleton;
    }

    Manager* M = Manager::GetSingleton();

};


class BSAnimationGraphEventHandler final : public RE::BSTEventSink<RE::BSAnimationGraphEvent> {
public:
    using EventResult = RE::BSEventNotifyControl;

    static BSAnimationGraphEventHandler* GetSingleton();
    EventResult ProcessEvent(const RE::BSAnimationGraphEvent* a_event,
                             RE::BSTEventSource<RE::BSAnimationGraphEvent>* a_eventSource) override;

    BSAnimationGraphEventHandler(const BSAnimationGraphEventHandler&) = delete;
    BSAnimationGraphEventHandler(BSAnimationGraphEventHandler&&) = delete;

    BSAnimationGraphEventHandler& operator=(const BSAnimationGraphEventHandler&) = delete;
    BSAnimationGraphEventHandler& operator=(BSAnimationGraphEventHandler&&) = delete;

protected:
    BSAnimationGraphEventHandler() = default;
    virtual ~BSAnimationGraphEventHandler() = default;
};
				

bool AnimationGraphEventHandler(RE::BSTEventSink<RE::BSAnimationGraphEvent>* a_sink);

class AnimGraphSinkDelegate : public SKSE::detail::TaskDelegate{
public:
    void Run() override;
    void Dispose() override;
};