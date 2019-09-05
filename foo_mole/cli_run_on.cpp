#include "stdafx.h"
#include "cli_command.h"

#include "event.h"

namespace cli {

class RunOn: public Command {
public:
  RunOn(): Command("run_on", 2) {}

  void Run() override {
    auto event_spec = NextArg();

    pfc::string8 commands;
    base64_decode_to_string(commands, NextArg().c_str());

    ArgsProcessed();

    event::Schedule(event_spec.c_str(), commands.c_str());
  }
};

static commandline_handler_factory_t<RunOn> g_run_on;

} // namespace cli
