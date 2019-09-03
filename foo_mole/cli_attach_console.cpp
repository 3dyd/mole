#include "stdafx.h"
#include <fcntl.h>
#include <io.h>
#include "../pfc/pfc-fb2k-hooks.h"

class cli_attach_console: public commandline_handler {
public:
  result on_token(const char* token) override {
    core_api::ensure_main_thread();

    if (0 != strcmp(token, "/attach_console")) {
      return RESULT_NOT_OURS;
    }

    FreeConsole();

    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
      freopen("CON", "w", stdout);

      mlog << "attached stdout to parent console";
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
