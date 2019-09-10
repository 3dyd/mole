#pragma once

namespace event {

void RunFoobar2000(const char* args);
void Schedule(const char* spec, const char* commands);

class Group: public service_base {
public:
  virtual const char* GetName() = 0;
  virtual void Schedule(const char* event, const char* commands) = 0;

  FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT(Group);
};

using EventNames = std::vector<std::string>;

class GroupImpl: public Group {
public:
  GroupImpl(const char* name, EventNames supported_events);

  const char* GetName() override;
  void Schedule(const char* event, const char* commands) override;
  void OnEvent(const char* event);

private:
  std::string name_;
  EventNames supported_events_;
  using Commands = std::vector<std::string>;
  std::map<std::string, Commands> schedule_;
};

struct Bridge {
  Bridge(GroupImpl& parent): parent_(parent) {}
  GroupImpl& parent_;
};

#define BRIDGE_EVENT(name, ...) \
  void name(__VA_ARGS__) override { parent_.OnEvent(#name); }

template <class T>
class BridgedGroup: public GroupImpl {
public:
  BridgedGroup(const char* name, EventNames supported_events)
    : GroupImpl(name, std::move(supported_events)), bridge_(*this) {}

private:
  service_factory_single_t<T> bridge_;
};

} // namespace event
