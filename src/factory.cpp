//
// Created by kamil on 18/12/2023.
//

#include "factory.hpp"
#include <algorithm>
#include <stdexcept>
#include <map>

enum class ElementType {
    RAMP, WORKER, STOREHOUSE, LINK
};


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
        os << std::endl<< "LOADING_RAMP id=" << r.get_id();
        os << " delivery-interval=" << r.get_delivery_interval();
    };
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(),ramp);

    // saving Worker
    os << std::endl << std::endl << " == WORKER ==" << std::endl;
    auto worker = [&os] (const Worker& w){
        os << std::endl<< "WORKER id=" << w.get_id();
        os << " processing-time=" << w.get_processing_duration();
        std::string s;
        if(w.get_queue()->get_queue_type() == PackageQueueType::FIFO) s = "FIFO";
        else s = "LIFO";
        os << " queue-type=" << s;
    };
    std::for_each(factory.worker_cbegin(), factory.worker_cend(),worker);

    // saving storehouse
    os << std::endl << std::endl << " == STOREHOUSE ==" << std::endl;
    auto storehouse = [&os] (const Storehouse& s){
        os << std::endl << "STOREHOUSE id=" << s.get_id();
    };
    std::for_each(factory.storehouse_cbegin(), factory.storehouse_cend(), storehouse);



    // saving links
    os <<std::endl << std::endl << " == LINKS ==" << std::endl;
    // links from ramp to worker
    auto ramp_to = [&os] (const Ramp& r){
        for (auto& receiver: r.receiver_preferences_.get_preferences())
            os << std::endl << "LINK src=ramp-" << r.get_id() << " dest=worker-" << receiver.first->get_id();
    };
    std::for_each(factory.ramp_cbegin(), factory.ramp_cend(), ramp_to);
    // links from worker
    auto worker_to = [&os](const Worker& w){
        for (auto& receiver: w.receiver_preferences_.get_preferences()){
            if (receiver.first->get_receiver_type() == ReceiverType::WORKER){
                os << std::endl << "LINK src=worker-" << w.get_id() << " dest=worker-" << receiver.first->get_id();
            } else if (receiver.first->get_receiver_type() == ReceiverType::STOREHOUSE){
                os << std::endl << "LINK src=worker-" << w.get_id() << " dest=store-" << receiver.first->get_id();
            }
        }
    };
    std::for_each(factory.worker_cbegin(), factory.worker_cend(), worker_to);
}


typedef struct {
    ElementType element_type;
    std::map<std::string, std::string> parameters;
} ParsedLineData;

ParsedLineData parse_line(std::string line) {
    ParsedLineData obj;

    std::vector<std::string> tokens;
    std::string token;

    std::istringstream token_stream(line);
    char delimiter = ' ';

    while (std::getline(token_stream, token, delimiter)) {
        tokens.push_back(token);
    }

    if (tokens[0] == "LOADING_RAMP") obj.element_type = ElementType::RAMP;
    if (tokens[0] == "WORKER") obj.element_type = ElementType::WORKER;
    if (tokens[0] == "STOREHOUSE") obj.element_type = ElementType::STOREHOUSE;
    if (tokens[0] == "LINK") obj.element_type = ElementType::LINK;

    char equal = '=';
    std::string t;
    for (auto i = tokens.begin() + 1; i != tokens.end(); i++) {
        std::istringstream tok_s(*i);
        std::vector<std::string> v;
        while (std::getline(tok_s, t, equal)) {
            v.push_back(t);
        }
        obj.parameters.insert(std::make_pair(v[0], v[1]));
    }

    return obj;
}


Factory load_factory_structure(std::istream& is) {
    Factory factory;

    std::string line;
    while(std::getline(is, line)) {
        if (!line.empty() && line[0] != ';') {
            auto par = parse_line(line);

            if (par.element_type == ElementType::RAMP) {
                factory.add_ramp(Ramp(std::stoi(par.parameters["id"]), std::stoi(par.parameters["delivery-interval"])));
            }

            else if (par.element_type == ElementType::WORKER) {
                if (par.parameters["queue-type"] == "FIFO")  {
                    factory.add_worker(Worker(std::stoi(par.parameters["id"]), std::stoi(par.parameters["processing-time"]), std::make_unique<PackageQueue>(PackageQueueType::FIFO)));
                }

                else if (par.parameters["queue-type"] == "LIFO")  {
                    factory.add_worker(Worker(std::stoi(par.parameters["id"]), std::stoi(par.parameters["processing-time"]), std::make_unique<PackageQueue>(PackageQueueType::LIFO)));
                }
            }

            else if (par.element_type == ElementType::STOREHOUSE) {
                factory.add_storehouse(Storehouse(std::stoi(par.parameters["id"])));
            }

            else if (par.element_type == ElementType::LINK) {
                std::vector<std::string> src;
                std::vector<std::string> dest;
                std::string token;
                std::istringstream token_src(par.parameters["src"]);
                while (std::getline(token_src, token, '-')) {
                    src.push_back(token);
                }

                std::istringstream token_dest(par.parameters["dest"]);
                while (std::getline(token_dest, token, '-')) {
                    dest.push_back(token);
                }


                if(src[0] == "worker" && dest[0] == "store"){
                    auto obj_src = factory.find_worker_by_id(std::stoi(src[1]));
                    auto obj_dest = factory.find_storehouse_by_id(std::stoi(dest[1]));
                    obj_src->receiver_preferences_.add_receiver(&*obj_dest);
                }
                else if(src[0] == "ramp" && dest[0] == "worker"){
                    auto obj_src = factory.find_ramp_by_id(std::stoi(src[1]));
                    auto obj_dest = factory.find_worker_by_id(std::stoi(dest[1]));
                    obj_src->receiver_preferences_.add_receiver(&*obj_dest);
                }
                else if(src[0] == "worker" && dest[0] == "worker"){
                    auto obj_src = factory.find_worker_by_id(std::stoi(src[1]));
                    auto obj_dest = factory.find_worker_by_id(std::stoi(dest[1]));
                    obj_src->receiver_preferences_.add_receiver(&*obj_dest);
                }
                else if(src[0] == "ramp" && dest[0] == "store"){
                    auto obj_src = factory.find_ramp_by_id(std::stoi(src[1]));
                    auto obj_dest = factory.find_storehouse_by_id(std::stoi(dest[1]));
                    obj_src->receiver_preferences_.add_receiver(&*obj_dest);
                }

                }
            }
        }

    return factory;
}


void generate_structure_report(const Factory& f, std::ostream& os){

}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t){

}

