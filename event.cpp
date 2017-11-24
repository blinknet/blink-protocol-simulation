//#include "event.hpp"
//
//
//#include "globals.hpp"
//
//
//Event::Event() = default;
//
//Event::Event(int source, int target, int state_version, double timestamp, int type) {
//    this->source = source;
//    this->target = target;
//    this->state_version = state_version;
//    this->timestamp = timestamp;
//    this->type = type;
//}
//
//bool Event::operator < (const Event &other) const {
//    return timestamp > other.timestamp;
//}
//
//void Event::apply() const {
//    if (this->type == Event::EventType::BROADCAST) {
//        nodes[this->target].set_version(*this);
//    } else if (this->type == Event::EventType::SYNC_QUERY) {
//        nodes[this->target].receive_sync_query(*this);
//    } else if (this->type == Event::EventType::SYNC_SUCCESSFUL) {
//        nodes[this->target].receive_successful_sync(*this);
//    } else if (this->type == Event::EventType::SYNC_FAILED) {
//        nodes[this->target].receive_failed_sync(*this);
//    }
//}
//
//
//void EventQueue::push(const Event &event) {
//    this->event_queue.push(event);
//}
//
//void EventQueue::apply_first() {
//    const auto event = this->event_queue.top();
//    this->event_queue.pop();
//    event.apply();
//}
//
//bool EventQueue::empty() {
//    return this->event_queue.empty();
//}
//
//size_t EventQueue::size() {
//    return this->event_queue.size();
//}