#pragma once

#include <nn/ui2d/AnimTransform.h>

namespace nn::ui2d {
class Layout;
}

namespace nn::ui2d::detail {

class AnimPaneTree {
public:
    void Set(Pane* pane, const AnimResource& resource);
    AnimTransformBasic* Bind(gfx::Device* device, Layout* layout, Pane* pane,
                             ResourceAccessor* accessor) const;

private:
    void findPaneAndMaterialAnimContents(Pane* pane, const AnimResource& resource);

    AnimResource mResource;
    const ResAnimationContent* mPaneContent;
    const ResAnimationContent* mMaterialContents[9];
    u16 mBindCount;
    u8 mMaterialCount;
};
static_assert(sizeof(AnimPaneTree) == 0x78);

}  // namespace nn::ui2d::detail
