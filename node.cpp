#include "node.hpp"

#include "utils.hpp"
#include "globals.hpp"

Node::Node() {
    this->init(0);
}

Node::Node(int index) {
    this->init(index);
}

void Node::init(int index) {
    const auto city = random_city();
    this->longitude = deg_to_rad(city.second.first);
    this->latitude = deg_to_rad(city.second.second);
    this->set_corrupt(corruption_chance);
    this->index = index;
}

void Node::reset(const double &corruption_chance) {
    const auto city = random_city();
    this->longitude = deg_to_rad(city.second.first);
    this->latitude = deg_to_rad(city.second.second);
    this->set_corrupt(corruption_chance);
//    this->version_receive_time.clear();
//    while (!this->event_queue.empty()) {
//        this->event_queue.pop();
//    }
}

bool Node::is_corrupt() const {
    return this->corrupt;
}

void Node::set_corrupt(const double &chance) {
    this->corrupt = rand01() <= chance;
}

double Node::sphere_distance(const Node &other) const {
    return acos(sin(longitude) * sin(other.longitude) + cos(longitude) * cos(other.longitude) * cos(fabs(latitude - other.latitude)));
}

double Node::broadcast_duration(const Node &other) const {
    const double dist = this->sphere_distance(other);
    return (computing_time + latency * dist) * latency_distribution();
}

//int Node::rand_target() {
//    int target;
//    do {
//        target = rand_int(num_nodes);
//    } while(target == this->index);
//    return target;
//}
//
//void Node::apply_events_from_queue() {
//    while (!this->event_queue.empty()) {
//        const Event event = this->event_queue.front();
//        const int version = event.state_version;
//        if (version == 0 || this->version_receive_time.count(version - 1)) {
//            // we can apply this event! Remove it from the queue, then set this version
//            this->event_queue.pop();
//            this->set_version(event, false);
//        }
//    }
//}
//
//void Node::send_message(int version, double timestamp, int target, int type) {
//    time_line.push(
//            Event(
//                    this->index,
//                    target,
//                    version,
//                    timestamp + this->broadcast_duration(nodes[target]),
//                    type
//            )
//    );
//}
//
//void Node::broadcast(int version, double timestamp, int target) {
//    this->send_message(version, timestamp, target, Event::EventType::BROADCAST);
//}
//
//void Node::request_sync(int version, double timestamp, int target) {
//    this->send_message(version, timestamp, target, Event::EventType::SYNC_QUERY);
//}
//
//void Node::receive_successful_sync(const Event &event) {
//    // If we didn't yet receive this version from somewhere else, apply it
//    this->set_version(event);
//}
//
//void Node::receive_failed_sync(const Event &event) {
//    if (!this->version_receive_time.count(event.state_version)) {
//        // If we didn't receive this version from somewhere else, request another sync
//        this->request_sync(event.state_version, event.timestamp, rand_target());
//    }
//}
//
//void Node::receive_sync_query(const Event &event) {
//    int event_type;
//    if (this->version_receive_time.count(event.state_version)) {
//        // If we have this version, the sync is successful.
//        event_type = Event::EventType::SYNC_SUCCESSFUL;
//    } else {
//        // If we don't have this version, the sync attempt failed.
//        event_type = Event::EventType::SYNC_FAILED;;
//    }
//    this->send_message(event.state_version, event.timestamp, event.source, event_type);
//}
//
//void Node::set_version(const Event &event, bool try_apply_queue) {
//    const int version = event.state_version;
//    const double timestamp = event.timestamp;
//
//    if (this->version_receive_time.count(version)) {
//        return; // Already has it, and he broadcast-ed it as well
//    }
//    if (version == 0 || this->version_receive_time.count(version - 1)) {
//        this->version_receive_time[version] = timestamp;
//        if (!this->is_corrupt()) {
//            for (int i = 0; i < gossip_factor; ++ i) {
//                this->broadcast(version, timestamp, this->rand_target());
//            }
//        }
//    } else {
//        this->event_queue.push(event);
//        this->request_sync(version - 1, timestamp, this->rand_target());
//    }
//    if (try_apply_queue) {
//        this->apply_events_from_queue();
//    }
//}
//
//bool Node::received_version(int version) {
//    return this->version_receive_time.count(version) > 0;
//}
//
//double Node::get_version_receive_time(int version) {
//    return this->version_receive_time[version];
//}