/**
 * @file Pane.h
 * @brief Base UI panel.
 */

#pragma once

#include <nn/font/font_Util.h>
#include <nn/gfx/gfx_Device.h>
#include <nn/types.h>
#include <nn/ui2d/Types.h>
#include <nn/ui2d/ResPane.h>
#include <nn/util.h>
#include <nn/util/MathTypes.h>
#include <nn/util/util_IntrusiveList.h>


namespace nn::ui2d::detail {

class PaneBase {
    NN_NO_COPY(PaneBase);

public:
    PaneBase() = default;
    virtual ~PaneBase() = default;

    util::IntrusiveListNode m_Link;
};

}  // namespace nn::ui2d::detail

namespace nn::ui2d {
class AnimTransform;
class Layout;
class DrawInfo;
class ResPane;
struct BuildArgSet;
class Material;
struct ResExtUserDataList;
struct ResExtUserData;

class Pane : public detail::PaneBase {
public:
    NN_RUNTIME_TYPEINFO_BASE();

    // Set's original body identifies the matrix, scale, alpha and layout fields.
    // The drawing-object pointer's type and the final flag remain unknown.
    struct CalculateContext {
        void Set(const DrawInfo&, const Layout*);

        void* _0 = nullptr;
        const util::Matrix4x3fType* mViewMtx = nullptr;
        util::Float2 mLocationAdjustScale{};
        f32 mAlpha = 0.0f;
        bool _1c = false;
        bool _1d = false;
        bool _1e = false;
        bool _1f = false;
        const Layout* mLayout = nullptr;
        bool _28 = false;
    };
    static_assert(sizeof(CalculateContext) == 0x30);

    typedef util::IntrusiveList<Pane, util::IntrusiveListMemberNodeTraits<
                                          detail::PaneBase, &detail::PaneBase::m_Link, Pane>>
        PaneList;

    Pane();
    Pane(const ResPane*, const BuildArgSet&);
    Pane(const Pane&);

    ~Pane() override;
    virtual void Finalize(gfx::Device*);
    // Color element 16 is the alpha (Pane::GetColorElement reads / writes mAlpha for it); the others are vertex
    // color elements. A plain pane has no vertex colors (GetVertexColor returns -1, the element getter 0xff).
    virtual util::Unorm8x4 GetVertexColor(s32) const;
    virtual void SetVertexColor(s32, util::Unorm8x4 const&);
    virtual u8 GetColorElement(s32) const;
    virtual void SetColorElement(s32, u8);
    virtual u8 GetVertexColorElement(s32) const;
    virtual void SetVertexColorElement(s32, u8);
    // The result is a byte (callers zero-extend it; Picture / TextBox return 0 or 1).
    virtual u8 GetMaterialCount() const;
    virtual Material* GetMaterial(s32) const;
    virtual Pane* FindPaneByName(char const*, bool);
    virtual const Pane* FindPaneByName(char const*, bool) const;
    virtual Material* FindMaterialByName(char const*, bool);
    virtual const Material* FindMaterialByName(char const*, bool) const;
    virtual void BindAnimation(AnimTransform*, bool, bool);
    virtual void UnbindAnimation(AnimTransform*, bool);
    virtual void UnbindAnimationSelf(AnimTransform*);
    // Slot 19 (0x7100ab98d8): forwards its arguments to Calculate() (the bool is masked to one bit).
    virtual void m19(DrawInfo&, CalculateContext&, bool);
    virtual void Calculate(DrawInfo&, CalculateContext&, bool);
    virtual void Draw(DrawInfo&, gfx::CommandBuffer&);
    virtual void DrawSelf(DrawInfo&, gfx::CommandBuffer&);

    void SetName(const char*);
    void SetUserData(const char*);
    Material* GetMaterial() const;
    void AppendChild(Pane*);
    void PrependChild(Pane*);
    void InsertChild(Pane*, Pane*);
    void RemoveChild(Pane*);
    // The animated copy of the list (PaneFlagEx_ExtUserDataAnimationEnabled) replaces the resource's list as the
    // source of the entries.
    u16 GetExtUserDataCount() const;
    const ResExtUserData* GetExtUserDataArray() const;
    const ResExtUserData* FindExtUserDataByName(const char*) const;

    // Public: eui::Screen::m80 (0x71009cf8a4) inlines it on a pane of another class.
    void SetVisible(bool state) { detail::SetBit(&mFlags, PaneFlag_Visible, state); }

    void Show() { SetVisible(true); }
    void Hide() { SetVisible(false); }

    bool IsVisible() const { return detail::TestBit(mFlags, PaneFlag_Visible); }
    bool IsInfluencedAlpha() const { return detail::TestBit(mFlags, PaneFlag_InfluencedAlpha); }
    bool IsLocationAdjust() const { return detail::TestBit(mFlags, PaneFlag_LocationAdjust); }
    bool IsUserAllocated() const { return detail::TestBit(mFlags, PaneFlag_UserAllocated); }
    bool IsGlobalMatrixDirty() const {
        return detail::TestBit(mFlags, PaneFlag_IsGlobalMatrixDirty);
    }
    bool IsUserMatrix() const { return detail::TestBit(mFlags, PaneFlag_UserMatrix); }
    bool IsUserGlobalMatrix() const { return detail::TestBit(mFlags, PaneFlag_UserGlobalMatrix); }
    bool IsConstantBufferReady() const {
        return detail::TestBit(mFlags, PaneFlag_IsConstantBufferReady);
    }
    bool IsMaxPanelFlag() const { return detail::TestBit(mFlags, PaneFlag_MaxPaneFlag); }

    Pane* GetParent() const { return mParent; }
    PaneList& GetChildList() { return mChildList; }
    const char* GetName() const { return mPanelName; }

    HorizontalPosition GetBasePositionH() const {
        return static_cast<HorizontalPosition>(mBasePosition & 3);
    }
    VerticalPosition GetBasePositionV() const {
        return static_cast<VerticalPosition>((mBasePosition >> 2) & 3);
    }

    const util::Float3& GetPosition() const { return mPosition; }
    void SetPosition(const util::Float3& position) {
        mPosition = position;
        SetGlobalMatrixDirty(true);
    }
    // Only x and y are stored (z is kept).
    void SetPosition(const util::Float2& position) {
        mPosition.x = position.x;
        mPosition.y = position.y;
        SetGlobalMatrixDirty(true);
    }

    const util::Float3& GetRotation() const { return mRotation; }
    void SetRotation(const util::Float3& rotation) {
        mRotation = rotation;
        SetGlobalMatrixDirty(true);
    }

    const util::Float2& GetScale() const { return mScale; }
    void SetScale(const util::Float2& scale) {
        mScale = scale;
        SetGlobalMatrixDirty(true);
    }

    const Size& GetSize() const { return mSize; }
    void SetSize(const Size& size) {
        mSize.Set(size.width, size.height);
        SetGlobalMatrixDirty(true);
    }

    void SetAlpha(u8 alpha) { mAlpha = alpha; }
    // inline-only in the original; name is a guess. The same alpha read occurs
    // in Pane::GetColorElement and ScreenGameOver::sub_7100A0A914.
    u8 GetAlpha() const { return mAlpha; }

    u8 GetGlobalAlpha() const { return mGlobalAlpha; }

    const util::MatrixT4x3fType& GetMtx() const { return mMtx; }

protected:
    virtual void LoadMtx(DrawInfo&);
    virtual Pane* FindPaneByNameRecursive(const char*);
    virtual const Pane* FindPaneByNameRecursive(const char*) const;
    virtual Material* FindMaterialByNameRecursive(const char*);
    virtual const Material* FindMaterialByNameRecursive(const char*) const;

    util::Float2 GetVertexPos() const;

    void SetInfluencedAlpha(bool state) {
        detail::SetBit(&mFlags, PaneFlag_InfluencedAlpha, state);
    }
    void SetLocationAdjust(bool state) { detail::SetBit(&mFlags, PaneFlag_LocationAdjust, state); }
    void SetUserAllocated(bool state) { detail::SetBit(&mFlags, PaneFlag_UserAllocated, state); }
    void SetGlobalMatrixDirty(bool state) {
        detail::SetBit(&mFlags, PaneFlag_IsGlobalMatrixDirty, state);
    }
    void SetUserMatrix(bool state) { detail::SetBit(&mFlags, PaneFlag_UserMatrix, state); }
    void SetUserGlobalMatrix(bool state) {
        detail::SetBit(&mFlags, PaneFlag_UserGlobalMatrix, state);
    }
    void setConstantBufferReady(bool state) {
        detail::SetBit(&mFlags, PaneFlag_IsConstantBufferReady, state);
    }
    void setMaxPanelFlag(bool state) { detail::SetBit(&mFlags, PaneFlag_MaxPaneFlag, state); }

private:
    // inline-only in the original; name is a guess.
    // The bounded comparison repeats in both Pane searches and Parts' recursive search.
    bool IsNameEqual(const char* name) const {
        for (s32 i = 0; i < 24; ++i) {
            if (mPanelName[i] != name[i])
                return false;
            if (mPanelName[i] == '\0')
                return true;
        }
        return true;
    }

    void Initialize();
    const Pane& operator=(const Pane&);
    void CalculateScaleFromPartsRoot(util::Float2*, Pane*) const;
    void AllocateAndCopyAnimatedExtUserData(const ResExtUserDataList*);
    void CalculateGlobalMatrixSelf(CalculateContext&);

    Pane* mParent;
    PaneList mChildList;
    util::Float3 mPosition;
    util::Float3 mRotation;
    util::Float2 mScale;
    Size mSize;
    u8 mFlags;
    u8 mAlpha;
    u8 mGlobalAlpha;
    u8 mBasePosition;
    u8 mFlagEx;
    u32 mSystemDataFlags;
    Layout* mLayout;
    util::MatrixT4x3fType mMtx;
    const util::MatrixT4x3fType* mUserMtx;
    const ResExtUserDataList* mExtUserDataList;
    ResExtUserDataList* mAnimExtUserData;
    char mPanelName[25];
    char mUserData[9];
    u16 _DA;
    u32 _DC;
};
}  // namespace nn::ui2d
