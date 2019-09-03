#include "stdafx.h"
#include "cli_command.h"

class ConsoleMirror: public console_receiver {
public:
  void MirrorToStdout() {
    core_api::ensure_main_thread();
    stdout_ = true;
  }

private:
  void print(const char* message, t_size message_length) override {
    // Better slower than messing in console by simultaneous writing
    fb2k::inMainThread2([this, text = pfc::string8(message, message_length)]() {
      if (stdout_) {
        puts(text.c_str());
      }
    });
  }

private:
  bool stdout_ = false;
};

static service_factory_single_t<ConsoleMirror> g_console_mirror;

namespace cli {

class Tee: public Command {
public:
  Tee(): Command("tee", {TOKEN, TOKEN}) {}

  void run(const std::vector<std::string>& args) override {
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
