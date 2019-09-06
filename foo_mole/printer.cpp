#include "stdafx.h"
#include "printer.h"

Printer& Printer::Get() {
  static Printer instance;
  return instance;
}

void Printer::Print(const char* text) {
  std::lock_guard<std::mutex> lock(mutex_);
  printer_(text);
}

void Printer::SetDestination(Destination destination, const char* path) {
  std::lock_guard<std::mutex> lock(mutex_);

  switch (destination) {
    case Destination::Console:
      printer_ = console::print;
      break;
    case Destination::Popup:
      printer_ = [](const char* text) {
        popup_message::g_show(text, APP_NAME);
      };
      break;
    case Destination::Stdout:
      printer_ = puts;
      break;
    case Destination::File:
      {
        pfc::stringcvt::string_wide_from_utf8 path16(path);
        FILE* ptr;
        if (0 == _wfopen_s(&ptr, path16, L"wb")) {
          std::shared_ptr<FILE> file(ptr, fclose);
          printer_ = [file](const char* text) {
            fwrite(text, 1, strlen(text), file.get());
          };
        }
        else {
          auto code = errno;
          throw std::runtime_error("failed to open file (errno=" + std::to_string(code) + ")");
        }
      }
      break;
    default:
      throw std::invalid_argument("invalid destination for the printer: " + std::to_string(static_cast<int>(destination)));
  }
}
