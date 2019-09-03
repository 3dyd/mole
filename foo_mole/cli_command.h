#pragma once

namespace cli {

struct InvalidArg: std::logic_error {
  InvalidArg(size_t zero_based_index, std::string reason)
    : std::logic_error(reason), zero_based_index_(zero_based_index) {}

  const int zero_based_index_;
};

class Command: public commandline_handler {
public:
  enum ArgType {
    TOKEN,
    PATH
  };

  Command(const char* name, std::vector<ArgType> arg_types);

  virtual void Run(const std::vector<std::string>& args) = 0;

private:
  result on_token(const char* token) final;
  void on_file(const char* url) final;
  void on_files_done() final;

private:
  pfc::string8 name_;
  std::vector<ArgType> arg_types_;
  std::vector<std::string> args_;
  size_t skipped_ = 0;
};

} // namespace cli
