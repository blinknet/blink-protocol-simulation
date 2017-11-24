#pragma once

#include <map>
#include <queue>

#include "event.hpp"

class Node {
private:
    double longitude, latitude;
    bool corrupt;
    int index;
//    std::map<int, double> version_receive_time;
//    std::queue<Event> event_queue;

    void init(int index);

public:
    Node();
    Node(int index);

    void reset(const double &corruption_chance);

    bool is_corrupt() const;
    void set_corrupt(const double &chance);

    double sphere_distance(const Node &other) const;
    double broadcast_duration(const Node &other) const;

//    int rand_target();
//
//    void apply_events_from_queue();
//
//    void send_message(int version, double timestamp, int target, int type);
//    void broadcast(int version, double timestamp, int target);
//    void request_sync(int version, double timestamp, int target);
//    void receive_successful_sync(const Event &event);
//    void receive_failed_sync(const Event &event);
//    void receive_sync_query(const Event &event);
//    void set_version(const Event &event, bool try_apply_queue=true);
//
//    bool received_version(int version);
//    double get_version_receive_time(int version);
};
