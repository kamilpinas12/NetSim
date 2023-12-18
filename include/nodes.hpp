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


enum class ReceiverType {
    WORKER, STOREHOUSE
};


class IPackageReceiver
{
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ReceiverType get_receiver_type() const = 0;

    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;

    virtual ~IPackageReceiver() = default;
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




class PackageSender
{
public:
    ReceiverPreferences receiver_preferences_;

    PackageSender() = default;
    PackageSender(PackageSender&& pack_sender) = default;

    std::optional<Package>& get_sending_buffer() { return bufor_; };
    void send_package();

protected:
    void push_package(Package&& package) { bufor_.emplace(package.get_id()); };

private:

    std::optional<Package> bufor_ = std::nullopt;
};




class Ramp: public PackageSender
{
public:
    Ramp(ElementID id, TimeOffset di) : PackageSender(), id_(id), di_(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval() { return di_; };
    ElementID get_id() const { return id_; };

private:
    ElementID id_;
    TimeOffset di_;
    Time last_delivery_time_t_;
};




class Worker: public PackageSender, public IPackageReceiver
{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q): PackageSender(), id_(id), pd_(pd), q_(std::move(q)) {}
    void do_work(Time t);

    TimeOffset get_processing_duration() const { return pd_; };
    Time get_package_processing_start_time() const { return start_t_; };

    void receive_package(Package&& p) override {q_->push(std::move(p));}

    ElementID get_id() const override { return id_; }

    IPackageQueue::const_iterator cbegin() const override { return q_ -> cbegin(); }
    IPackageQueue::const_iterator cend() const override { return q_ -> cend(); }
    IPackageQueue::const_iterator begin() const override { return q_ -> cbegin(); }
    IPackageQueue::const_iterator end() const override { return q_ -> cend(); }


    ReceiverType get_receiver_type() const override {return ReceiverType::WORKER;}

private:
    ElementID id_;
    TimeOffset pd_;
    Time start_t_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> bufor_ = std::nullopt;
};


class Storehouse: public IPackageReceiver
{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id) {d_ = std::move(d);};

    IPackageStockpile::const_iterator begin() const override {return d_->begin();}
    IPackageStockpile::const_iterator cbegin() const override {return d_->cbegin();}
    IPackageStockpile::const_iterator end() const override {return d_->end();}
    IPackageStockpile::const_iterator cend() const override {return d_->cend();}

    void receive_package(Package&& p) override {d_->push(std::move(p));}
    ElementID get_id() const override {return id_;}
    ReceiverType get_receiver_type() const override {return ReceiverType::STOREHOUSE;}

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};


#endif //LAB_NETSIM_NODES_HPP
