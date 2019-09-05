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
    else {
      throw std::invalid_argument("invalid destination given");
    }

    ArgsProcessed();
  }
};

static commandline_handler_factory_t<PrintTo> g_print_to;

} // namespace cli
