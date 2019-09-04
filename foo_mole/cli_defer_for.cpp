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
    pfc::stringcvt::string_wide_from_utf8 commands_wide(commands, commands.get_length());
    auto cmdline = std::wstring(L"foobar2000.exe ").append(commands_wide);

    ArgsProcessed();

    std::thread deferred([cmdline{std::move(cmdline)}, delay]() {
      std::this_thread::sleep_for(delay);

      if (0 != _wsystem(cmdline.c_str())) {
        auto code = errno;
        mlog << "failed to execute foobar2000.exe while running /defer_for command (errno=" << code << ")";
      }
    });

    deferred.detach();
  }
};

static commandline_handler_factory_t<DeferFor> g_defer_for;

} // namespace cli
