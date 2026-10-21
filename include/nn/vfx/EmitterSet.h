#pragma once

#include <nn/types.h>

namespace nn {
namespace vfx {

// Partial model: only the create id at +0x2c is placed
// (xlink2::OneEmitterMgr::releaseOneEmitterInstance 0x7100bce19c compares it).
class EmitterSet {
public:
    // 0x7100acaaf4
    void Kill(bool immediate);

    int GetCreateId() const { return m_CreateId; }

private:
    u8 _0[0x2c];
    int m_CreateId;
};

}  // namespace vfx
}  // namespace nn
