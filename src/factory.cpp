//
// Created by kamil on 18/12/2023.
//

#include "factory.hpp"



void Factory::do_work(Time t) {
    for (auto& worker : worker_){
        worker.do_work(t);
    }
}


void Factory::do_delivery(Time t) {
    for (auto& ramp : ramp_){
        ramp.deliver_goods(t);
    }
}


void Factory::do_package_passing() {
    for (auto& worker : worker_){
        worker.send_package();
    }

    for (auto& ramp : ramp_){
        ramp.send_package();
    }
}


template <class Node>
void Factory::remove_receiver(NodeCollection<Node>& collection, ElementID id)
{
    auto it = &(*collection.find_by_id(id));
    for(auto& worker : worker_){
        worker.receiver_preferences_.remove_receiver(it);
    }
    for(auto& ramp : ramp_){
        ramp.receiver_preferences_.remove_receiver(it);
    }
}

