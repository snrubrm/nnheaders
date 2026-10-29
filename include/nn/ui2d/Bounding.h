/**
 * @file Bounding.h
 * @brief UI pane that only has a bounding rectangle (it draws nothing).
 */

#pragma once

#include <nn/ui2d/Pane.h>

namespace nn::ui2d {
// Known resource prefix: the constructor passes it directly to Pane without reading later fields.
// The full resource extent is not established.
struct ResBounding : ResPane {};

// Bounding adds no pane members; its complete destructor delegates to Pane's destructor.
class Bounding : public Pane {
public:
    NN_RUNTIME_TYPEINFO(Pane)

    Bounding(const ResBounding*, const ResBounding*, const BuildArgSet&);  // 0x7100abfe78
    Bounding(const Bounding&);                                             // 0x7100abfeac

    ~Bounding() override;                                    // D0 0x7100abfee0
    void DrawSelf(DrawInfo&, gfx::CommandBuffer&) override;  // 0x7100abff04
};
static_assert(sizeof(Bounding) == sizeof(Pane));

}  // namespace nn::ui2d
