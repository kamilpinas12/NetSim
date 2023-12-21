//
// Created by kamil on 18/12/2023.
//

#include "factory.hpp"
#include <algorithm>
#include <stdexcept>


void Factory::do_work(Time t) {
    std::for_each(worker_.begin(), worker_.end(),
                  [t](auto &elem){elem.do_work(t);});
}


void Factory::do_delivery(Time t) {
    std::for_each(ramp_.begin(), ramp_.end(),
                  [t](auto &elem){elem.deliver_goods(t);});
}


void Factory::do_package_passing() {
    std::for_each(worker_.begin(), worker_.end(),
                  [](auto &elem){elem.send_package();});

    std::for_each(ramp_.begin(), ramp_.end(),
                  [](auto &elem){elem.send_package();});
}


enum class NodeColor { UNVISITED, VISITED, VERIFIED };


bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors){
    if (node_colors[sender] == NodeColor::VISITED) {return true;}
    node_colors[sender]  = NodeColor::VISITED;
    if ((sender->receiver_preferences_).get_preferences().empty()){
        throw std::logic_error("Nadawca nie ma zddefiniowanych odbiorców");
    }
    bool has_reciver = false;
    for (auto& [receiver, p]: sender->receiver_preferences_.get_preferences()){
        if (receiver->get_receiver_type() == ReceiverType::STOREHOUSE){
            has_reciver = true;
        }else{
            IPackageReceiver* receiver_ptr = receiver;
            auto worker_ptr = dynamic_cast<Worker*>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender*>(worker_ptr);
            if (sendrecv_ptr == sender){
                continue;
            }
            has_reciver = true;
            if (node_colors[sendrecv_ptr] == NodeColor::UNVISITED){
                has_reachable_storehouse(sendrecv_ptr, node_colors);
            }
        }

    }
    node_colors[sender] = NodeColor::VERIFIED;
    if (has_reciver){
        return true;
    }else{
        throw std::logic_error("Nadawca wysyła paczkę tylko do siebie");
    }

}


bool Factory::is_consistent() const {
    // Inicjalizacja mapy wszystkich senderów na unvisited
    std::map<const PackageSender*, NodeColor> color;
    for (auto& ramp : ramp_){color[&ramp] = NodeColor::UNVISITED;}
    for (auto& worker : worker_){color[&worker] = NodeColor::UNVISITED;}

    // sprawdzenie za pomocą algorytmu DFS czy z każdej rampy da się dojść do magazynu
    try {
        for (auto &ramp: ramp_) {
            has_reachable_storehouse(&ramp, color);
        }
    }
    catch (const std::logic_error& e) {
        return false;
    }
    return true;
}

void save_factory_structure(Factory& factory, std::ostream& os){
    // saving ramps
    os << std::endl << " == LOADING RAMPS ==" << std::endl;
    auto ramp = [&os] (const Ramp& r){
        os << std::endl<< "LOADING_RAMP " << r.get_id();
        os << "delivery-interval=" << r.get_delivery_interval();
    };
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(),ramp);

    // saving Worker
    os << std::endl << " == WORKER ==" << std::endl;
    auto worker = [&os] (const Worker& w){
        os << std::endl<< "LOADING_RAMP id=" << w.get_id();
        os << "processing-time=" << w.get_processing_duration();
        os << "queue-type=" << w.get_queue();
    };
    std::for_each(factory.worker_cbegin(), factory.worker_cend(),worker);

    // saving storehouse
    os << std:: endl << " == STOREHOUSE ==" << std::endl;
    auto storehouse = [&os] (const Storehouse& s){
        os << std::endl << "STOREHOUSE id=" << s.get_id();
    };
    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), storehouse);

    // saving links
    os << " == LINKS ==" << std::endl;
    // links from ramp to worker
    auto ramp_worker = [&os] (const Ramp& r){
        for (auto& receiver : r.receiver_preferences_.get_preferences())
            os << std::endl <<"LINK src=ramp-" << r.get_id() << "dest=worker-" << receiver.first;
    };
    std::for_each(factory.ramp_cbegin(), factory.ramp_cbegin(), ramp_worker);



}