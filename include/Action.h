//
// Created by yoan@wincs.cs.bgu.ac.il on 11/19/18.
//

#ifndef SPL_ASS_ACTION_H
#define SPL_ASS_ACTION_H


#include <string>
#include <iostream>
#include "../include/Customer.h"
#include "../include/Table.h"
using namespace std;

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    BaseAction();
    ActionStatus getStatus() const;
    virtual void act(Restaurant& restaurant)=0;
    virtual std::string toString() const=0;
    virtual BaseAction* clone()=0;
    virtual ~BaseAction(); //destructor
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    void act(Restaurant &restaurant);
    std::string toString() const;
    OpenTable* clone();
    virtual ~OpenTable();
    OpenTable (const OpenTable& other); //copy constructor
    OpenTable(OpenTable &&other); //move constructor
private:
    const int tableId;
    std::vector<Customer*> customers;
    void clear();
    string customerString;
};


class Order : public BaseAction {
public:
    Order(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    Order* clone();
    virtual ~Order();
private:
    const int tableId;
    string output;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    void act(Restaurant &restaurant);
    std::string toString() const;
    MoveCustomer* clone();
    virtual ~MoveCustomer();
private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    Close* clone();
    virtual ~Close();
private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
    CloseAll* clone();
    virtual ~CloseAll();
private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;
    PrintMenu* clone();
    virtual ~PrintMenu();
private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    void act(Restaurant &restaurant);
    std::string toString() const;
    PrintTableStatus* clone();
    virtual ~PrintTableStatus();
private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
    PrintActionsLog* clone();
    virtual ~PrintActionsLog();
private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BackupRestaurant* clone();
    virtual ~BackupRestaurant();
private:
};


class RestoreResturant : public BaseAction {
public:
    RestoreResturant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    RestoreResturant* clone();
    virtual ~RestoreResturant();
};



#endif //SPL_ASS_ACTION_H
