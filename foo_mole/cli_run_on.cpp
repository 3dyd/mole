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
static CommandSynopsis g_run_on_synopsis(
  "/run_on/<event>/<command_line_base64>\n\trun foobar2000.exe with given args on given event");

} // namespace cli
