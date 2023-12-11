//
// Created by kamil on 07/12/2023.
//
#include"nodes.hpp"

void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    std::size_t size = preferences_.size();
    if (size == 0){
        preferences_[r] = 1.0;
    } else {
        for (auto& obj : preferences_){
            obj.second = 1.0 / (double(size) + 1);
        }
        preferences_[r] = 1.0 / (double(size) + 1);
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
    preferences_.erase(r);
    std::size_t size = preferences_.size();
    if (size != 1){
        for (auto& obj : preferences_){
            obj.second = 1.0 / (double(size) - 1);
        }
    }
}


