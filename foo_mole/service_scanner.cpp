#include "stdafx.h"
#include "service_scanner.h"
#include <algorithm>
#include "../acfu-sdk/acfu.h"
#include "../columns_ui-sdk/columns_ui.h"
#include "../columns_ui-sdk/win32_helpers.h"
#include "../columns_ui-sdk/columns_ui_appearance.h"
#include "../columns_ui-sdk/ui_extension.h"
#include "../foobar2000/SDK/component.h"

#define DECLARE_SERVICE(name)  {name::class_guid, #name}

/*
foobar2000.h:
- #define FOOBAR2000_TARGET_VERSION 78 // 1.3
+ // #define FOOBAR2000_TARGET_VERSION 78 // 1.3
  // #define FOOBAR2000_TARGET_VERSION 79 // 1.4
- // #define FOOBAR2000_TARGET_VERSION 80 // 1.5
+ #define FOOBAR2000_TARGET_VERSION 80 // 1.5
=====
component_client.cpp:
- t_uint32 get_version() {return FOOBAR2000_CLIENT_VERSION;}
+ t_uint32 get_version() {return FOOBAR2000_CLIENT_VERSION_COMPATIBLE;}
*/

const struct {
  const GUID& guid;
  const char* name;
} KNOWN_SERVICES[] = {
  // foobar2000
  DECLARE_SERVICE(hasher_md5),
  DECLARE_SERVICE(config_io_callback),
  DECLARE_SERVICE(componentversion),
  DECLARE_SERVICE(preferences_page),
  DECLARE_SERVICE(preferences_branch),
  DECLARE_SERVICE(console_receiver),
  DECLARE_SERVICE(core_version_info),
  DECLARE_SERVICE(audio_postprocessor),
  DECLARE_SERVICE(dsp_config_manager),
  DECLARE_SERVICE(dsp_config_callback),
  DECLARE_SERVICE(dsp_config_manager_v2),
  DECLARE_SERVICE(dsp_entry),
  DECLARE_SERVICE(dsp),
  DECLARE_SERVICE(dsp_v2),
  DECLARE_SERVICE(dsp_entry_v2),
  DECLARE_SERVICE(dsp_entry_hidden),
  DECLARE_SERVICE(decode_postprocessor_entry),
  DECLARE_SERVICE(decode_postprocessor_instance),
  DECLARE_SERVICE(resampler_entry),
  DECLARE_SERVICE(resampler_manager),
  DECLARE_SERVICE(initquit),
  DECLARE_SERVICE(metadb_display_field_provider),
  DECLARE_SERVICE(metadb),
  DECLARE_SERVICE(metadb_io_callback),
  DECLARE_SERVICE(contextmenu_item),
  DECLARE_SERVICE(visualisation_manager),
  DECLARE_SERVICE(visualisation_stream),
  DECLARE_SERVICE(packet_decoder),
  DECLARE_SERVICE(packet_decoder_streamparse),
  DECLARE_SERVICE(packet_decoder_entry),
  DECLARE_SERVICE(contextmenu_manager),
  DECLARE_SERVICE(input_file_type),
  DECLARE_SERVICE(input_file_type_v2),
  DECLARE_SERVICE(ui_control),
  DECLARE_SERVICE(ui_control_v2),
  DECLARE_SERVICE(ui_status_text_override),
  DECLARE_SERVICE(ui_drop_item_callback),
  DECLARE_SERVICE(commandline_handler),
  DECLARE_SERVICE(modeless_dialog_manager),
  DECLARE_SERVICE(play_callback_manager),
  DECLARE_SERVICE(play_callback_static),
  DECLARE_SERVICE(playback_control),
  DECLARE_SERVICE(playlist_callback_static),
  DECLARE_SERVICE(playlist_callback_single_static),
  DECLARE_SERVICE(playlist_lock),
  DECLARE_SERVICE(filesystem),
  DECLARE_SERVICE(unpacker),
  DECLARE_SERVICE(archive),
  DECLARE_SERVICE(archive_v2),
  DECLARE_SERVICE(file),
  DECLARE_SERVICE(file_dynamicinfo),
  DECLARE_SERVICE(file_dynamicinfo_v2),
  DECLARE_SERVICE(file_cached),
  DECLARE_SERVICE(replaygain_manager),
  DECLARE_SERVICE(replaygain_manager_v2),
  DECLARE_SERVICE(titleformat_compiler),
  DECLARE_SERVICE(user_interface),
  DECLARE_SERVICE(user_interface_v2),
  DECLARE_SERVICE(keyboard_shortcut_manager),
  DECLARE_SERVICE(keyboard_shortcut_manager_v2),
  DECLARE_SERVICE(playlist_manager),
  DECLARE_SERVICE(genrand_service),
  DECLARE_SERVICE(playlist_incoming_item_filter),
  DECLARE_SERVICE(library_manager),
  DECLARE_SERVICE(library_callback),
  DECLARE_SERVICE(popup_message),
  DECLARE_SERVICE(app_close_blocker),
  DECLARE_SERVICE(file_operation_callback),
  DECLARE_SERVICE(config_object),
  DECLARE_SERVICE(config_object_notify_manager),
  DECLARE_SERVICE(config_object_notify),
  DECLARE_SERVICE(titleformat_object),
  DECLARE_SERVICE(tag_processor_id3v2),
  DECLARE_SERVICE(tag_processor_trailing),
  DECLARE_SERVICE(metadb_handle),
  DECLARE_SERVICE(threaded_process),
  DECLARE_SERVICE(threaded_process_callback),
  DECLARE_SERVICE(metadb_io),
  DECLARE_SERVICE(library_viewer),
  DECLARE_SERVICE(message_loop),
  DECLARE_SERVICE(input_decoder),
  DECLARE_SERVICE(input_decoder_v2),
  DECLARE_SERVICE(input_decoder_v3),
  DECLARE_SERVICE(input_decoder_v4),
  DECLARE_SERVICE(input_info_reader),
  DECLARE_SERVICE(input_info_writer),
  DECLARE_SERVICE(input_entry),
  DECLARE_SERVICE(input_entry_v2),
  DECLARE_SERVICE(input_entry_v3),
  DECLARE_SERVICE(input_manager),
  DECLARE_SERVICE(input_stream_selector),
  DECLARE_SERVICE(input_stream_info_reader),
  DECLARE_SERVICE(input_stream_info_reader_entry),
  DECLARE_SERVICE(input_stream_manipulator),
  DECLARE_SERVICE(input_stream_manipulator_callback),
  DECLARE_SERVICE(link_resolver),
  DECLARE_SERVICE(playback_queue_callback),
  DECLARE_SERVICE(main_thread_callback_manager),
  DECLARE_SERVICE(main_thread_callback),
  DECLARE_SERVICE(mainmenu_group),
  DECLARE_SERVICE(mainmenu_group_popup),
  DECLARE_SERVICE(mainmenu_group_popup_v2),
  DECLARE_SERVICE(mainmenu_commands),
  DECLARE_SERVICE(mainmenu_manager),
  DECLARE_SERVICE(playback_statistics_collector),
  DECLARE_SERVICE(advconfig_entry),
  DECLARE_SERVICE(advconfig_branch),
  DECLARE_SERVICE(advconfig_entry_checkbox),
  DECLARE_SERVICE(advconfig_entry_string),
  DECLARE_SERVICE(playback_control_v2),
  DECLARE_SERVICE(preferences_page_v2),
  DECLARE_SERVICE(preferences_branch_v2),
  DECLARE_SERVICE(advconfig_entry_enum),
  DECLARE_SERVICE(info_lookup_handler),
  DECLARE_SERVICE(info_lookup_handler_v2),
  DECLARE_SERVICE(completion_notify),
  DECLARE_SERVICE(metadb_io_v2),
  DECLARE_SERVICE(file_info_filter),
  DECLARE_SERVICE(file_info_filter_v2),
  DECLARE_SERVICE(metadb_hint_list),
  DECLARE_SERVICE(playlist_incoming_item_filter_v2),
  DECLARE_SERVICE(process_locations_notify),
  DECLARE_SERVICE(library_manager_v2),
  DECLARE_SERVICE(track_property_provider),
  DECLARE_SERVICE(track_property_provider_v2),
  DECLARE_SERVICE(library_manager_v3),
  DECLARE_SERVICE(metadb_io_v3),
  DECLARE_SERVICE(search_filter),
  DECLARE_SERVICE(search_filter_manager),
  DECLARE_SERVICE(playlist_manager_v2),
  DECLARE_SERVICE(playlist_manager_v3),
  DECLARE_SERVICE(titleformat_common_methods),
  DECLARE_SERVICE(core_version_info_v2),
  DECLARE_SERVICE(visualisation_stream_v2),
  DECLARE_SERVICE(visualisation_stream_v3),
  DECLARE_SERVICE(album_art_data),
  DECLARE_SERVICE(album_art_extractor),
  DECLARE_SERVICE(album_art_extractor_instance),
  DECLARE_SERVICE(album_art_editor),
  DECLARE_SERVICE(album_art_editor_instance),
  DECLARE_SERVICE(icon_remapping),
  DECLARE_SERVICE(file_operation_callback_dynamic_manager),
  DECLARE_SERVICE(ui_selection_holder),
  DECLARE_SERVICE(ui_selection_manager),
  DECLARE_SERVICE(ui_selection_manager_v2),
  DECLARE_SERVICE(ole_interaction),
  DECLARE_SERVICE(tag_processor_album_art_utils),
  DECLARE_SERVICE(playlist_incoming_item_filter_v3),
  DECLARE_SERVICE(menu_item_resolver),
  DECLARE_SERVICE(app_close_blocking_task_manager),
  DECLARE_SERVICE(message_loop_v2),
  DECLARE_SERVICE(autoplaylist_client),
  DECLARE_SERVICE(autoplaylist_client_factory),
  DECLARE_SERVICE(autoplaylist_manager),
  DECLARE_SERVICE(replaygain_result),
  DECLARE_SERVICE(replaygain_scanner),
  DECLARE_SERVICE(replaygain_scanner_entry),
  DECLARE_SERVICE(replaygain_scanner_entry_v2),
  DECLARE_SERVICE(replaygain_alter_stream),
  DECLARE_SERVICE(replaygain_alter_stream_entry),
  DECLARE_SERVICE(search_filter_manager_v2),
  DECLARE_SERVICE(search_filter_v2),
  DECLARE_SERVICE(autoplaylist_client_v2),
  DECLARE_SERVICE(library_search_ui),
  DECLARE_SERVICE(search_filter_v3),
  DECLARE_SERVICE(event_logger),
  DECLARE_SERVICE(event_logger_recorder),
  DECLARE_SERVICE(playlist_manager_v4),
  DECLARE_SERVICE(ole_interaction_v2),
  DECLARE_SERVICE(autoplaylist_manager_v2),
  DECLARE_SERVICE(library_file_move_scope),
  DECLARE_SERVICE(library_file_move_manager),
  DECLARE_SERVICE(library_file_move_notify),
  DECLARE_SERVICE(library_meta_autocomplete),
  DECLARE_SERVICE(input_protocol_type),
  DECLARE_SERVICE(metadb_hint_list_v2),
  DECLARE_SERVICE(playlist_loader),
  DECLARE_SERVICE(playlist_loader_callback),
  DECLARE_SERVICE(playlist_loader_callback_v2),
  DECLARE_SERVICE(album_art_manager_v2),
  DECLARE_SERVICE(album_art_extractor_instance_v2),
  DECLARE_SERVICE(album_art_path_list),
  DECLARE_SERVICE(album_art_fallback),
  DECLARE_SERVICE(preferences_page_callback),
  DECLARE_SERVICE(preferences_page_instance),
  DECLARE_SERVICE(preferences_page_v3),
  DECLARE_SERVICE(advconfig_entry_string_v2),
  DECLARE_SERVICE(advconfig_entry_checkbox_v2),
  DECLARE_SERVICE(config_io_callback_v2),
  DECLARE_SERVICE(contextmenu_item_v2),
  DECLARE_SERVICE(contextmenu_group_manager),
  DECLARE_SERVICE(contextmenu_group),
  DECLARE_SERVICE(contextmenu_group_popup),
  DECLARE_SERVICE(mainmenu_commands_v2),
  DECLARE_SERVICE(mainmenu_node),
  DECLARE_SERVICE(system_time_keeper),
  DECLARE_SERVICE(component_installation_validator),
  DECLARE_SERVICE(playback_stream_capture),
  DECLARE_SERVICE(http_request),
  DECLARE_SERVICE(http_request_post),
  DECLARE_SERVICE(http_client),
  DECLARE_SERVICE(http_reply),
  DECLARE_SERVICE(popup_message_v2),
  DECLARE_SERVICE(metadb_index_client),
  DECLARE_SERVICE(metadb_index_manager),
  DECLARE_SERVICE(init_stage_callback),
  DECLARE_SERVICE(metadb_io_edit_callback),
  DECLARE_SERVICE(progress_meter_instance),
  DECLARE_SERVICE(progress_meter),
  DECLARE_SERVICE(album_art_manager_config),
  DECLARE_SERVICE(album_art_manager_v3),
  DECLARE_SERVICE(now_playing_album_art_notify_manager),
  DECLARE_SERVICE(album_art_editor_instance_v2),
  DECLARE_SERVICE(input_info_writer_v2),
  DECLARE_SERVICE(playback_control_v3),
  DECLARE_SERVICE(metadb_info_container),
  DECLARE_SERVICE(metadb_hint_list_v3),
  DECLARE_SERVICE(track_property_provider_v3),
  DECLARE_SERVICE(output),
  DECLARE_SERVICE(output_entry),
  DECLARE_SERVICE(output_entry_v2),
  DECLARE_SERVICE(output_entry_v3),
  DECLARE_SERVICE(volume_control),
  DECLARE_SERVICE(output_v2),
  DECLARE_SERVICE(output_v3),
  DECLARE_SERVICE(output_manager),
  DECLARE_SERVICE(output_manager_v2),
  DECLARE_SERVICE(ui_element_instance),
  DECLARE_SERVICE(ui_element),
  DECLARE_SERVICE(ui_element_v2),
  DECLARE_SERVICE(ui_element_instance_callback),
  DECLARE_SERVICE(ui_element_instance_callback_v2),
  DECLARE_SERVICE(ui_element_children_enumerator),
  DECLARE_SERVICE(ui_element_common_methods),
  DECLARE_SERVICE(ui_element_common_methods_v2),
  DECLARE_SERVICE(ui_element_common_methods_v3),
  DECLARE_SERVICE(ui_element_replace_dialog_notify),
  DECLARE_SERVICE(ui_element_popup_host),
  DECLARE_SERVICE(ui_element_popup_host_callback),
  DECLARE_SERVICE(ui_element_config),
  DECLARE_SERVICE(ui_element_instance_callback_v3),
  DECLARE_SERVICE(ui_element_popup_host_v2),
  DECLARE_SERVICE(ui_edit_context),
  DECLARE_SERVICE(ui_edit_context_manager),
  DECLARE_SERVICE(ui_edit_context_playlist),
  DECLARE_SERVICE(file_format_sanitizer),
  DECLARE_SERVICE(file_format_sanitizer_stdtags),
  DECLARE_SERVICE(filesystem_transacted),
  DECLARE_SERVICE(filesystem_transacted_entry),
  DECLARE_SERVICE(config_io_callback_v3),
  DECLARE_SERVICE(filesystem_v2),
  DECLARE_SERVICE(playlist_incoming_item_filter_v4),
  DECLARE_SERVICE(file_lock),
  DECLARE_SERVICE(file_lock_manager),
  DECLARE_SERVICE(track_property_provider_v4),
  DECLARE_SERVICE(album_art_extractor_v2),
  DECLARE_SERVICE(album_art_editor_v2),
  DECLARE_SERVICE(input_manager_v2),
  DECLARE_SERVICE(replaygain_scanner_config),
  DECLARE_SERVICE(metadb_io_v4),
  DECLARE_SERVICE(popup_message_v3),
  DECLARE_SERVICE(file_lowLevelIO),
  DECLARE_SERVICE(async_task_manager),
  // foo_acfu
  DECLARE_SERVICE(acfu::authorization),
  DECLARE_SERVICE(acfu::request),
  DECLARE_SERVICE(acfu::source),
  DECLARE_SERVICE(acfu::updates),
  // foo_ui_columns
  DECLARE_SERVICE(cui::fcl::dataset),
  DECLARE_SERVICE(cui::fcl::dataset_v2),
  DECLARE_SERVICE(cui::fcl::group),
  DECLARE_SERVICE(cui::control),
  DECLARE_SERVICE(cui::colours::manager),
  DECLARE_SERVICE(cui::colours::manager_instance),
  DECLARE_SERVICE(cui::fonts::manager),
  DECLARE_SERVICE(cui::colours::client),
  DECLARE_SERVICE(cui::fonts::client),
  DECLARE_SERVICE(uie::window_host),
  DECLARE_SERVICE(uie::window_host_ex),
  DECLARE_SERVICE(uie::menu_window),
  DECLARE_SERVICE(uie::menu_window_v2),
  DECLARE_SERVICE(uie::window),
  DECLARE_SERVICE(uie::splitter_window),
  DECLARE_SERVICE(uie::splitter_window_v2),
  DECLARE_SERVICE(uie::visualisation),
  DECLARE_SERVICE(uie::visualisation_host),
  DECLARE_SERVICE(uie::button),
  DECLARE_SERVICE(uie::button_v2),
  DECLARE_SERVICE(uie::custom_button),
  DECLARE_SERVICE(uie::menu_button),
  DECLARE_SERVICE(uie::playlist_window),
  DECLARE_SERVICE(uie::window_host_with_control),
};

ServiceScanner::ServiceScanner() {
  auto modules = GetModules();

  for (auto instance = FindInitialFactory(); !!instance; instance = instance->__internal__next) {
    const Origin& origin = EstablishOrigin(instance, modules);
    const Service& service = EstablishService(instance->get_class_guid());
    instances_.emplace_back(origin, service, *instance);
  }

  FillTargetVersion(modules);

  origins_.sort();
  services_.sort();
}

ServiceScanner::OriginMap ServiceScanner::ByOrigin() const {
  OriginMap om;
  for (const auto& instance : instances_) {
    om[&std::get<0>(instance)].emplace(std::get<1>(instance), std::get<2>(instance));
  }
  return om;
}

const ServiceScanner::Origin& ServiceScanner::EstablishOrigin(service_factory_base* service, const std::vector<Module>& modules) {
  std::string origin_name;

  for (const auto& module : modules) {
    char* begin = (char*)module.info.lpBaseOfDll;
    char* end = (char*)module.info.lpBaseOfDll + module.info.SizeOfImage;
    char* cur = (char*)service;
    if (begin <= cur && end > cur) {
      origin_name = module.name;
      break;
    }
  }
  if (origin_name.empty()) {
    origin_name = "???";
  }

  const Origin* origin = nullptr;
  for (const auto& existing : origins_) {
    if (existing.module == origin_name) {
      origin = &existing;
      break;
    }
  }
  if (!origin) {
    origin = &origins_.emplace_back(origin_name);
  }

  return *origin;
}

const ServiceScanner::Service& ServiceScanner::EstablishService(const GUID& guid) {
  for (const auto& service : services_) {
    if (service.guid == guid) {
      return service;
    }
  }

  services_.push_back(Service{"", guid, pfc::print_guid(guid)});

  for (const auto& known : KNOWN_SERVICES) {
    if (known.guid == guid) {
      services_.back().name = known.name;
      break;
    }
  }

  return services_.back();
}

void ServiceScanner::FillTargetVersion(const std::vector<Module>& modules) {
  using Fb2kGetInterface = foobar2000_client* (*)(foobar2000_api*, HINSTANCE);

  for (const auto& module : modules) {
    if (auto entry_point = (Fb2kGetInterface)GetProcAddress(module.handle, "foobar2000_get_interface")) {
      if (auto client = entry_point(g_foobar2000_api, module.handle)) {
        bool found = false;

        for (auto& origin : origins_) {
          if (origin.module == module.name) {
            found = true;
            origin.fb2k_target_version = client->get_version();
          }
        }

        if (!found) {
          origins_.emplace_back(module.name).fb2k_target_version = client->get_version();
        }
      }
    }
  }
}

service_factory_base* ServiceScanner::FindInitialFactory() const {
  service_factory_base* initial = nullptr;

  MODULEINFO module = {0};
  GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &module, sizeof(MODULEINFO));

  char* begin = (char*)module.lpBaseOfDll;
  char* end = (char*)module.lpBaseOfDll + module.SizeOfImage;

  // All factories are part of single chain, so find the one that starts the
  // longest chain
  int max_chain_size = 0;

  for (const auto& known : KNOWN_SERVICES) {
    service_class_ref c = service_factory_base::enum_find_class(known.guid);

    if (service_ptr out; service_factory_base::enum_create(out, c, 0)) {
      out.get_ptr()->service_add_ref();
      int counter = out.get_ptr()->service_add_ref();
      out.get_ptr()->service_release();
      out.get_ptr()->service_release();

      if (1 == counter) { // i.e. looks like service_factory_single_t
        char* cur = (char*)out.get_ptr();

        if (begin <= cur && end > cur) { // in fb2k space
          auto instance = (service_factory_base*)out.get_ptr() - 1;

          if (int chain_size = GetChainSize(instance); chain_size > max_chain_size) {
            max_chain_size = chain_size;
            initial = instance;
          }
        }
      }
    }
  }

  return initial;
}

int ServiceScanner::GetChainSize(service_factory_base* service) const {
  int count = 0;
  __try {
    for (; !!service; service = service->__internal__next) {
      count++;
    }
  }
  __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
    return 0;
  }
  return count;
}

std::string ServiceScanner::GetModuleName(HMODULE module) const {
  wchar_t name[128] = {0};
  GetModuleBaseName(GetCurrentProcess(), module, name, _countof(name) - 1); // -1 for XP

  return pfc::stringcvt::string_utf8_from_os(name).get_ptr();
}

std::vector<ServiceScanner::Module> ServiceScanner::GetModules() const {
  std::vector<Module> modules;

  HANDLE process = GetCurrentProcess();
  HMODULE handles[10240];
  DWORD count;

  if (EnumProcessModules(GetCurrentProcess(), handles, sizeof(handles), &count)) {
    modules.resize(count / sizeof(HMODULE));

    for (DWORD i = 0; i < modules.size(); i ++) {
      if (!GetModuleInformation(process, handles[i], &modules[i].info, sizeof(MODULEINFO))) {
        memset(&modules[i].info, 0, sizeof(MODULEINFO));
      }
      modules[i].handle = handles[i];
      modules[i].name = GetModuleName(handles[i]);
    }
  }

  return modules;
}
