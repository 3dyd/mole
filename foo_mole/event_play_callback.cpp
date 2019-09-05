#include "stdafx.h"
#include "event.h"

namespace event {

class PlayCallbackBridge: public play_callback_static, public Bridge {
  unsigned get_flags() override {
    return flag_on_playback_all;
  }

  BRIDGE_EVENT(on_playback_starting, play_control::t_track_command, bool);
  BRIDGE_EVENT(on_playback_new_track, metadb_handle_ptr);
  BRIDGE_EVENT(on_playback_stop, play_control::t_stop_reason);
  BRIDGE_EVENT(on_playback_seek, double);
  BRIDGE_EVENT(on_playback_pause, bool);
  BRIDGE_EVENT(on_playback_edited, metadb_handle_ptr);
  BRIDGE_EVENT(on_playback_dynamic_info, const file_info&);
  BRIDGE_EVENT(on_playback_dynamic_info_track, const file_info&);
  BRIDGE_EVENT(on_playback_time, double);
  BRIDGE_EVENT(on_volume_change, float);
};

static service_factory_single_t<PlayCallbackBridge> g_play_callback_bridge;

static service_factory_single_t<GroupFromBridge> g_play_callback(
  "play_callback", g_play_callback_bridge.get_static_instance());

} // namespace event
