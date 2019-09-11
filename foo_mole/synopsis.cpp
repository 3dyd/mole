#include "stdafx.h"
#include "synopsis.h"

Synopsis* Synopsis::s_list = nullptr;

Synopsis::Synopsis() {
  CFG_VAR_ASSERT_SAFEINIT;

  next_ = s_list;
  s_list = this;
}

Synopsis::~Synopsis() {
  CFG_VAR_ASSERT_SAFEINIT;
}

std::string Synopsis::Collect() {
  std::map<std::string, std::string> all;
  for (Synopsis* ptr = s_list; !!ptr; ptr = ptr->next_) {
    all[ptr->GetSection()].append("\n\n").append(ptr->GetInfo());
  }

  std::string text;
  for (const auto& section : all) {
    if (!text.empty()) {
      text.append("\n\n").append(75, '-').append("\n\n");
    }
    text.append(section.first).append(section.second);
  }

  return text;
}
