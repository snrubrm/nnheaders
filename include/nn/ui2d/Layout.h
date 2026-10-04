/**
 * @file Layout.h
 * @brief UI Layout implementation.
 */

#pragma once

#include <nn/types.h>

namespace nn {
namespace font {
template <typename CharType>
class TagProcessorBase;
}  // namespace font

namespace ui2d {
class AnimTransform;
class Pane;

class Layout {
public:
    Layout();

    virtual ~Layout();

    virtual void DeleteAnimTransform(nn::ui2d::AnimTransform*);
    virtual void BindAnimation(nn::ui2d::AnimTransform*);
    virtual void UnbindAnimation(nn::ui2d::AnimTransform*);
    virtual void UnbindAnimation(nn::ui2d::Pane*);
    virtual void UnbindAllAnimation();

    virtual void Animate();
    virtual void UpdateAnimFrame(f32 frame);
    virtual void AnimateAndUpdateAnimFrame(f32 frame);

    typedef void* (*AllocateFunction)(size_t size, size_t alignment, void* user_data);
    typedef void (*FreeFunction)(void* ptr, void* user_data);

    static void SetAllocator(AllocateFunction, FreeFunction, void* user_data);
    // The callers pass the alignment (the CSV's one-argument name for 0x7100ab65f4 is an IDA guess).
    static void* AllocateMemory(size_t size, size_t alignment);
    static void FreeMemory(void* src);

    Pane* GetPane() const { return mPane; }

    // Sets the tag processor of every text box of the layout's pane tree.
    void SetTagProcessor(font::TagProcessorBase<u16>* tag_processor);

    // The allocator set by SetAllocator (public: eui::GetNwAllocatorHeap returns the user data, which is the
    // game's heap).
    static AllocateFunction g_pAllocateFunction;
    static FreeFunction g_pFreeFunction;
    static void* g_pUserData;

private:
    u64 _8;
    u64 _10;
    Pane* mPane;
    u64 _20;
    f32 _28;
    f32 _2c;
    u64 _30;

    u64 _40;
    u64 _48;
    u64 _50;
    u64 _58;
    u64 _60;
};
}  // namespace ui2d
}  // namespace nn
