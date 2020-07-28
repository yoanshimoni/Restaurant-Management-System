//
// Created by yoan@wincs.cs.bgu.ac.il on 11/19/18.
//

#ifndef SPL_ASS_CUSTOMER_H
#define SPL_ASS_CUSTOMER_H

#include <vector>
#include <string>
#include "../include/Dish.h"

class Customer{
public:
    virtual ~Customer();
    Customer(std::string c_name, int c_id);
    virtual std::vector<int> order(const std::vector<Dish> &menu)=0;
    virtual std::string toString() const = 0;
    std::string getName() const;
    int getId() const;
    virtual Customer* clone()=0;
    virtual std::string type()=0;

private:
    const std::string name;
    const int id;
};


class VegetarianCustomer : public Customer {
public:
    VegetarianCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    VegetarianCustomer* clone();
    std::string type();
private:
};


class CheapCustomer : public Customer {
public:
    CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    CheapCustomer* clone();
    std::string type();
private:
    bool didOrder;
};


class SpicyCustomer : public Customer {
public:
    SpicyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    SpicyCustomer* clone();
    std::string type();
private:
    int OrdersCounter;
};


class AlchoholicCustomer : public Customer {
public:
    AlchoholicCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Dish> &menu);
    std::string toString() const;
    std::vector<int> organizeALC(const std::vector<Dish>& ALC);
    AlchoholicCustomer* clone();
    std::string type();
private:
    int lastId, lastPrice;
};


#endif