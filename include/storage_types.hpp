// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)
#ifndef NETSIM_STORAGE_TYPES_HPP
#define NETSIM_STORAGE_TYPES_HPP

#include "package.hpp"
#include "types.hpp"
#include <list>

enum class PackageQueueType{
    FIFO, LIFO
};

class IPackageStockpile{
public:
    using iterator = std::list<Package>::iterator;
    using const_iterator = std::list<Package>::const_iterator;

    virtual void push(Package&& elem) = 0;
    virtual bool empty() const = 0;
    virtual size_type size() const = 0;

    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;

    virtual ~IPackageStockpile() = default;

};

class IPackageQueue: public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() const = 0;

    virtual ~IPackageQueue() = default;
};

class PackageQueue: public IPackageQueue{
public:
    PackageQueue(PackageQueueType queue_type): queue_type_(queue_type) {}
    void push(Package &&elem) override {queue_.emplace_back(std::move(elem));}
    Package pop() override;
    size_type size() const override {return queue_.size();}
    bool empty() const override {return queue_.size() == 0;}
    PackageQueueType get_queue_type() const override {return queue_type_;}

    const_iterator begin() const override {return queue_.cbegin();}
    const_iterator cbegin() const override {return queue_.cbegin();}
    const_iterator end() const override {return queue_.cend();}
    const_iterator cend() const override {return queue_.cend();}

    ~PackageQueue() = default;
private:
    PackageQueueType queue_type_;
    std::list<Package> queue_;

};

#endif //NETSIM_STORAGE_TYPES_HPP
