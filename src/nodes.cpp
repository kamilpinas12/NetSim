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
    auto x = preferences_.find(r);
    if (x != preferences_.end()){
        preferences_.erase(x);
        std::size_t size = preferences_.size();
        if (size != 0){
            for (auto& obj : preferences_){
                obj.second = 1.0 / (double(size));
            }
        }
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    auto ran = probability_generator_();
    auto it = preferences_.begin();
    double i = it-> second;
    while (i < ran){
        if (it != preferences_.end()){
            std::advance(it, 1);
            i += it->second;
        }else{
            break;
        }
    }
    return it->first;
}



// (Kamil) funkcje do zweryfikowania bo nie wiem na 100% czy o to chodzi


void Worker::do_work(Time t) {
    // jeśli bufor jest pusty i jest coś w kolejce to przenieś package
    // z kolejki do bufora i zacznij przetwarzanie
    if (!bufor_.has_value() && !(q_->empty())){
        start_t_ = t;
        bufor_.emplace(std::move(q_->pop()));
    }
    // przetwarzanie zakończone, package przeniesiona do bufora PackageSender
    else if((t - start_t_) >= pd_ - 1 && bufor_.has_value()){
        push_package(std::move(bufor_.value()));
        bufor_.reset();
    }
}


void Ramp::deliver_goods(Time t) {
    if(t - last_delivery_time_t_ >= di_ || t == 1){
        Package p;
        push_package(std::move(p));
        last_delivery_time_t_ = t;
    }
}


void PackageSender::send_package(){
    if(bufor_.has_value()) {
        receiver_preferences_.choose_receiver()->receive_package(std::move(bufor_.value()));
        bufor_.reset();
    }
}
