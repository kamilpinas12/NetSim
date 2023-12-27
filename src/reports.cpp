//
// Created by kamil on 27/12/2023.
//

#include<iostream>


#include"reports.hpp"


//funkcja pomocnicza do generate_structure_report
void print_receivers(std::ostream& os, ReceiverPreferences preferences){
    // sortowanie według typu (storehouse < worker) oraz
    // według id (wstawienie id do std::set automatycznie sortuje)

    std::set<ElementID> workers;
    std::set<ElementID> stores;

    for(const auto& pair : preferences){
        if(pair.first->get_receiver_type() == ReceiverType::WORKER) workers.insert(pair.first->get_id());
        else stores.insert(pair.first->get_id());
    }

    os << "  Receivers:" << std::endl;
    for(ElementID worker_id : workers){
        os << "    worker #" << worker_id << "\n";
    }
    for(ElementID store_id : stores){
        os << "    storehouse #" << store_id << "\n";
    }
    os << std::endl;
}


void generate_structure_report(const Factory& f, std::ostream& os){
    // sortowanie węzłów według id (wstawienie do mapy automatycznie sortuje według klucza)
    // możliwe że to sortowanie nie just tutaj potrzebne bo jak dodaje się węzły do fabryki w odpowiedniej
    // kolejności to będzie działać (bez tego sortowania testy przechodzą)
    std::map<ElementID, const Worker*> workers;
    std::map<ElementID, const Ramp*> ramps;
    std::map<ElementID, const Storehouse*> stores;

    std::for_each(f.ramp_cbegin(), f.ramp_cend(),[&ramps](const Ramp& ramp) {
        ramps.insert(std::make_pair(ramp.get_id(),  &ramp));
    });
    std::for_each(f.worker_cbegin(), f.worker_cend(),[&workers](const Worker& worker) {
        workers.insert(std::make_pair(worker.get_id(),  &worker));
    });
    std::for_each(f.storehouse_cbegin(), f.storehouse_cend(),[&stores](const Storehouse& store) {
        stores.insert(std::make_pair(store.get_id(),  &store));
    });


    os << std::endl << "== LOADING RAMPS ==" << "\n\n";
    for(const auto pair : ramps){
        os << "LOADING RAMP #" << pair.first << std::endl;
        os << "  Delivery interval: " << pair.second->get_delivery_interval() << std::endl;
        print_receivers(os, pair.second->receiver_preferences_);
    }

    os << std::endl << "== WORKERS ==" << "\n\n";
    for(const auto pair : workers){
        os << "WORKER #" << pair.first << std::endl;
        os << "  Processing time: " << pair.second->get_processing_duration() << std::endl;
        std::string type;
        pair.second->get_queue()->get_queue_type()== PackageQueueType::FIFO ? type = "FIFO" : type = "LIFO";
        os << "  Queue type: " << type << std::endl;
        print_receivers(os, pair.second->receiver_preferences_);
    }

    os << std::endl << "== STOREHOUSES ==" << "\n\n";
    for(const auto pair : stores){
        os << "STOREHOUSE #" << pair.first << "\n\n";
    }
}







//funkcja pomocnicza do generate_simulation_turn_report
void print_queue (std::ostream& os, IPackageStockpile* q){
    bool is_first = true;
    for_each(q->cbegin(), q->cend(),
    [&os, &is_first](const auto &package) {
        if(is_first) {os << " #" << package.get_id(); is_first=false;}
        else os << ", #" << package.get_id();
    });
    os << std::endl;
}


void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t){
    //sortowanie węzłów według id (wstawienie do mapy automatycznie sortuje według klucza)
    // możliwe że to sortowanie nie just tutaj potrzebne bo jak dodaje się węzły do fabryki w odpowiedniej
    // kolejności to będzie działać (bez tego sortowania testy przechodzą)

    std::map<ElementID, const Worker*> workers;
    std::map<ElementID, const Storehouse*> stores;

    std::for_each(f.worker_cbegin(), f.worker_cend(),[&workers](const Worker& worker) {
        workers.insert(std::make_pair(worker.get_id(),  &worker));
    });
    std::for_each(f.storehouse_cbegin(), f.storehouse_cend(),[&stores](const Storehouse& store) {
        stores.insert(std::make_pair(store.get_id(),  &store));
    });

    os << "=== [ Turn: "<< t << " ] ===" << "\n\n";

    os << "== WORKERS ==";
    for(const auto& worker : workers) {
        os << "\n\nWORKER #" << worker.first << std::endl;
        if (worker.second->get_processing_buffer().has_value()){
            os << "  PBuffer: #" << worker.second->get_processing_buffer().value().get_id() <<
            " (pt = " << t - worker.second->get_package_processing_start_time() + 1 << ")\n";
        }
        else os << "  PBuffer: (empty)\n";

        os << "  Queue:";
        if(!worker.second->get_queue()->empty()) {
            print_queue(os, worker.second->get_queue());
        }
         else os << " (empty)\n";

        os << "  SBuffer: ";
        if(worker.second->get_sending_buffer().has_value()){
           os << "#" << worker.second->get_sending_buffer().value().get_id() << "";
        }
        else os << "(empty)";
    }


    os << "\n\n\n== STOREHOUSES ==\n\n";
    for(const auto& store : stores){
        os << "STOREHOUSE #" << store.first << std::endl;
        os << "  Stock:";
        if(!store.second->get_queue()->empty()){
            print_queue(os, store.second->get_queue());
        }
        else{os << " (empty)\n";}
    }
    os << "\n";
}
