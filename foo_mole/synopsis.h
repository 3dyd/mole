#pragma once

class Synopsis {
public:
  Synopsis();
  ~Synopsis();

  virtual const char* GetSection() = 0;
  virtual const char* GetInfo() = 0;

  static std::string Collect();

private:
  static Synopsis* s_list;
  Synopsis* next_;
};

class CommandSynopsis: public Synopsis {
public:
  CommandSynopsis(const char* synopsis): synopsis_(synopsis) {}

  const char* GetSection() override { return "Commands:"; }
  const char* GetInfo() override { return synopsis_.c_str(); }

private:
  std::string synopsis_;
};
