#include "stdafx.h"
#include "resource.h"
#include "service_wnd.h"
#include "synopsis.h"

class MainMenuCommands: public mainmenu_commands {
public:
  using Commands = std::vector<std::tuple<std::string, GUID, std::function<void()>>>;

  MainMenuCommands(const GUID& parent, Commands commands): parent_(parent), commands_{std::move(commands)} {}

  t_uint32 get_command_count() override { return commands_.size(); }
  GUID get_command(t_uint32 index) override { return std::get<1>(commands_.at(index)); }
  void get_name(t_uint32 index, pfc::string_base& out) override { out = std::get<0>(commands_.at(index)).c_str(); }
  bool get_description(t_uint32 p_index, pfc::string_base & p_out) override { return false; }
  GUID get_parent() override { return parent_; }
  void execute(t_uint32 index, service_ptr_t<service_base>) override { std::get<2>(commands_.at(index))(); }

private:
  GUID parent_;
  Commands commands_;
};

// {BD72DF60-FF16-4D4B-BD25-45D53724D24E}
static const GUID guid_view_menu_group =
{0xbd72df60, 0xff16, 0x4d4b, {0xbd, 0x25, 0x45, 0xd5, 0x37, 0x24, 0xd2, 0x4e}};

static mainmenu_group_popup_factory g_view_menu_group(guid_view_menu_group,
  mainmenu_groups::view, mainmenu_commands::sort_priority_dontcare, APP_NAME);

static service_factory_single_t<MainMenuCommands> g_view_menu_commands(
  guid_view_menu_group, MainMenuCommands::Commands{
    {"Services", {0x1a8c9733, 0xd8b4, 0x4ef7, {0x84, 0xaa, 0x7f, 0xa2, 0xb5, 0xc7, 0x2d, 0x52}}, [] {
      ServiceWnd::Show();
    }}
  });

static service_factory_single_t<MainMenuCommands> g_help_menu_commands(
  mainmenu_groups::help, MainMenuCommands::Commands{
    {APP_NAME, {0x752bbd72, 0xdb0c, 0x49f2, {0x87, 0x9c, 0xcb, 0x1d, 0x55, 0x69, 0x8c, 0x96}}, [] {
      popup_message::g_show(Synopsis::Collect().c_str(), APP_NAME);
    }}
  });
