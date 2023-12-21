//
// Created by kamil on 18/12/2023.
//

#include "factory.hpp"
#include <algorithm>
#include <stdexcept>
#include <map>

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
    if ((sender->receiver_preferences_).preferences_.empty()){
        throw std::logic_error("Nadawca nie ma zddefiniowanych odbiorców");
    }
    bool has_reciver = false;
    for (auto& [receiver, p]: sender->receiver_preferences_.preferences_){
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



void add_links(std::ostream& os, ReceiverPreferences preferences){

}


void save_factory_structure(Factory& factory, std::ostream& os){
    // ramp label "== LOADING RAMPS ==";
    // worker label  "== WORKERS ==";
    // storehouse label "== STOREHOUSES ==";
    os << std::endl << "== LOADING RAMPS ==" << std::endl << std::endl;
    // zamiana na mapę żeby posortować według id
    std::map<ElementID, const Ramp*> ramps;
    for(auto ramp = factory.ramp_cbegin(); ramp != factory.ramp_cend(); ramp ++){
        ramps.insert(std::make_pair(ramp->get_id(), &*ramp));
    }
    for(auto ramp : ramps){
        os << "LOADING RAMP #" << ramp.first << std::endl;
        os << "  Delivery interval: " << ramp.second->get_delivery_interval() << std::endl;
        os << "  Receivers:" << std::endl;
        add_links(os, ramp.second->receiver_preferences_);
        os << "\n";
    }
    os << "\n\n";

    os << std::endl << "== WORKERS ==" << std::endl << std::endl;
    std::map<ElementID, const Worker*> workers;
    for(auto worker = factory.worker_cbegin(); worker != factory.worker_cend(); worker++){
        workers.insert(std::make_pair(worker->get_id(), &*worker));
    }
    for(auto worker : workers){
        os << "WORKER #" << worker.first << std::endl;
        os << "  Processing time: " << worker.second->get_processing_duration() << std::endl;
//        os << "  Queue type: " << (worker.second->get_receiver_type() == PackageQueueType::FIFO) ? "FIFO" : "LIFO" << std::endl;
//        os << "  Receivers:" << std::endl;
//        add_links(os, ramp.second->receiver_preferences_);
//        os << "\n";
    }
    os << "\n\n";



}

