// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)
#include "storage_types.hpp"

Package PackageQueue::pop() {
    Package elem;
    switch(queue_type_){
        case PackageQueueType::FIFO:
            elem = std::move(queue_.front());
            queue_.pop_front();
            break;
        case PackageQueueType::LIFO:
            elem = std::move(queue_.back());
            queue_.pop_back();
            break;
    }
    return elem;
}