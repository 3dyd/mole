#include "stdafx.h"
#include "cli_command.h"
#include <chrono>
#include <thread>
#include "event.h"

namespace cli {

class DeferFor: public Command {
public:
  DeferFor(): Command("defer_for", 2) {}

  void Run() override {
    std::chrono::seconds delay(std::stoull(NextArg()));

    pfc::string8 commands;
    base64_decode_to_string(commands, NextArg().c_str());

    ArgsProcessed();

    std::thread deferred([commands, delay]() {
      std::this_thread::sleep_for(delay);
      event::RunFoobar2000(commands.c_str());
    });

    deferred.detach();
  }
};

static commandline_handler_factory_t<DeferFor> g_defer_for;
static CommandSynopsis g_defer_for_synopsis(
  "/defer_for/<seconds>/<command_line_base64>\trun foobar2000.exe with given command line after given number of seconds");

} // namespace cli
