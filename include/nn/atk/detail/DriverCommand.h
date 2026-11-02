#pragma once

namespace nn::atk::detail {

class CommandManager;

// The native singleton constructs and returns a CommandManager. Reference return and class
// scope are inferred from the call sites and mangled name; no singleton body is provided here.
class DriverCommand {
public:
    static CommandManager& GetInstance();
    static void RequestProcessCommand();
};

}  // namespace nn::atk::detail
