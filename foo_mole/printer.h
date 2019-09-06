#pragma once

class Printer {
public:
  enum class Destination {
    Console,
    Popup,
    Stdout,
    File,
  };

  static Printer& Get();

  void Print(const char* text);
  void SetDestination(Destination destination, const char* path = nullptr);

private:
  Printer() { SetDestination(Destination::Popup); }
  ~Printer() {}

private:
  std::mutex mutex_;
  std::function<void(const char*)> printer_;
};
