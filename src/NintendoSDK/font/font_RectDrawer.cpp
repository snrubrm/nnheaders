#include <nn/font/font_RectDrawer.h>

namespace nn::font {

RectDrawer::RectDrawer() : mState(0) {}

RectDrawer::~RectDrawer() = default;

void RectDrawer::RegisterSamplerSlot(SamplerRegistrationCallback callback, void* user_data) {
    callback(&mSamplerSlot, mSampler, user_data);
}

void RectDrawer::UnregisterSamplerSlot(SamplerDescriptorCallback callback, void* user_data) {
    callback(&mSamplerSlot, mSampler, user_data);
    mSamplerSlot.Invalidate();
}

}  // namespace nn::font
