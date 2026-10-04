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
    PartsLayoutLink mPartsLayoutLink;
};
static_assert(sizeof(Parts) == 0xf8);
}  // namespace ui2d
}  // namespace nn
