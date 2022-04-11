// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)

#ifndef NETSIM_FACTORY_HPP
#define NETSIM_FACTORY_HPP

#include "nodes.hpp"
#include <algorithm>

enum class NodeColor{
    UNVISITED, VISITED, VERIFIED
};

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors);

template <typename Node>
class NodeCollection{
public:
    using container_t = typename std::list<Node>;
    using iterator = typename container_t::iterator;
    using const_iterator = typename container_t::const_iterator;

    void add(Node& node) {collection_.emplace_back(std::move(node));};

    void remove_by_id(ElementID id_){
        iterator it = find_by_id(id_);
        if (it != collection_.end()) {
            collection_.erase(it);
        }
    }

    NodeCollection<Node>::iterator find_by_id(ElementID id_){
/*        return std::find_if(collection_.begin(), collection_.end(),
                               [id_](auto elem){ return (elem.get_id() == id_);});*/
        for(auto elem = collection_.begin(); elem != collection_.end(); elem++){
            if(elem->get_id() == id_){return elem;}
        }
        return collection_.end();
    }

    NodeCollection<Node>::const_iterator find_by_id(ElementID id_) const{
/*        return std::find_if(collection_.cbegin(), collection_.cend(),
                               [id_](const auto elem){ return (elem.get_id == id_);}); */
        for(auto elem = collection_.cbegin(); elem != collection_.cend(); elem++){
            if(elem->get_id() == id_){return elem;}
        }
        return collection_.cend();
    }

    const_iterator begin() const {return collection_.cbegin();}
    const_iterator cbegin() const {return collection_.cbegin();}
    const_iterator end() const {return collection_.cend();}
    const_iterator cend() const {return collection_.cend();}
    iterator begin() {return collection_.begin();}
    iterator end() {return collection_.end();}

private:
    container_t collection_;
};

class Factory{
public:
    void add_ramp(Ramp&& ramp_) {ramps.add(ramp_);};
    void remove_ramp(ElementID id) {ramps.remove_by_id(id);};
    NodeCollection<Ramp>::iterator find_ramp_by_id(ElementID id) {return ramps.find_by_id(id);};
    NodeCollection<Ramp>::const_iterator find_ramp_by_id(ElementID id) const {return ramps.find_by_id(id);};
    NodeCollection<Ramp>::const_iterator ramp_cbegin() const {return ramps.cbegin();};
    NodeCollection<Ramp>::const_iterator ramp_cend() const {return ramps.cend();};

    void add_worker(Worker&& worker_) {workers.add(worker_);};
    void remove_worker(ElementID id);
    NodeCollection<Worker>::iterator find_worker_by_id(ElementID id) {return workers.find_by_id(id);};
    NodeCollection<Worker>::const_iterator find_worker_by_id(ElementID id) const {return workers.find_by_id(id);};
    NodeCollection<Worker>::const_iterator worker_cbegin() const {return workers.cbegin();};
    NodeCollection<Worker>::const_iterator worker_cend() const {return workers.cend();};

    void add_storehouse(Storehouse&& storehouse_) {storehouses.add(storehouse_);};
    void remove_storehouse(ElementID id);
    NodeCollection<Storehouse>::iterator find_storehouse_by_id(ElementID id) {return storehouses.find_by_id(id);};
    NodeCollection<Storehouse>::const_iterator find_storehouse_by_id(ElementID id) const {return storehouses.find_by_id(id);};
    NodeCollection<Storehouse>::const_iterator storehouse_cbegin() const {return storehouses.cbegin();};
    NodeCollection<Storehouse>::const_iterator storehouse_cend() const {return storehouses.cend();};

    bool is_consistent(void) const;
    void do_deliveries(Time t);
    void do_package_passing(void);
    void do_work(Time t);

private:
    template <typename Node>
    void remove_receiver(NodeCollection<Node>& collection, ElementID id);

    NodeCollection<Ramp> ramps;
    NodeCollection<Worker> workers;
    NodeCollection<Storehouse> storehouses;

};

#endif //NETSIM_FACTORY_HPP
