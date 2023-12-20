//
// Created by Filip Połatyński on 18/12/2023.
//

#ifndef LAB_NETSIM_FACTORY_HPP
#define LAB_NETSIM_FACTORY_HPP

#include <istream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>


#include "types.hpp"
#include "storage_types.hpp"
#include "nodes.hpp"
#include "package.hpp"


template <class Node>
class NodeCollection{
public:
    using container_t = typename std::list<Node>;
    using const_iterator = typename container_t::const_iterator;
    using iterator = typename container_t::iterator;

    const_iterator begin() const {return collection_.begin();}
    const_iterator end() const {return collection_.end();}
    const_iterator cbegin() const {return collection_.cbegin();}
    const_iterator cend() const {return collection_.cend();}
    iterator begin() {return collection_.begin();}
    iterator end() {return collection_.end();}


    void add(Node&& node) {collection_.emplace_back(std::move(node));}

    //TODO: metody remove_by_id i find_by_id (Kamil)
    void remove_by_id(ElementID id){
        auto it = find_by_id(id);
        if(it != collection_.end()) collection_.erase(it);
    }

    iterator find_by_id(ElementID id){
        return std::find_if(collection_.begin(), collection_.end(),
        [id](const auto& elem){ return (elem.get_id() == id);});}


    const_iterator find_by_id (ElementID id) const {
        return std::find_if(collection_.cbegin(), collection_.cend(),
        [id](const auto& elem){ return (elem.get_id() == id);});}


private:
    container_t collection_;
};




class Factory{
public:
    // TODO: Zrealizować funkcjonalności Rampy, Workera i Storehouse (Kamil)
    void add_ramp(Ramp&& ramp){ramp_.add(std::move(ramp));}
    void remove_ramp(ElementID id){ramp_.remove_by_id(id);}
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id){ return ramp_.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return ramp_.find_by_id(id);}
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const{return ramp_.cbegin();}
    NodeCollection<Ramp>::const_iterator ramp_cend() const{return ramp_.cend();}

    void add_storehouse(Storehouse&& storehouse){storehouse_.add(std::move(storehouse));}
    void remove_storehouse(ElementID id){remove_receiver(storehouse_, id); storehouse_.remove_by_id(id);}
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id){ return storehouse_.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return storehouse_.find_by_id(id);}
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const{return storehouse_.cbegin();}
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const{return storehouse_.cend();}

    void add_worker(Worker&& worker){worker_.add(std::move(worker));}
    void remove_worker(ElementID id){remove_receiver(worker_, id); worker_.remove_by_id(id);}
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id){ return worker_.find_by_id(id);}
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return worker_.find_by_id(id);}
    NodeCollection<Worker>::const_iterator worker_cbegin() const{return worker_.cbegin();}
    NodeCollection<Worker>::const_iterator worker_cend() const{return worker_.cend();}



    //TODO: Sprawdzenie spójności (Filip)
    // ZMIEŃ !!! return 1 dodane tylko aby nie wywalało błędu
    bool is_consistent() const{return 0;};


    //TODO: Metody wykonujące przekazania (Kamil)
    void do_delivery(Time t);
    void do_package_passing();
    void do_work(Time t);

private:

    // wiem że tak długie funkcje powinny być w cpp ale jak jest szablon to nie działa definicja
    // w cpp dlatego dałem tutaj jeżeli ktoś wie jak to się powinno robić to zmieńcie
    template <class Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id)
    {
        auto it = &(*collection.find_by_id(id));
        std::for_each(worker_.begin(), worker_.end(),
                      [it](auto &elem){elem.receiver_preferences_.remove_receiver(it);});

        std::for_each(ramp_.begin(), ramp_.end(),
                      [it](auto &elem){elem.receiver_preferences_.remove_receiver(it);});
    }

    NodeCollection<Storehouse>  storehouse_;
    NodeCollection<Worker> worker_;
    NodeCollection<Ramp> ramp_;
};


Factory load_factory_structure(std::istream& is);
void save_factory_structure(Factory& factory, std::ostream& os);




#endif //LAB_NETSIM_FACTORY_HPP