#include "stdafx.h"
#include <fcntl.h>
#include <io.h>
#include "cli_command.h"

namespace cli {

class AttachStdout: public Command {
public:
  AttachStdout(): Command("attach_stdout", 1) {}

  void Run() override {
    DWORD process_id = ATTACH_PARENT_PROCESS;
    if (auto arg = NextArg(); "parent" != arg) {
      process_id = std::stoi(arg);
    }

    s_stdout.reset();
    FreeConsole();

    if (!AttachConsole(process_id)) {
      throw exception_win32(GetLastError());
    }

    FILE* reopened;
    if (0 != freopen_s(&reopened, "CON", "w", stdout)) {
      auto code = errno;
      throw std::runtime_error("freopen failed (errno=" + std::to_string(code) + ")");
    }
    s_stdout.reset(reopened, fclose);

    if (ATTACH_PARENT_PROCESS == process_id) {
      mlog << "attached stdout to console of the parent process";
    }
    else {
      mlog << "attached stdout to console of process with pid=" << process_id;
    }
  }

private:
  static std::shared_ptr<FILE> s_stdout; // it is accessed only in main thread
};

std::shared_ptr<FILE> AttachStdout::s_stdout;

static commandline_handler_factory_t<AttachStdout> g_attach_stdout;

} // namespace cli
