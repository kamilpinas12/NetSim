//
// Created by kamil on 07/12/2023.
//

#ifndef LAB_NETSIM_NODES_HPP
#define LAB_NETSIM_NODES_HPP

#include<memory>
#include<map>
#include<optional>

#include"types.hpp"
#include"storage_types.hpp"


// @TODO zaimplementować metody tutaj lub w pliku nodes.cpp

// nie jestem pewien relacji między klasami



//klasa wirtualna
class IPackageReceiver
{
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() = 0;
};



class ReceiverPreferences
{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg);
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver(void);
    preferences_t& get_preferences(void);

private:
    preferences_t preferences_;
};




class PackageSender
{
public:
    PackageSender(PackageSender&&);
    void send_package(void);
    std::optional<Package>& get_sending_buffer(void);

protected:
    void push_package(Package&&);

private:
    ReceiverPreferences receiver_preferences_;
};





class Ramp: public PackageSender
{
public:
    Ramp(ElementID id, TimeOffset di);
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval(void);
    ElementID get_id(void);
};




class Worker: public PackageSender, public IPackageQueue, public IPackageReceiver
{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
    void do_work(Time t);
    TimeOffset get_processing_duration(void);
    Time get_package_processing_start_time(void);
};


// Dziedziczymy tylko po IPackageReciver a do klasy IPackageStockpile tworzymy inteligentny wskaźnik

class Storehouse: public IPackageReceiver
{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) : id_(id) {d_ = std::move(d);};

    IPackageStockpile::const_iterator begin() const {return d_->begin();}
    IPackageStockpile::const_iterator cbegin() const {return d_->cbegin();}
    IPackageStockpile::const_iterator end() const {return d_->end();}
    IPackageStockpile::const_iterator cend() const {return d_->cend();}

    void receive_package(Package&& p) override {d_->push(std::move(p));}
    ElementID get_id() override {return id_;}

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};


#endif //LAB_NETSIM_NODES_HPP
