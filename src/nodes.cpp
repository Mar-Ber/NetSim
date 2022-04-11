// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)

#include "nodes.hpp"
#include <iostream>

void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    map_of_preferences[r] = 1/(map_of_preferences.size()+1);
    double size = double(map_of_preferences.size());
    for (auto elem : map_of_preferences){
        map_of_preferences[elem.first] = 1/size;
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
    auto search = map_of_preferences.find(r);
    map_of_preferences.erase(search);
    double size = double(map_of_preferences.size());
    for (auto elem : map_of_preferences){
        map_of_preferences[elem.first] = 1/size;
    }
}

IPackageReceiver *ReceiverPreferences::choose_receiver(void) {
    double number = generator_();
    for (auto elem : map_of_preferences){
        number = number - elem.second;
        if (number <= 0){
            return elem.first;
        }
    }
    return nullptr;
}

void PackageSender::send_package(void) {
    if(buffer_){
        IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
        receiver->receive_package(std::move(buffer_.value()));
        buffer_.reset();
    }
}

void Ramp::deliver_goods(Time t) {
    if (t % di_ == 1){
        Package good;
        push_package(std::move(good));
    }
}

Worker::Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q){
    id_ = id;
    pd_ = pd;
    q_ = std::move(q);
    t_ = std::nullopt;
    work_buffer_ = std::nullopt;
}

void Worker::do_work(Time t) {
    if(!work_buffer_){
        if(!q_->empty()){
            t_ = t;
            Package elem = q_->pop();
            work_buffer_.emplace(std::move(elem));
        }
    }
    if(t_){
        if(t == t_.value() + pd_ - 1){
            push_package(std::move(work_buffer_.value()));
            t_.reset();
            work_buffer_.reset();
        }
    }
}

void Worker::receive_package(Package&& p) {
    q_->push(std::move(p));
}


Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d) {
    id_ = id;
    d_ = std::move(d);
}

void Storehouse::receive_package(Package&& p) {
    d_->push(std::move(p));
}
