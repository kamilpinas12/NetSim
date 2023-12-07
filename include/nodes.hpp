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

// nie jestem pewny dziedziczeń w między klasami ReceiverPreferences i PackageSender ale wysyłam tak żeby już na GitHubie
//  był jakiś szkielet :)

class Ramp
{
public:
    Ramp(ElementID id, TimeOffset di);
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval(void);
    ElementID get_id(void);
};


class Worker: public IPackageQueue
{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
    void do_work(Time t);
    TimeOffset get_processing_duration(void);
    Time get_package_processing_start_time(void);
};


class Storehouse: public IPackageStockpile
{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d);
};


//klasa wirtualna
class IPackageReceiver: public Worker, public Storehouse
{
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id(void) = 0;
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


class PackageSender: public Ramp, public Worker
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






#endif //LAB_NETSIM_NODES_HPP
