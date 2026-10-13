#pragma once

namespace nn::atk::detail {

// Static interface only. The instance representation and its relationship to
// CurveLfoParam remain opaque.
class CurveLfo {
public:
    static void InitializeCurveTable();
};

}  // namespace nn::atk::detail
