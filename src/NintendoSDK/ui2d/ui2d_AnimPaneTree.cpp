#include <nn/ui2d/detail/AnimPaneTree.h>

#include <nn/ui2d/Layout.h>
#include <nn/ui2d/Material.h>
#include <nn/ui2d/Pane.h>

namespace nn::ui2d::detail {

void AnimPaneTree::Set(Pane* pane, const AnimResource& resource) {
    *this = {};
    findPaneAndMaterialAnimContents(pane, resource);
}

void AnimPaneTree::findPaneAndMaterialAnimContents(Pane* pane, const AnimResource& resource) {
    const ResAnimationBlock* block = resource.GetAnimationBlock();
    const ResAnimationContent* pane_content = nullptr;
    for (u64 i = 0; i < block->contentCount; ++i) {
        const ResAnimationContent* content = block->GetContentAtOffset(block->GetContentOffsets()[i]);
        if (content->type == 0) {
            bool equal = true;
            for (int j = 0; j < 28; ++j) {
                if (content->name[j] != pane->GetName()[j]) {
                    equal = false;
                    break;
                }
                if (content->name[j] == '\0')
                    break;
            }
            if (equal) {
                pane_content = content;
                break;
            }
        }
    }

    u16 count = pane_content != nullptr;
    const u8 material_count = pane->GetMaterialCount();
    const ResAnimationContent* material_contents[9];
    for (u64 i = 0; i < material_count; ++i) {
        const Material* material = pane->GetMaterial(i);
        const ResAnimationContent* found = nullptr;
        if (block->contentCount) {
            const char* material_name = material->mName;
            for (u64 j = 0; j < block->contentCount; ++j) {
                const ResAnimationContent* content = block->GetContentAtOffset(block->GetContentOffsets()[j]);
                if (content->type == 1) {
                    bool equal = true;
                    for (int k = 0; k < 28; ++k) {
                        if (content->name[k] != material_name[k]) {
                            equal = false;
                            break;
                        }
                        if (content->name[k] == '\0')
                            break;
                    }
                    if (equal) {
                        found = content;
                        break;
                    }
                }
            }
        }
        material_contents[i] = found;
        if (found)
            ++count;
    }

    if (count) {
        mResource = resource;
        mPaneContent = pane_content;
        mMaterialCount = material_count;
        for (u8 i = 0; i < material_count; ++i)
            mMaterialContents[i] = material_contents[i];
        mBindCount = count;
    }
}

AnimTransformBasic* AnimPaneTree::Bind(gfx::Device* device, Layout* layout, Pane* pane,
                                       ResourceAccessor* accessor) const {
    auto* transform = static_cast<AnimTransformBasic*>(layout->CreateAnimTransformBasic());
    transform->SetResource(device, accessor, mResource.GetAnimationBlock(), mBindCount);
    if (mPaneContent && transform->mBindCount < transform->_3a)
        transform->_30[transform->mBindCount++] = {pane, mPaneContent};

    const u8 material_count = pane->GetMaterialCount();
    const u8 count = material_count < mMaterialCount ? material_count : mMaterialCount;
    for (u8 i = 0; i < count; ++i) {
        if (mMaterialContents[i])
            transform->BindMaterialImpl(pane->GetMaterial(i), mMaterialContents[i]);
    }
    return transform;
}

}  // namespace nn::ui2d::detail
