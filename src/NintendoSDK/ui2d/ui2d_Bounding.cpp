#include <nn/ui2d/Bounding.h>

namespace nn::ui2d {

Bounding::Bounding(const ResBounding* resource, const ResBounding*, const BuildArgSet& args)
    : Pane(resource, args) {}

Bounding::Bounding(const Bounding& other) : Pane(other) {}

Bounding::~Bounding() = default;

void Bounding::DrawSelf(DrawInfo&, gfx::CommandBuffer&) {}

}  // namespace nn::ui2d
