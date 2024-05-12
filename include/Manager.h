#include "Settings.h"

class Manager {

    CustomPresets presets;
    std::map<FormID, Remnants> sources;
    std::set<std::uint8_t> benches;

    const Remnants _MakeSource(const FormID item, const Remnants& remnants);

public:
    Manager() { Init(); };

    static Manager* GetSingleton() {
        static Manager singleton;
        return &singleton;
    }

    const bool IsBenchSupported(const std::uint8_t bench) const;

    const Remnants GetRemnants(const FormID item);

    void Init();

    void UnInstall();

};