#pragma once

class Printer {
public:
  enum class Destination {
    Console,
    Popup,
    Stdout,
  };

  static Printer& Get();

  void Print(const char* text);
  void SetDestination(Destination destination);

private:
  ~Printer() {}

private:
  std::atomic<Destination> destination_ = Destination::Popup;
};
