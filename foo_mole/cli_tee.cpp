#include "stdafx.h"
#include "cli_command.h"
#include <iostream>

class ConsoleMirror: public console_receiver {
public:
  void MirrorToStdout() {
    stdout_ = true;
  }

private:
  void print(const char* message, t_size length) override {
    if (stdout_) {
      if (~0 == length) {
        length = strlen(message);
      }
      std::cout << std::string_view(message, length) << std::endl;
    }
  }

private:
  std::atomic_bool stdout_ = false;
};

static service_factory_single_t<ConsoleMirror> g_console_mirror;

namespace cli {

class Tee: public Command {
public:
  Tee(): Command("tee", {TOKEN, TOKEN}) {}

  void Run(const std::vector<std::string>& args) override {
    if ("console" != args[0]) {
      throw InvalidArg(0, "allowed only 'console' but given '" + args[0] + "'");
    }
    if ("stdout" != args[1]) {
      throw InvalidArg(1, "allowed only 'stdout' but given '" + args[1] + "'");
    }

    g_console_mirror.get_static_instance().MirrorToStdout();
  }

private:
  std::vector<std::string> args_;
};

static commandline_handler_factory_t<Tee> g_tee;

} // namespace cli
