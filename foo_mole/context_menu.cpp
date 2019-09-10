#include "stdafx.h"
#include "event.h"
#include "printer.h"

// {569EA512-B78F-41FB-8CB8-D60DF0F4C085}
static const GUID guid_context_menu_group =
{ 0x569ea512, 0xb78f, 0x41fb, { 0x8c, 0xb8, 0xd6, 0xd, 0xf0, 0xf4, 0xc0, 0x85 } };

// {F5DCE743-DF4F-4449-B163-D53BC830587E}
static const GUID guid_menu_item_dump =
{ 0xf5dce743, 0xdf4f, 0x4449, { 0xb1, 0x63, 0xd5, 0x3b, 0xc8, 0x30, 0x58, 0x7e } };

static contextmenu_group_popup_factory g_context_menu_group(guid_context_menu_group, contextmenu_groups::root, APP_NAME, 0);

class ContextMenuItems: public contextmenu_item_simple {
public:
  GUID get_parent() override {
    return guid_context_menu_group;
  }

  unsigned get_num_items() override {
    return 1;
  }

  void get_item_name(unsigned p_index,pfc::string_base & p_out) override {
    switch (p_index) {
      case 0: p_out = "Dump"; break;
      default: uBugCheck();
    }
  }

  void context_command(unsigned p_index, metadb_handle_list_cref p_data, const GUID& p_caller) override {
    switch (p_index) {
      case 0: return Dump(p_data);
      default: uBugCheck();
    }
  }

  GUID get_item_guid(unsigned p_index) override {
    switch(p_index) {
      case 0: return guid_menu_item_dump;
      default: uBugCheck();
    }
  }

  bool get_item_description(unsigned p_index,pfc::string_base & p_out) {
    return false;
  }

private:
  void Dump(metadb_handle_list_cref p_data) {
    pfc::string_formatter dump;

    for (size_t i = 0, count = p_data.get_size(); i < count; i ++) {
      const auto& handle = *p_data[i];
      dump << handle.get_subsong_index() << "," << handle.get_path() << "\n";

      file_info_impl info;
      if (handle.get_info(info)) {
        info.to_formatter(dump);
        if (i + 1 != count) {
          dump << "----------------------------------------------------------------------\n";
        }
      }
    }

    Printer::Get().Print(dump);
  }
};

static contextmenu_item_factory_t<ContextMenuItems> g_context_menu_items;
static PrinterTarget g_printer_target_dump("\tcontext menu item '" APP_NAME " > Dump'");
