#pragma once

class Synopsis {
public:
  Synopsis();
  ~Synopsis();

  virtual const char* GetSection() = 0;
  virtual std::string GetInfo() = 0;

  static std::string Collect();

private:
  static Synopsis* s_list;
  Synopsis* next_;
};

class CommandSynopsis: public Synopsis {
public:
  CommandSynopsis(const char* synopsis): synopsis_(synopsis) {}

  const char* GetSection() override {
    return "Command format: /<command_name>[/<arg>...]\nCommands:";
  }

  std::string GetInfo() override {
    return synopsis_;
  }

private:
  std::string synopsis_;
};
