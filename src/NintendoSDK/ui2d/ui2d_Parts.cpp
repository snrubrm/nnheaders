#include <nn/ui2d/Parts.h>

namespace nn::ui2d {

// 0x7100ab98e8
Parts::Parts(const ResParts* resource, const ResParts*, const BuildArgSet& args)
    : Pane(resource, args), mPartsLayoutLink{{}, nullptr} {}

// 0x7100ab9928
Parts::Parts(const Parts& other) : Pane(other), mPartsLayoutLink{{}, nullptr} {}

// 0x7100ab9964 / 0x7100ab9968
Parts::~Parts() {}

// 0x7100ab998c
Pane* Parts::FindPaneByNameRecursive(const char* name) {
    const char* own_name = GetName();
    for (s32 i = 0; i < 24; ++i) {
        if (own_name[i] != name[i])
            return nullptr;
        if (own_name[i] == '\0')
            return this;
    }
    return this;
}

// 0x7100ab99c0
const Pane* Parts::FindPaneByNameRecursive(const char* name) const {
    return const_cast<Parts*>(this)->FindPaneByNameRecursive(name);
}

// 0x7100ab9a60
const Material* Parts::FindMaterialByNameRecursive(const char* name) const {
    return const_cast<Parts*>(this)->FindMaterialByNameRecursive(name);
}

}  // namespace nn::ui2d
