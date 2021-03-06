#ifndef CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_EVENTS_H_
#define CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_EVENTS_H_

#include <functional>
#include <map>
#include <vector>
#include <algorithm>

namespace cells_evo::event {

// todo this is a very lazy implementation, improve it

using DescriptorType = const char *;

class Event {
 public:
//  virtual ~Event() = 0;

  [[nodiscard]] virtual DescriptorType Type() const = 0;
};

class ToggleCellsDrawingEvent : public Event {
 public:
//  ToggleCellsDrawingEvent() = default;
//  ~ToggleCellsDrawingEvent() override = default;

  static constexpr DescriptorType descriptor_ = "ToggleCellsDrawingEvent";

  [[nodiscard]] DescriptorType Type() const override {
    return descriptor_;
  }
};

class ToggleSimulation : public Event {
 public:
  static constexpr DescriptorType descriptor_ = "ToggleSimulation";

  [[nodiscard]] DescriptorType Type() const override {
    return descriptor_;
  }
};

class ToggleDrawCellIndices : public Event {
 public:
  static constexpr DescriptorType descriptor_ = "ToggleDrawCellIndices";

  [[nodiscard]] DescriptorType Type() const override {
    return descriptor_;
  }
};

class ToggleDrawFoodIndices : public Event {
 public:
  static constexpr DescriptorType descriptor_ = "ToggleDrawFoodIndices";

  [[nodiscard]] DescriptorType Type() const override {
    return descriptor_;
  }
};

class GenerateHunterCell : public Event {
 public:
  static constexpr DescriptorType descriptor_ = "GenerateHunterCell";

  [[nodiscard]] DescriptorType Type() const override {
    return descriptor_;
  }
};

class GenerateNonHunterCell : public Event {
 public:
  static constexpr DescriptorType descriptor_ = "GenerateNonHunterCell";

  [[nodiscard]] DescriptorType Type() const override {
    return descriptor_;
  }
};

class Dispatcher {
 public:
  using SlotType = std::function<void(const Event &)>;

  void Subscribe(const DescriptorType &descriptor, SlotType &&slot);
  void Post(const Event &event) const;

 private:
  std::map<DescriptorType, std::vector<SlotType>> observers_;
};

//class Observer {
// public:
//  static void HandleEvent(const Event &e) {
//    if (e.Type() == ToggleCellsDrawingEvent::descriptor_) {
//      const auto &demo_event = dynamic_cast<const ToggleCellsDrawingEvent &>(e);
//    }
//  }
//};
}

#endif //CELLS_EVOLUTION_INCLUDE_CELLSEVO_CORE_EVENTS_H_
