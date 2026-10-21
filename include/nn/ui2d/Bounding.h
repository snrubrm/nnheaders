/**
 * @file Bounding.h
 * @brief UI pane that only has a bounding rectangle (it draws nothing).
 */

#pragma once

#include <nn/ui2d/Pane.h>

namespace nn::ui2d {
struct ResBounding;

// Declarations only. Bounding::Bounding (0x7100abfe78) passes the first resource to Pane::Pane and stores the
// vtable; the complete destructor 0x7100abfedc branches to Pane::~Pane, so the class adds no members.
class Bounding : public Pane {
public:
    NN_RUNTIME_TYPEINFO(Pane)

    Bounding(const ResBounding*, const ResBounding*, const BuildArgSet&);  // 0x7100abfe78
    Bounding(const Bounding&);                                             // 0x7100abfeac

    ~Bounding() override;                                    // D0 0x7100abfee0
    void DrawSelf(DrawInfo&, gfx::CommandBuffer&) override;  // 0x7100abff04
};

}  // namespace nn::ui2d
