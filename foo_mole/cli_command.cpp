#include "stdafx.h"
#include "cli_command.h"

namespace cli {

Command::Command(const char* name, std::vector<ArgType> arg_types)
  : name_(name), arg_types_(std::move(arg_types)) {
}

void Command::on_file(const char* url) {
  if (!strncmp(url, "file://",7)) {
    url += 7;
  }

  if (arg_types_.size() > args_.size()) {
    if (PATH != arg_types_[args_.size()]) {
      pfc::string8 cwd;
      uGetCurrentDirectory(cwd);
      if (0 == pfc::stricmp_ascii_partial(url, cwd.c_str())) {
        args_.push_back(url + cwd.get_length() + 1);
      }
      else {
        mlog << "failed to extract token: expected full path in CWD ('" << cwd << "') but given '" << url << "'";
        args_.push_back(url);
      }
    }
    else {
      args_.push_back(url);
    }
  }
  else {
    skipped_ ++;
  }
}

void Command::on_files_done() {
  if (arg_types_.size() > args_.size()) {
    mlog << "skipped command /" << name_ << ": expected " << arg_types_.size() << " argument"
         << (1 == arg_types_.size() ? "" : "s") << " but given " << args_.size();
    return;
  }

  if (skipped_ > 0) {
    mlog << "skipped " << skipped_ << " argument" << (1 == skipped_ ? "" : "s") << " for command /" << name_
         << ": command accepts " << arg_types_.size() << " argument" << (1 == arg_types_.size() ? "" : "s");
  }

  try {
    run(args_);
  }
  catch (const InvalidArg& e) {
    mlog << "invalid arg #" << (e.zero_based_index_ + 1) << " for command /" << name_ << ": " << e.what();
  }
  catch (const std::exception& e) {
    mlog << "failed to execute command /" << name_ << ": " << e.what();
  }
}

commandline_handler::result Command::on_token(const char* token) {
  core_api::ensure_main_thread();

  if ('/' == *token && 0 == strcmp(token + 1, name_.c_str())) {
    return RESULT_PROCESSED_EXPECT_FILES;
  }

  return RESULT_NOT_OURS;
}

} // namespace cli
