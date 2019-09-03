#include "stdafx.h"
#include <fcntl.h>
#include <io.h>
#include "cli_command.h"

namespace cli {

class AttachStdout: public Command {
public:
  AttachStdout(): Command("attach_stdout", {TOKEN}) {}

  void run(const std::vector<std::string>& args) override {
    DWORD process_id = ATTACH_PARENT_PROCESS;
    if ("parent" != args.front()) {
      process_id = std::stoi(args.front());
    }

    FreeConsole();

    if (!AttachConsole(process_id)) {
      throw exception_win32(GetLastError());
    }

    freopen("CON", "w", stdout);

    if (ATTACH_PARENT_PROCESS == process_id) {
      mlog << "attached stdout to console of the parent process";
    }
    else {
      mlog << "attached stdout to console of process with pid=" << process_id;
    }
  }
};

static commandline_handler_factory_t<AttachStdout> g_attach_stdout;

} // namespace cli
