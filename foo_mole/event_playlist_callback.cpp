#include "stdafx.h"
#include "event.h"

namespace event {

class PlaylistCallbackBridge: public playlist_callback_static, public Bridge {
  unsigned get_flags() override {
    return flag_all;
  }

  BRIDGE_EVENT(on_items_added, t_size, t_size, const pfc::list_base_const_t<metadb_handle_ptr>&, const bit_array&);
  BRIDGE_EVENT(on_items_reordered, t_size, const t_size*, t_size);
  BRIDGE_EVENT(on_items_removing, t_size, const bit_array&, t_size, t_size);
  BRIDGE_EVENT(on_items_removed, t_size, const bit_array&, t_size, t_size);
  BRIDGE_EVENT(on_items_selection_change, t_size, const bit_array&, const bit_array&);
  BRIDGE_EVENT(on_item_focus_change, t_size, t_size, t_size);
  BRIDGE_EVENT(on_items_modified, t_size, const bit_array&);
  BRIDGE_EVENT(on_items_modified_fromplayback, t_size, const bit_array&, play_control::t_display_level);
  BRIDGE_EVENT(on_items_replaced, t_size, const bit_array&, const pfc::list_base_const_t<t_on_items_replaced_entry>&);
  BRIDGE_EVENT(on_item_ensure_visible, t_size, t_size);
  BRIDGE_EVENT(on_playlist_activate, t_size, t_size);
  BRIDGE_EVENT(on_playlist_created, t_size, const char*, t_size);
  BRIDGE_EVENT(on_playlists_reorder, const t_size*, t_size);
  BRIDGE_EVENT(on_playlists_removing, const bit_array&, t_size, t_size);
  BRIDGE_EVENT(on_playlists_removed, const bit_array&, t_size, t_size);
  BRIDGE_EVENT(on_playlist_renamed, t_size, const char*, t_size);
  BRIDGE_EVENT(on_default_format_changed);
  BRIDGE_EVENT(on_playback_order_changed, t_size);
  BRIDGE_EVENT(on_playlist_locked, t_size, bool);
};

static service_factory_single_t<PlaylistCallbackBridge> g_playlist_callback_bridge;

static service_factory_single_t<GroupFromBridge> g_playlist_callback(
  "playlist_callback", g_playlist_callback_bridge.get_static_instance());

} // namespace event
