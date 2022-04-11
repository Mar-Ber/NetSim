// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)

#include <factory.hpp>

bool has_reachable_storehouse(const PackageSender* sender, std::map<const PackageSender*, NodeColor>& node_colors) {

    if (node_colors[sender] == NodeColor::VERIFIED) { return true; };
    node_colors[sender] = NodeColor::VISITED;

    if (sender->receiver_preferences_.get_preferences().empty()) {
        throw std::logic_error("Nadawca nie posiada zadnego polaczenia wyjsciowego.");
    }

    bool has_receiver_other_than_self = false;
    for(auto receiver_ = sender->receiver_preferences_.get_preferences().cbegin(); receiver_ != sender->receiver_preferences_.get_preferences().cend(); receiver_++) {
        if (receiver_->first->get_receiver_type() == ReceiverType::STOREHOUSE) { has_receiver_other_than_self = true; }
        if (receiver_->first->get_receiver_type() == ReceiverType::WORKER) {
            IPackageReceiver *receiver_ptr = receiver_->first;
            auto worker_ptr = dynamic_cast<Worker *>(receiver_ptr);
            auto sendrecv_ptr = dynamic_cast<PackageSender *>(worker_ptr);

            if (sendrecv_ptr == sender) continue;
            has_receiver_other_than_self = true;

            if (node_colors[sendrecv_ptr] != NodeColor::VISITED) {
                has_reachable_storehouse(sendrecv_ptr, node_colors);
            };
        }
    }

    node_colors[sender] = NodeColor::VERIFIED;
    if(has_receiver_other_than_self){return true;}
    else {throw std::logic_error("Magazyn jest nieosiagalny.");}
}

void Factory::remove_worker(ElementID id) {
    remove_receiver(workers, id);
    workers.remove_by_id(id);
}
void Factory::remove_storehouse(ElementID id) {
    remove_receiver(storehouses, id);
    storehouses.remove_by_id(id);
}

void Factory::do_deliveries(Time t) {
    //std::for_each(ramp_cbegin(), ramp_cend(), [t](Ramp &ramp){ramp.deliver_goods(t);});
    for(auto ramp_ = ramps.begin(); ramp_ != ramps.end(); ramp_++){
        ramp_->deliver_goods(t);
    }
}

void Factory::do_package_passing(void) {
    //std::for_each(ramp_cbegin(), ramp_cend(), [](Ramp &ramp){ramp.send_package();});
    //std::for_each(worker_cbegin(), worker_cend(), [](Worker &worker){worker.send_package();});
    for(auto ramp_ = ramps.begin(); ramp_ != ramps.end(); ramp_++){
        ramp_->send_package();
    }
    for(auto worker_ = workers.begin(); worker_ != workers.end(); worker_++){
        worker_->send_package();
    }

}

void Factory::do_work(Time t) {
    //std::for_each(worker_cbegin(), worker_cend(), [t](Worker &worker){worker.do_work(t);});
    for(auto worker_ = workers.begin(); worker_ != workers.end(); worker_++){
        worker_->do_work(t);
    }
}

template<typename Node>
void Factory::remove_receiver(NodeCollection<Node> &collection, ElementID id) {
    auto receiver = collection.find_by_id(id);
    if(receiver!= collection.end()) {
        if (receiver->get_receiver_type() == ReceiverType::WORKER) {
            std::for_each(ramps.begin(), ramps.end(), [receiver](Ramp &ramp) {
                ramp.receiver_preferences_.remove_receiver(&*receiver);
            });
        } else {
            std::for_each(workers.begin(), workers.end(), [receiver](Worker &worker) {
                worker.receiver_preferences_.remove_receiver(&*receiver);
            });
        }
    }
}

bool Factory::is_consistent(void) const {
    std::map<const PackageSender*, NodeColor> node_colors;
    for(auto ramp_ = ramp_cbegin(); ramp_ != ramp_cend(); ramp_++){
        node_colors[&*ramp_] = NodeColor::UNVISITED;
    }

    for(auto worker_ = worker_cbegin(); worker_ != worker_cend(); worker_++){
        node_colors[&*worker_] = NodeColor::UNVISITED;
    }

    for(auto ramp_ = ramp_cbegin(); ramp_ != ramp_cend(); ramp_++){
        try {
            has_reachable_storehouse(&*ramp_, node_colors);
            return true;
        } catch (std::logic_error& e) {
            return false;
        }
    }
    return false;
}
