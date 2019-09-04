#include "stdafx.h"
#include "cli_command.h"
#include <chrono>
#include <thread>

namespace cli {

class DeferFor: public Command {
public:
  DeferFor(): Command("defer_for", 2) {}

  void Run() override {
    std::chrono::seconds delay(std::stoull(NextArg()));

    pfc::string8 commands;
    base64_decode_to_string(commands, NextArg().c_str());

    ArgsProcessed();

    std::thread deferred([cmdline{std::string("foobar2000.exe ").append(commands)}, delay]() {
      std::this_thread::sleep_for(delay);
      system(cmdline.c_str());
    });

    deferred.detach();
  }
};

static commandline_handler_factory_t<DeferFor> g_defer_for;

} // namespace cli
