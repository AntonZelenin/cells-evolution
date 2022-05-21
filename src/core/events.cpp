#include "CellsEvo/core/events.h"

namespace cells_evo::event {
void Dispatcher::Subscribe(const DescriptorType &descriptor, SlotType &&slot) {
  observers_[descriptor].push_back(slot);
}

void Dispatcher::Post(const Event &event) const {
  auto type = event.Type();

  // Ignore events for which we do not have an observer (yet).
  if (observers_.find(type) == observers_.end())
    return;

  auto &&observers = observers_.at(type);

  for (auto &&observer : observers)
    observer(event);
}
}