
#pragma once
#include "Manager.h"

class ourEventSink : public RE::BSTEventSink<RE::TESContainerChangedEvent>,
                     public RE::BSTEventSink<RE::TESFurnitureEvent> {

	ourEventSink() = default;
    ourEventSink(const ourEventSink&) = delete;
    ourEventSink(ourEventSink&&) = delete;
    ourEventSink& operator=(const ourEventSink&) = delete;
    ourEventSink& operator=(ourEventSink&&) = delete;

    
    RE::BSEventNotifyControl ProcessEvent(const RE::TESContainerChangedEvent* event,
                                          RE::BSTEventSource<RE::TESContainerChangedEvent>*);

    RE::BSEventNotifyControl ProcessEvent(const RE::TESFurnitureEvent* event,
                                          RE::BSTEventSource<RE::TESFurnitureEvent>*);

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
						