#include "stdafx.h"

class console_mirror: public console_receiver {
public:
  void print(const char* message, t_size message_length) override {
    // Better slower than messing in console by simultaneous writing
    fb2k::inMainThread2([this, text = pfc::string8(message, message_length)]() {
      if (stdout_) {
        puts(text.c_str());
      }
    });
  }

  void mirror_to_stdout() {
    core_api::ensure_main_thread();
    stdout_ = true;
  }

private:
  bool stdout_ = false;
};

static service_factory_single_t<console_mirror> g_console_mirror;

class cli_tee: public commandline_handler {
public:
  result on_token(const char* token) override {
    if (0 != strcmp(token, "/tee")) {
      return RESULT_NOT_OURS;
    }
    return RESULT_PROCESSED_EXPECT_FILES;
  }

  void on_file(const char* url) override {
    args_.push_back(url);
  }

  void on_files_done() override {
    if (args_.size() != 2) {
      mlog << "command /tee expects two arguments but given " << args_.size();
      return;
    }

    if ("console" != args_[0]) {
      mlog << "unexpected 1st arg for command /tee: allowed only 'console' but given '" << args_[0].c_str() << "'";
      return;
    }

    if ("stdout" != args_[1]) {
      mlog << "unexpected 2nd arg for command /tee: allowed only 'stdout' but given '" << args_[0].c_str() << "'";
      return;
    }

    g_console_mirror.get_static_instance().mirror_to_stdout();
  }

private:
  std::vector<std::string> args_;
};

static commandline_handler_factory_t<cli_tee> g_cli_tee;
