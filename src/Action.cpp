//
// Created by yoan@wincs.cs.bgu.ac.il on 11/19/18.
//

#include "../include/Action.h"
#include "../include/Restaurant.h"
#include <string>
#include <iostream>
#include <utility>

extern Restaurant* backup;
using namespace std;


BaseAction::BaseAction():errorMsg(),status(PENDING) {}

BaseAction::~BaseAction() {}

void BaseAction::complete(){
    status=COMPLETED;
}

void BaseAction::error(std::string errorMsg) {
    this->errorMsg="Error: "+errorMsg;
    status=ERROR;
    std::cout<<"Error: "+errorMsg<<std::endl;
}

std::string BaseAction::getErrorMsg() const {
    return this->errorMsg;
}

ActionStatus BaseAction::getStatus() const {
    return this->status;
}

OpenTable::OpenTable(int id, std::vector<Customer*> &customersList):tableId(id), customers(),customerString("") {
    for(auto i : customersList)
        customers.push_back(i);

}
void OpenTable::act(Restaurant &restaurant) {
    //filling the customers string:
    customerString="open "+to_string(tableId)+" ";
    for(int i=0;i<(signed)this->customers.size();i++)
        customerString=customerString+customers[i]->getName()+","+customers[i]->type()+" ";
    if (tableId>=restaurant.getNumOfTables()||tableId<0||restaurant.getTable(tableId)->isOpen()) {
        error("Table does not exist or is already open");
        for(auto i : customers){
            delete i;
            i=nullptr;
        }
    }
    else {
        for (int i = 0; i < (signed)customers.size(); i++) {
            restaurant.getTable(tableId)->addCustomer(customers[i]);
        }


        restaurant.getTable(tableId)->openTable();
        this->complete();
    }
    //complete the string:
    if(getStatus()==COMPLETED)
        customerString=customerString+"Completed";
    else{
        customerString=customerString+getErrorMsg();
    }

}

OpenTable::~OpenTable() {}

std::string OpenTable::toString() const{
    return customerString;
}
OpenTable* OpenTable::clone() {
    return new OpenTable(*this);
}

OpenTable::OpenTable(const OpenTable &other):tableId(other.tableId), customers() {
    for (int i=0; i<(signed)other.customers.size(); i++) {
        customers.push_back(other.customers[i]->clone()); //Making a deep copy of the elements from the other vector of customers
    }
}


OpenTable::OpenTable(OpenTable &&other):tableId(other.tableId), customers(){
    for (int i=0; i<(signed)other.customers.size(); i++) {
        customers.push_back(other.customers[i]->clone()); //Making a deep copy of the elements from the other vector of customers
    }
    other.clear();
}

void OpenTable::clear() { //Delete the elements from the old customers vector
    if (customers.size() > 0){
        for (int i=0; i<(signed)customers.size(); i++) {
            delete customers[i];
            customers[i] = nullptr;
        }
        customers.clear();
    }
    else {return;}
}


Order::Order(int id):tableId(id), output("") {}
void Order::act(Restaurant &restaurant){
    if(tableId>= (signed)restaurant.getNumOfTables() or tableId<0 or !restaurant.getTable(tableId)->isOpen()){
        error("Table does not exist or is not open");
        return;
    }
    restaurant.getTable(tableId)->order(restaurant.getMenu());
    this->complete();
}


Order* Order::clone() {
    return new Order(*this);
}

Order::~Order() {}

std::string Order::toString() const {
    if(getStatus()==COMPLETED)
        return "order " + std::to_string(tableId) + " " +"Completed" ;

    return "order " + std::to_string(tableId) + " " +getErrorMsg();
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId): BaseAction(),srcTable(src),dstTable(dst),id(customerId) {}
void MoveCustomer::act(Restaurant &restaurant) {
    Table *src_table = restaurant.getTable(srcTable);
    Table *dst_table = restaurant.getTable(dstTable);

    // Checking that both tables exist, open, that there is such customer at the source table, and that destination is not full.
    if (src_table == nullptr || dst_table == nullptr) {
        this->error("Cannot move customer");
        return;
    } else if (dst_table->isFull() || !src_table->isOpen() || !dst_table->isOpen() ||
               src_table->getCustomer(id) == nullptr) {
        this->error("Cannot move customer");
        std::cout << "Error: Cannot move customer" << std::endl;
        return;
    }

    dst_table->addCustomer(src_table->getCustomer(id));   //add customer to the new table
    src_table->removeCustomer(id);                       //remove from old table

    std::vector<OrderPair>& src_orders = src_table->getOrders();
    std::vector<OrderPair>& dst_orders = dst_table->getOrders();
    std::vector<OrderPair> new_src_orders;

    // going over all orders from the src table, and adding the orders which were made by the customer to the destination table
    // Copying the rest of the orders in order to add them to the source table orders again (without the customer`s orders)
    int num_of_orders = int(src_orders.size());
    for (int i = 0; i < num_of_orders; i++) {
        if (std::get<0>(src_orders[i]) == id) {
            dst_orders.push_back(src_orders[i]);
        } else {
            new_src_orders.push_back(src_orders[i]);
        }
    }

    src_orders.clear();
    for (auto &order : new_src_orders) {
        src_orders.push_back(order);
    }

    if (src_table->isEmpty())     //if the source table is empty - close it
        src_table->closeTable();

    this->complete();
}

MoveCustomer* MoveCustomer::clone() {
    return new MoveCustomer(*this);
}
MoveCustomer::~MoveCustomer() {}

std::string MoveCustomer::toString() const {
    if (getStatus() == COMPLETED)
        return "move " + to_string(srcTable) + " " + to_string(dstTable) + " " + to_string(id) + " Completed";

    return "move " + to_string(srcTable) + " " + to_string(dstTable) + " " + to_string(id) + " " + getErrorMsg();
}


Close::Close(int id):tableId(id){}
void Close::act(Restaurant &restaurant) {
    if(tableId>=restaurant.getNumOfTables()||tableId<0||!restaurant.getTable(tableId)->isOpen()){
        error("Table does not exist or is not open");
    }
    vector <OrderPair> tOrders = restaurant.getTable(tableId)->getOrders();
    int total_price = 0;
    for (int i=0; i<(signed)tOrders.size(); i++){
        total_price = total_price + tOrders[i].second.getPrice();
    }
    string bill="Table "+to_string(tableId)+" was closed. Bill "+to_string(total_price)+"NIS";
    cout<<bill<<endl;
    restaurant.getTable(tableId)->CloseTable();
    this->complete();
}
Close* Close::clone() {
    return new Close(*this);
}
Close::~Close() {}

std::string Close::toString() const {
    if(getStatus() == COMPLETED)
        return "close "+to_string(tableId)+" Completed" ;

    return "close "+to_string(tableId)+" "+ getErrorMsg();
}


CloseAll::CloseAll(){}
void CloseAll::act(Restaurant &restaurant){
    for (int i = 0; i < (signed)restaurant.getNumOfTables(); i++) {
        if (restaurant.getTable(i)->isOpen()) {
            Close action(i);
            action.act(restaurant);
        }
    }

    this->complete();
}

CloseAll* CloseAll::clone() {
    return new CloseAll(*this);
}
CloseAll::~CloseAll() {}

std::string CloseAll::toString() const {
    return std::__cxx11::string();
}

PrintMenu::PrintMenu() = default;
void PrintMenu::act(Restaurant &restaurant) {
    vector <Dish> &menu = restaurant.getMenu();
    for (int i=0; i<(signed)menu.size(); i++){
        if(menu[i].getType()==0)
            cout<<menu[i].getName()+" " + "VEG" +" " + to_string(menu[i].getPrice())+"NIS"<<endl;
        else if(menu[i].getType()==1)
            cout<<menu[i].getName()+" " + "SPC" +" " + to_string(menu[i].getPrice())+"NIS"<<endl;
        else if(menu[i].getType()==2)
            cout<<menu[i].getName()+" " + "BVG" +" " + to_string(menu[i].getPrice())+"NIS"<<endl;
        else
            cout<<menu[i].getName()+" " + "ALC" +" " + to_string(menu[i].getPrice())+"NIS"<<endl;
    }
    this->complete();
}


PrintMenu::~PrintMenu() {}

PrintMenu *PrintMenu::clone() {
    return new PrintMenu(*this);
}

std::string PrintMenu::toString() const {
    return "menu Completed";
}

PrintTableStatus::PrintTableStatus(int id): tableId(id){}
void PrintTableStatus::act(Restaurant &restaurant) {
    if(restaurant.getTable(tableId)->isOpen()==false){
        cout<<"Table " + to_string(tableId) + " status: closed"<<endl;
    }
    else {
        cout<<"Table " + to_string(tableId) + " status: open"<<endl;
        cout<<"Customers:"<<endl;
        vector <Customer *> &customerList = restaurant.getTable(tableId)->getCustomers(); // gets a reference to the customerList (of this Table)
        for(auto customer : customerList){
            cout<<to_string(customer->getId()) + " " + customer->getName()<<endl;
        }
        cout<< "Orders:"<<endl;
        vector <OrderPair> &orderList = restaurant.getTable(tableId)->getOrders(); // gets a reference to the orderList (of this Table)
        for(int i=0; i<(signed)orderList.size(); i++){
            for (int j=0; j<(signed)customerList.size(); j++) {
                if(orderList[i].first == customerList[j]->getId()){
                    cout<<orderList[i].second.getName() + " " + to_string(orderList[i].second.getPrice()) + "NIS"+" " + to_string(customerList[j]->getId())<<endl;
                }
            }
        }
        cout<<"Current Bill: " + to_string(restaurant.getTable(tableId)->getBill())+"NIS"<<endl;
    }
    this->complete();
}


PrintTableStatus::~PrintTableStatus() {}

PrintTableStatus *PrintTableStatus::clone() {
    return new PrintTableStatus(*this);
}

std::string PrintTableStatus::toString() const {
    if(getStatus()==COMPLETED)
        return "status " + to_string(tableId)+" Completed";

    return "status " + to_string(tableId)+" "+ getErrorMsg();
}

PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Restaurant &restaurant){
    string output="";
    vector<BaseAction*> Actions=restaurant.getActionsLog();
    for(int i=0;i<(signed)restaurant.getActionsLog().size();i++){

        output=output+Actions[i]->toString()+"\n";
    }
    cout<<output;
}


PrintActionsLog* PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}

PrintActionsLog::~PrintActionsLog() {}

std::string PrintActionsLog::toString() const {
return "log Completed";
}


BackupRestaurant::BackupRestaurant() {}

void BackupRestaurant::act(Restaurant &restaurant){
    if(backup== nullptr)
    ::backup=new Restaurant(restaurant);
    else
        *backup=restaurant;

    this->complete();

}

BackupRestaurant* BackupRestaurant::clone() {
    return new BackupRestaurant(*this);
}
BackupRestaurant::~BackupRestaurant() {}

std::string BackupRestaurant::toString() const {
    return "backup Completed";
}


RestoreResturant::RestoreResturant(){}

void RestoreResturant::act(Restaurant &restaurant){
    if(backup== nullptr){
        this->error("no backup available");
        return;
    }
    restaurant=*backup;
    this->complete();
}

RestoreResturant* RestoreResturant::clone() {
    return new RestoreResturant(*this);
}

RestoreResturant::~RestoreResturant() {}

std::string RestoreResturant::toString() const {
    if(getStatus()==COMPLETED)
        return "restore Completed";

    return "restore "+getErrorMsg();
}
