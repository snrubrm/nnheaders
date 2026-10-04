/**
 * @file Parts.h
 * @brief Layout parts.
 */

#pragma once

#include <nn/ui2d/Pane.h>
#include <nn/ui2d/Layout.h>

namespace nn {
namespace ui2d {
struct BuildArgSet;
struct ResParts;

class Parts : public nn::ui2d::Pane {
public:
    NN_RUNTIME_TYPEINFO(nn::ui2d::Pane);

    Parts();
    Parts(nn::ui2d::ResParts const*, nn::ui2d::ResParts const*, nn::ui2d::BuildArgSet const&);
    Parts(nn::ui2d::Parts const&);

    virtual ~Parts();

    // Both constructors initialize this link; BuildPartsImpl links it into
    // the parent's parts-layout list and stores the created Layout at +0xf0.
    // Known fields end at +0xf8. BuildPaneObj allocates 0x100 bytes; the
    // original alignment or remaining extent is not established yet.
    PartsLayoutLink mPartsLayoutLink;

protected:
    Pane* FindPaneByNameRecursive(const char*) override;
    const Pane* FindPaneByNameRecursive(const char*) const override;
    Material* FindMaterialByNameRecursive(const char*) override;
    const Material* FindMaterialByNameRecursive(const char*) const override;
};
}  // namespace ui2d
}  // namespace nn
