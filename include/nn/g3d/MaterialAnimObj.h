#pragma once

#include <nn/g3d/ModelAnimObj.h>

namespace nn::g3d {

// Known inheritance prefix for pointer use only. Native type information at
// 0x710252cab0 names MaterialAnimObj and its direct ModelAnimObj base at
// 0x710252ca20. The material-type-3 producer 0x7100bfe96c installs that type.
// Additional state and virtual contracts are not modelled; this declaration
// does not establish the total extent or support allocation/copy/lifetime code.
class MaterialAnimObj : public ModelAnimObj {
public:
    MaterialAnimObj();
    ~MaterialAnimObj() override;
};

}  // namespace nn::g3d
