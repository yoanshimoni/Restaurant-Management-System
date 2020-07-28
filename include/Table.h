//
// Created by yoan@wincs.cs.bgu.ac.il on 11/19/18.
//

#ifndef SPL_ASS_TABLE_H
#define SPL_ASS_TABLE_H

#include <vector>
#include "../include/Customer.h"
#include "../include/Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public:
    Table(int t_capacity);
    Table (const Table& other); //copy constructor
    Table& operator=(const Table& other); //copy assignment operator
    virtual ~Table(); //destructor
    Table(Table &&other); //move constructor
    Table& operator=(Table&& other); //move assignment operator
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const std::vector<Dish> &menu);
    void openTable();
    void closeTable();
    int getBill();
    bool isOpen();
    std::string GetOutput();
    void deleteOutput();
    void CloseTable();
    Table* clone();
    Dish getDish (const std::vector<Dish> &menu, int d_id);
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::string output;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
    void clear(); //private method for the destructor
};


#endif