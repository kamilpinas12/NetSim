//
// Created by kamil on 07/12/2023.
//

#ifndef LAB_NETSIM_NODES_HPP
#define LAB_NETSIM_NODES_HPP

#include<memory>
#include<map>
#include<optional>


#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include "package.hpp"



// @TODO zaimplementować metody tutaj lub w pliku nodes.cpp

//@TODO: (Maria) Klasa enum


// (Kamil) w tym pliku zmieniłem kilka nazw zmiennych bo t_ troche mało mówi,
// wydaje mi się że można również usunąć bufor w klasie Ramp
// override w (128) nie wiem po co był skoro klasa nie nadpisuje tej metody



enum class ReceiverType {
    WORKER, STOREHOUSE
};


class IPackageReceiver
{
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;

    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
};


class ReceiverPreferences
{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) {probability_generator_ = std::move(pg);}

    const_iterator begin() const {return preferences_.begin();}
    const_iterator end() const {return preferences_.end();}
    const_iterator cbegin() const {return preferences_.cbegin();}
    const_iterator cend() const {return preferences_.cend();}

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    preferences_t& get_preferences() {return preferences_;};

private:
    preferences_t preferences_;
    ProbabilityGenerator probability_generator_;
};

//@TODO: (Maria) Tu by się przydał bufor, tak było napisane w konspekcie. Trzeba też uzupełnić
class PackageSender
{
public:
    ReceiverPreferences receiver_preferences_;

    PackageSender() = default;
    PackageSender(PackageSender&& pack_sender) = default;
    void send_package(void){}
    std::optional<Package>& get_sending_buffer(void) { return bufor_; };

protected:
    void push_package(Package&& package) { bufor_.emplace(package.get_id()); };

private:

    std::optional<Package> bufor_ = std::nullopt;
};




//@TODO: (Maria) Wiem w czym był problem - teraz powinno działać
class Ramp: public PackageSender
{
public:
    Ramp(ElementID id, TimeOffset di) : PackageSender(), id_(id), di_(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval(void) { return di_; };
    ElementID get_id(void) { return id_; };

private:
    ElementID id_;
    TimeOffset di_;
    Time last_delivery_time_t_;
};


//@TODO: (Maria) Udało mi się to naprawić.
class Worker: public PackageSender, public IPackageReceiver
{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q): PackageSender(), id_(id), pd_(pd), q_(std::move(q)) {}
    void do_work(Time t);

    TimeOffset get_processing_duration(void) const { return pd_; };
    Time get_package_processing_start_time(void) const { return start_t_; };

    void receive_package(Package&& p) override {q_->push(std::move(p));}

    ElementID get_id() const override { return id_; }

    IPackageQueue::const_iterator cbegin() const override { return q_ -> cbegin(); }
    IPackageQueue::const_iterator cend() const override { return q_ -> cend(); }
    IPackageQueue::const_iterator begin() const override { return q_ -> cbegin(); }
    IPackageQueue::const_iterator end() const override { return q_ -> cend(); }


    ReceiverType get_receiver_type() { return ReceiverType::WORKER; }

private:
    ElementID id_;
    TimeOffset  pd_;
    Time start_t_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> bufor_ = std::nullopt;
};


class Storehouse: public IPackageReceiver
{
public:
    //TODO: Ogarnąć to z wersją Marysi
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id) {d_ = std::move(d);};

    IPackageStockpile::const_iterator begin() const override {return d_->begin();}
    IPackageStockpile::const_iterator cbegin() const override {return d_->cbegin();}
    IPackageStockpile::const_iterator end() const override {return d_->end();}
    IPackageStockpile::const_iterator cend() const override {return d_->cend();}

    void receive_package(Package&& p) override {d_->push(std::move(p));}
    ElementID get_id() const override {return id_;}

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};


#endif //LAB_NETSIM_NODES_HPP
