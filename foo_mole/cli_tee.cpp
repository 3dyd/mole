#include "stdafx.h"
#include "cli_command.h"
#include <iostream>
#include <string_view>

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
  Tee(): Command("tee", 2) {}

  void Run() override {
    if ("console" != NextArg()) {
      throw std::invalid_argument("allowed only 'console' token");
    }
    if ("stdout" != NextArg()) {
      throw std::invalid_argument("allowed only 'stdout' token");
    }
    ArgsProcessed();

    g_console_mirror.get_static_instance().MirrorToStdout();
  }
};

static commandline_handler_factory_t<Tee> g_tee;
static CommandSynopsis g_tee_synopsis(
  "/tee/console/stdout\n\tmirror data from first arg to destination specified in second arg");

} // namespace cli
