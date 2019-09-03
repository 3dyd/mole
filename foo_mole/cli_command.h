#pragma once

namespace cli {

struct InvalidValue: std::logic_error {
  using logic_error::logic_error;
};

class Command: public commandline_handler {
public:
  Command(const char* name, size_t arg_count);

  virtual void Run() = 0;

protected:
  const std::string& NextArg();

private:
  result on_token(const char* token) final;

private:
  pfc::string8 name_;
  size_t arg_count_ = 0;
  std::deque<std::string> args_;
  std::string next_arg_;
};

} // namespace cli
