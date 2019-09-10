#include "stdafx.h"
#include "cli_command.h"
#include "printer.h"

namespace cli {

class PrintTo: public Command {
public:
  PrintTo(): Command("print_to", 1) {}

  void Run() override {
    auto destination = NextArg();

    if ("console" == destination) {
      Printer::Get().SetDestination(Printer::Destination::Console);
    }
    else if ("popup" == destination) {
      Printer::Get().SetDestination(Printer::Destination::Popup);
    }
    else if ("stdout" == destination) {
      Printer::Get().SetDestination(Printer::Destination::Stdout);
    }
    else if (0 == strncmp(destination.c_str(), "file=", 5)) {
      Printer::Get().SetDestination(Printer::Destination::File, &destination[5]);
    }
    else {
      throw std::invalid_argument("invalid destination given");
    }

    ArgsProcessed();
  }
};

static commandline_handler_factory_t<PrintTo> g_print_to;
static CommandSynopsis g_print_to_synopsis(
  "/print_to/console|popup|stdout|file=<path>\tuse given destination in commands that support it");

} // namespace cli
