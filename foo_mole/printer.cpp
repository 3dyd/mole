#include "stdafx.h"
#include "printer.h"

Printer& Printer::Get() {
  static Printer instance;
  return instance;
}

void Printer::Print(const char* text) {
  switch (destination_) {
    case Destination::Console:
      console::print(text);
      break;
    case Destination::Popup:
      popup_message::g_show(text, APP_NAME);
      break;
    case Destination::Stdout:
      puts(text);
      break;
  }
}

void Printer::SetDestination(Destination destination) {
  destination_ = destination;
}
