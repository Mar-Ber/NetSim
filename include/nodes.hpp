// Ben(302821), Bereznicki(302822), Dembrovskyi(302835)

#ifndef NETSIM_NODES_HPP
#define NETSIM_NODES_HPP

#include "package.hpp"
#include "storage_types.hpp"
#include "types.hpp"
#include "helpers.hpp"
#include "config.hpp"
#include <memory>
#include <map>
#include <random>
#include <functional>
#include <optional>

enum class ReceiverType{
    WORKER, STOREHOUSE
};

class IPackageReceiver{
public:
    using iterator = std::list<Package>::iterator;
    using const_iterator = std::list<Package>::const_iterator;

    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id(void) const = 0;
    virtual ReceiverType get_receiver_type(void) const = 0;

    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;
};

class ReceiverPreferences{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    using iterator = preferences_t::iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator): generator_(pg) {}
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver(void);
    const preferences_t& get_preferences() const {return map_of_preferences;};

    const_iterator begin() const {return map_of_preferences.cbegin();}
    const_iterator cbegin() const {return map_of_preferences.cbegin();}
    const_iterator end() const {return map_of_preferences.cend();}
    const_iterator cend() const {return map_of_preferences.cend();}

private:
    ProbabilityGenerator generator_;
    preferences_t map_of_preferences;
};

class PackageSender{
public:
    PackageSender(): buffer_(std::nullopt){}
    PackageSender(PackageSender&&) = default;
    ReceiverPreferences receiver_preferences_;
    void send_package(void);
    const std::optional<Package>& get_sending_buffer(void) const {return buffer_;};

protected:
    std::optional<Package> buffer_;
    void push_package(Package&& item){buffer_.emplace(std::move(item));}
};

class Ramp: public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di): id_(id), di_(di) {}
    void deliver_goods(Time t);
    TimeOffset get_delivery_interval(void) const {return di_;};
    ElementID get_id(void) const {return id_;};

private:
    ElementID id_;
    TimeOffset di_;
};

class Worker: public PackageSender, public IPackageReceiver{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q);
    void do_work(Time t);
    TimeOffset get_processing_duration(void) const {return pd_;};
    Time get_package_processing_start_time(void) const {return t_.value();};
    void receive_package (Package&& p) override;
    ElementID get_id (void) const override {return id_;};
    ReceiverType get_receiver_type(void) const override {return ReceiverType::WORKER;};

    const_iterator begin() const override {return q_.get()->cbegin();}
    const_iterator cbegin() const override {return q_.get()->cbegin();}
    const_iterator end() const override {return q_.get()->cend();}
    const_iterator cend() const override {return q_.get()->cend();}

private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;
    std::optional<Time> t_;
    std::optional<Package> work_buffer_;
};

class Storehouse: public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO));
    void receive_package (Package&& p) override;
    ElementID get_id (void) const override {return id_;};
    ReceiverType get_receiver_type(void) const override {return ReceiverType::STOREHOUSE;};

    const_iterator begin() const override {return d_.get()->cbegin();}
    const_iterator cbegin() const override {return d_.get()->cbegin();}
    const_iterator end() const override {return d_.get()->cend();}
    const_iterator cend() const override {return d_.get()->cend();}

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};
#endif //NETSIM_NODES_HPP
