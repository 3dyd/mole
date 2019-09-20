#include "stdafx.h"
#include "event.h"
#include <sstream>

namespace event {

// {BA8B8795-ADDF-4964-B826-8C415F49BC42}
static const GUID guid_branch_to_console =
{0xba8b8795, 0xaddf, 0x4964, {0xb8, 0x26, 0x8c, 0x41, 0x5f, 0x49, 0xbc, 0x42}};

static advconfig_branch_factory cfg_branch_to_console(
  "Events to print to console", guid_branch_to_console, advconfig_entry::guid_branch_debug, 0);

// {F6E8A66C-A290-47E6-B8EE-E6F477C14C05}
FOOGUIDDECL const GUID Group::class_guid =
{0xf6e8a66c, 0xa290, 0x47e6, {0xb8, 0xee, 0xe6, 0xf4, 0x77, 0xc1, 0x4c, 0x5}};

void RunFoobar2000(const char* args) {
  pfc::stringcvt::string_wide_from_utf8 args16(args);

  fb2k::inMainThread([args16]() {
    auto cmdline = std::wstring(L"start foobar2000.exe ").append(args16);

    if (0 != _wsystem(cmdline.c_str())) {
      auto code = errno;
      mlog << "failed to execute foobar2000.exe (errno=" << code << ")";
    }
  });
}

void Schedule(const char* spec, const char* commands) {
  const char* event = strchr(spec, '.');
  if (!event) {
    throw pfc::exception_invalid_params(std::string("invalid event specification: ").append(spec).c_str());
  }

  std::string group(spec, event);
  ++ event;

  service_enum_t<Group> e;
  for (service_ptr_t<Group> p; e.next(p);) {
    if (p->GetName() == group) {
      return p->Schedule(event, commands);
    }
  }

  throw pfc::exception_not_implemented(std::string("no such event: ").append(group).c_str());
}

GroupImpl::GroupImpl(const GUID& guid, const char* name, EventNames supported_events)
  : to_console_(name, guid, guid_branch_to_console, 0, false)
  , supported_events_(std::move(supported_events)) {
}

std::string GroupImpl::GetInfo() {
  std::ostringstream os;
  os << GetName();

  for (const auto& event : supported_events_) {
    os << "\n\t" << event;
  }

  return os.str();
}

std::string GroupImpl::GetName() {
  pfc::string8 name;
  to_console_.get_static_instance().get_name(name);

  return name.c_str();
}

const char* GroupImpl::GetSection() {
  return "Events (usage example: /run_on/play_callback.on_playback_stop/<cmd>):";
}

void GroupImpl::OnEvent(const char* event) {
  if (to_console_.get()) {
    console::formatter() << GetName().c_str() << ": " << event;
  }

  if (auto it = schedule_.find(event); schedule_.end() != it) {
    Commands commands({std::move(it->second)});
    schedule_.erase(it);

    for (const auto& command : commands) {
      RunFoobar2000(command.c_str());
    }
  }
}

void GroupImpl::Schedule(const char* event, const char* commands) {
  if (supported_events_.end() == find(supported_events_.begin(), supported_events_.end(), event)) {
    mlog << "invalid event: " << GetName().c_str() << ":" << event;
  }
  else {
    schedule_[event].push_back(commands);
  }
}

} // namespace event
