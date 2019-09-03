#include "stdafx.h"
#include "cli_command.h"
#include <boost/algorithm/string.hpp>

namespace cli {

Command::Command(const char* name, size_t arg_count): arg_count_(arg_count) {
  name_ << "/" << name;
}

const std::string& Command::NextArg() {
  if (args_.empty()) {
    throw std::logic_error("error in CLI logic: requested next arg more times than expected");
  }

  std::swap(next_arg_, args_.front());
  args_.pop_front();

  return next_arg_;
}

commandline_handler::result Command::on_token(const char* token) {
  core_api::ensure_main_thread();

  if (0 != pfc::strcmp_partial(token, name_.c_str()) || ':' != token[name_.get_length()]) {
    return RESULT_NOT_OURS;
  }

  boost::split(args_, std::string(token), [](char ch) { return ':' == ch; });
  args_.pop_front();

  if (args_.size() != arg_count_) {
    mlog << "skipped command " << name_ << ": expected " << arg_count_ << " argument"
         << (1 == arg_count_ ? "" : "s") << " but given " << args_.size();
    return RESULT_PROCESSED;
  }

  try {
    Run();
  }
  catch (const InvalidValue& e) {
    mlog << "invalid arg #" << (arg_count_ - args_.size()) << " ('" << next_arg_.c_str() << "')"
         << " for command " << name_ << ": " << e.what();
  }
  catch (const std::exception& e) {
    mlog << "failed to execute command " << name_ << ": " << e.what();
  }

  if (!args_.empty()) {
    mlog << "error in CLI logic: " << args_.size() << " argument"
         << (1 == args_.size() ? "" : "s") << " are left unprocessed";
  }

  return RESULT_PROCESSED;
}

} // namespace cli
