#include "stdafx.h"
#include <fcntl.h>
#include <io.h>
#include "../pfc/pfc-fb2k-hooks.h"

class cli_attach_stdout: public commandline_handler {
public:
  result on_token(const char* token) override {
    core_api::ensure_main_thread();

    if (0 != strcmp(token, "/attach_stdout")) {
      return RESULT_NOT_OURS;
    }

    return RESULT_PROCESSED_EXPECT_FILES;
  }

  void on_file(const char* url) override {
    args_.push_back(url);
  }

  void on_files_done() override {
    if (args_.size() != 1) {
      mlog << "command /attach_stdout expects one argument but given " << args_.size();
      return;
    }

    DWORD process_id = ATTACH_PARENT_PROCESS;
    if ("parent" != args_.front()) {
      process_id = std::stoi(args_.front());
    }

    FreeConsole();

    if (AttachConsole(process_id)) {
      freopen("CON", "w", stdout);

      if (ATTACH_PARENT_PROCESS == process_id) {
        mlog << "attached stdout to console of the parent process";
      }
      else {
        mlog << "attached stdout to console of process with pid=" << process_id;
      }
    }
    else {
      DWORD code = GetLastError();
      pfc::string8 message;
      if (!pfc::winFormatSystemErrorMessageImpl(message, code)) {
        message << "error #" << code;
      }
      mlog << "failed to attached console: " << message;
    }
  }

private:
  std::vector<std::string> args_;
};

static commandline_handler_factory_t<cli_attach_stdout> g_cli_attach_stdout;
