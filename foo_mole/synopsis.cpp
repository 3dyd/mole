#include "stdafx.h"
#include "synopsis.h"

Synopsis* Synopsis::s_list = nullptr;

class HelpMainmenuCommand: public mainmenu_commands {
public:
  t_uint32 get_command_count() {
    return 1;
  }

  GUID get_command(t_uint32 index) {
    if (0 != index) {
      uBugCheck();
    }
    static const GUID guid = {0x752bbd72, 0xdb0c, 0x49f2, {0x87, 0x9c, 0xcb, 0x1d, 0x55, 0x69, 0x8c, 0x96}};
    return guid;
  }

  void get_name(t_uint32 index, pfc::string_base& out) {
    if (0 != index) {
      uBugCheck();
    }
    out = APP_NAME;
  }

  bool get_description(t_uint32 p_index, pfc::string_base & p_out) {
    return false;
  }

  GUID get_parent() {
    return mainmenu_groups::help;
  }

  void execute(t_uint32 index, service_ptr_t<service_base>) {
    if (0 != index) {
      uBugCheck();
    }
    popup_message::g_show(Synopsis::Collect().c_str(), APP_NAME);
  }
};

static mainmenu_commands_factory_t<HelpMainmenuCommand> g_help_mainmenu_command;

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
