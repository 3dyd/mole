#pragma once

namespace cli {

class Command: public commandline_handler {
public:
  Command(const char* name, size_t arg_count);

  virtual void Run() = 0;

protected:
  const std::string& NextArg();
  void ArgsProcessed();

private:
  result on_token(const char* token) final;
  std::string UnprocessedArgsError() const;

private:
  pfc::string8 name_;
  size_t arg_count_ = 0;
  std::deque<std::string> args_;
  std::optional<std::string> next_arg_;
};

} // namespace cli
