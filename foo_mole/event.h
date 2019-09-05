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

class Bridge {
public:
  void Schedule(const char* event, const char* commands);

protected:
  void OnEvent(const char* event);

private:
  using Commands = std::vector<std::string>;
  std::map<std::string, Commands> schedule_;
};

#define BRIDGE_EVENT(name, ...) \
  void name(__VA_ARGS__) override { OnEvent(#name); }

class GroupFromBridge: public Group {
public:
  GroupFromBridge(const char* name, Bridge& bridge): name_(name), bridge_(bridge) {}

  const char* GetName() override {
    return name_.c_str();
  }

  void Schedule(const char* event, const char* commands) override {
    return bridge_.Schedule(event, commands);
  }

private:
  std::string name_;
  Bridge& bridge_;
};

} // namespace event
