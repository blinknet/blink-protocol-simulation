//#pragma once
//
//#include <cstdlib>
//
//#include <vector>
//#include <queue>
//
//class Event {
//public:
//    int source, target;
//    int type;
//    int state_version;
//    double timestamp;
//
//    enum EventType {
//        BROADCAST,
//        SYNC_QUERY,
//        SYNC_SUCCESSFUL,
//        SYNC_FAILED
//    };
//
//    Event();
//    Event(int source, int target, int state_version, double timestamp, int type=EventType::BROADCAST);
//
//    bool operator < (const Event &other) const;
//
//    void apply() const;
//};
//
//class EventQueue {
//private:
//    std::priority_queue<Event, std::vector<Event>> event_queue;
//public:
//    void push(const Event &event);
//
//    void apply_first();
//
//    bool empty();
//
//    size_t size();
//};
