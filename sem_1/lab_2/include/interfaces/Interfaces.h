#ifndef INTERFACES_H
#define INTERFACES_H

#include <string>

class IPowerable {
public:
    virtual ~IPowerable() = default;
    virtual void powerOn() = 0;
    virtual void powerOff() = 0;
    virtual bool isPowered() const = 0;
};

class ICoolable {
public:
    virtual ~ICoolable() = default;
    virtual double getTemperature() const = 0;
    virtual void cool(double amount) = 0;
};

class IDiagnosable {
public:
    virtual ~IDiagnosable() = default;
    virtual std::string getDiagnostics() const = 0;
};

class Component : public IPowerable, public IDiagnosable {
protected:
    std::string name;
    std::string manufacturer;
    bool powered;
    double price;

public:
    Component(const std::string& n, const std::string& m, double p) 
        : name(n), manufacturer(m), powered(false), price(p) {}

    virtual ~Component() = default;

    void powerOn() override { powered = true; }
    void powerOff() override { powered = false; }
    bool isPowered() const override { return powered; }

    std::string getName() const { return name; }
    std::string getManufacturer() const { return manufacturer; }
    double getPrice() const { return price; }

    std::string getDiagnostics() const override {
        return name + " (" + manufacturer + ") - " + 
               (powered ? "ON" : "OFF");
    }
};

#endif // INTERFACES_H
