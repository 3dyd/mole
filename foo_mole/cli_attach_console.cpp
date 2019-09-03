#include "stdafx.h"
#include <fcntl.h>
#include <io.h>
#include "../pfc/pfc-fb2k-hooks.h"

class console_mirror: public console_receiver {
public:
  void print(const char* message, t_size message_length) override {
    // Better slower than messing in console by simultaneous writing
    fb2k::inMainThread2([this, text = pfc::string8(message, message_length)]() {
      if (attached_) {
        puts(text.c_str());
      }
    });
  }

  void reset_attached(bool attached) {
    core_api::ensure_main_thread();
    attached_ = attached;
  }

private:
  bool attached_ = false;
};

static service_factory_single_t<console_mirror> g_console_mirror;

class cli_attach_console: public commandline_handler {
public:
  result on_token(const char* token) override {
    core_api::ensure_main_thread();

    if (0 != strcmp(token, "/attach_console")) {
      return RESULT_NOT_OURS;
    }

    g_console_mirror.get_static_instance().reset_attached(false);
    FreeConsole();

    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
      freopen("CON", "w", stdout);

      mlog << "attached stdout to parent console";
      g_console_mirror.get_static_instance().reset_attached(true);
    }
    else {
      DWORD code = GetLastError();
      pfc::string8 message;
      if (!pfc::winFormatSystemErrorMessageImpl(message, code)) {
        message << "error #" << code;
      }
      mlog << "failed to attached console: " << message;
    }

    return RESULT_PROCESSED;
  }
};

static service_factory_single_t<cli_attach_console> g_cli_attach_console;
