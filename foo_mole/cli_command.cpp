#include "stdafx.h"
#include "cli_command.h"
#include <sstream>
#include <boost/algorithm/string.hpp>

namespace cli {

Command::Command(const char* name, size_t arg_count): arg_count_(arg_count) {
  name_ << "/" << name;
}

void Command::ArgsProcessed() {
  next_arg_.reset();

  if (!args_.empty()) {
    throw std::logic_error(UnprocessedArgsError());
  }
}

const std::string& Command::NextArg() {
  if (args_.empty()) {
    throw std::logic_error("error in CLI logic: requested next arg more times than expected");
  }

  next_arg_ = std::move(args_.front());
  args_.pop_front();

  return *next_arg_;
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

  bool succeeded = false;
  try {
    Run();
    succeeded = true;
  }
  catch (const std::exception& e) {
    if (next_arg_) {
      mlog << "error while processing arg #" << (arg_count_ - args_.size()) << " ('"
           << next_arg_->c_str() << "')" << " for command " << name_ << ": " << e.what();
    }
    else {
      mlog << "failed to execute command " << name_ << ": " << e.what();
    }
  }

  if (succeeded) {
    if (next_arg_) {
      mlog << "error in " << name_ << " command logic: arg processing finish was not stated";
    }
    if (!args_.empty()) {
      mlog << UnprocessedArgsError().c_str();
    }
  }

  return RESULT_PROCESSED;
}

std::string Command::UnprocessedArgsError() const {
  std::ostringstream os;
  os << "error in " << name_ << " command logic: " << args_.size() << " argument"
     << (1 == args_.size() ? " was" : "s were") << " left unprocessed";
  return os.str();
}

} // namespace cli
