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

// nie jestem pewien relacji między klasami

//@TODO: (Maria) Klasa enum

enum class ReceiverType {
    WORKER, STOREHOUSE
};

//klasa wirtualna

//@TODO (Maria) dodałam odpowiednie pola i destruktor
class IPackageReceiver
{
public:
    virtual void receive_package(Package&& p) = 0;

    virtual ElementID get_id(void) = 0;

    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;

    virtual ReceiverType get_receiver_type() = 0;

    virtual ~IPackageReceiver() = default;
};


//@TODO (Maria) Dodałam iteratory i uzupełniłam metody
class ReceiverPreferences
{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(std::move(pg)) {}

    const_iterator cbegin() const { return preferences_.cbegin(); }
    const_iterator cend() const { return preferences_.cend(); }
    const_iterator begin() const { return preferences_.cbegin(); }
    const_iterator end() const { return preferences_.cend(); }

    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver(void);
    preferences_t& get_preferences(void) { return  preferences_; };

private:
    preferences_t preferences_;
    ProbabilityGenerator pg_;
};



//@TODO: (Maria) Uzupełniłam metody i bufor.
class PackageSender {
public:
    PackageSender() = default;

    PackageSender(PackageSender&& pack_sender) = default;

    void send_package(void);

    std::optional<Package>& get_sending_buffer(void) { return bufor_; };

protected:
    void push_package(Package&& package) { bufor_.emplace(package.get_id()); };

private:
    ReceiverPreferences receiver_preferences_;
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
    Time t_;
    std::optional<Package> bufor_ = std::nullopt;
};



//@TODO: (Maria) Udało mi się to naprawić.
class Worker: public PackageSender, public IPackageQueue, public IPackageReceiver
{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q): PackageSender(), id_(id), pd_(pd), q_(std::move(q)) {}
    void do_work(Time t);

    TimeOffset get_processing_duration(void) { return pd_; };
    Time get_package_processing_start_time(void) { return t_; };

    void receive_package(Package&& p) override;

    ElementID get_id() override { return id_; }

    const_iterator cbegin() const override { return q_ -> cbegin(); }
    const_iterator cend() const override { return q_ -> cend(); }
    const_iterator begin() const override { return q_ -> cbegin(); }
    const_iterator end() const override { return q_ -> cend(); }

    ReceiverType get_receiver_type() override { return ReceiverType::WORKER; }

private:
    ElementID id_;
    TimeOffset  pd_;
    Time t_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Package> bufor_ = std::nullopt;
};


//@TODO: (Maria) Uzupełniłam klasę - iteratory i metody
class Storehouse: public IPackageStockpile, public IPackageReceiver
{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)) : id_(id), d_(std::move(d)) {}

    void receive_package(Package&& p) override;

    ElementID get_id() override { return id_; }

    const_iterator cbegin() const override { return d_ -> cbegin(); }
    const_iterator cend() const override { return d_ -> cend(); }
    const_iterator begin() const override { return d_ -> cbegin(); }
    const_iterator end() const override { return d_ -> cend(); }

    ReceiverType get_receiver_type() override { return ReceiverType::STOREHOUSE; }

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};


#endif //LAB_NETSIM_NODES_HPP
