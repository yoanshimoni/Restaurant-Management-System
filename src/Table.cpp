#include <iostream>
#include "../include/Table.h"
//
// Created by nehoray on 11/5/18.
//
using namespace std;

Table::Table(int t_capacity):capacity(t_capacity), open(false), customersList(), output(""), orderList(){}

Table::Table (const Table& other):capacity(other.capacity), open(other.open), customersList(), output(other.output), orderList(other.orderList) {
    for (int i=0; i<(signed) other.customersList.size(); i++) {
        customersList.push_back(other.customersList[i]->clone()); //Making a deep copy of the elements from the other customersList
    }
}

Table& Table::operator=(const Table& other) {

    if (this == &other) //Testing that we do not copy the same variable
        return *this;
    clear();
    for(int i=0; i<other.customersList.size(); i++){
        customersList.push_back(other.customersList[i]->clone()); //Making a deep copy of the elements from the other customersList
    }
    capacity = other.capacity;
    open = other.open;
    output = other.output;
    orderList.clear();
    for(int i=0; i<other.orderList.size(); i++){
        orderList.push_back(other.orderList[i]);
    }
}

Table::~Table() {
    clear();
}

void Table::clear() {
    if ((signed)customersList.size() > 0) {
        for (int i = 0; i < (signed)customersList.size(); i++) { //Delete the elements from the old customersList
            delete customersList[i];
            customersList[i] = nullptr;
        }
        //customersList.clear();
    }
    return;
}
Table::Table(Table &&other):capacity(other.capacity), open(other.open), customersList(), output(other.output), orderList(other.orderList){

    for (int i=0; i<other.customersList.size(); i++) {
        customersList.push_back(other.customersList[i]); //Making a deep copy of the elements from the other customersList
    }
    other.clear();
}

Table& Table::operator=(Table &&other){//move assignment
    if (this == &other) //Testing that we do not copy the same variable
        return *this;
    clear();
    for(int i=0; i<other.customersList.size(); i++){
        customersList.push_back(other.customersList[i]); //Making a deep copy of the elements from the other customersList
    }
    capacity = other.capacity;
    open = other.open;
    output = other.output;
    orderList.clear();
    for(int i=0; i<other.orderList.size(); i++){
        orderList.push_back(other.orderList[i]);
    }
    other.clear();
}

int Table::getCapacity() const {
    return this-> capacity;
}

void Table::addCustomer(Customer *customer) {
    customersList.push_back(customer);
}

void Table::order(const std::vector<Dish> &menu) {
    for (auto &i : customersList) {
        std:: vector <int> orders = i->order(menu);
        for (int order : orders) {
            orderList.push_back(OrderPair(i->getId(),getDish(menu,order)));
            std::cout<<i->getName()+" ordered "+getDish(menu,order).getName()<< endl;
        }
    }
}
void Table::openTable() {
    this->open= true;
}


void Table::closeTable() {
    this->open=false;
}

int Table::getBill() {
    int sum = 0;
    for(OrderPair p : this->orderList){
        sum = sum + p.second.getPrice();
    }
    return sum;
}

bool Table::isOpen() {
    return this->open;
}

std::string Table::GetOutput() {
    return this->output;
}

void Table::deleteOutput() {
    this->output = "";
}
std::vector<OrderPair>& Table::getOrders(){
    return this->orderList;
}
void Table::CloseTable() {
    this->open=false;
}

Table* Table::clone() {
    return new Table(*this);
}

void Table::removeCustomer(int id) {
    for (int i=0; i<customersList.size(); i++){
        if(customersList[i]->getId()==id){
            customersList.erase(customersList.begin()+i);
        }
    }
}

Customer* Table::getCustomer(int id) {
    for (int i=0; i<customersList.size(); i++){
        if(customersList[i]->getId()==id){
            return customersList[i];
        }
    }
}

std::vector<Customer*> &Table::getCustomers() {
    return customersList;
}
Dish Table::getDish (const std::vector<Dish> &menu, int d_id) {   //changed by Alon to Dish*//
    for(int i=0; i<menu.size();i++){
        if (menu[i].getId() == d_id){
            // return new Dish(dish);
            return menu[i];
        }
    }
    cout<<"dish is not in the menu!"<<endl;
}