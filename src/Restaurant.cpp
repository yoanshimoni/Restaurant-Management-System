#include "../include/Restaurant.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "../include/Customer.h"
#include "../include/Table.h"
#include "../include/Dish.h"
#include "../include/Action.h"
using namespace std;

//
// Created by nehoray on 11/5/18.
//


Restaurant::Restaurant() = default;



Restaurant::Restaurant(const Restaurant &other):open(other.open), tables(), menu(), actionsLog(),id(0) {
    for(int i=0;i<other.menu.size();i++)
        menu.push_back(other.menu[i]);

    for (int i=0; i<(signed) other.tables.size(); i++) {
        this->tables.push_back(other.tables[i]->clone()); //Making a deep copy of the elements from the other tablesList
    }
    for (int i=0; i<(signed) other.actionsLog.size(); i++) {
        this->actionsLog.push_back(other.actionsLog[i]->clone()); ////Making a deep copy of the elements from the other actionsLog
    }
}

Restaurant& Restaurant::operator=(const Restaurant &other) {
    if (this == &other) //Testing that we do not copy the same variable
        return *this;
    clear();
    for(int i=0; i<other.tables.size(); i++){
        this->tables.push_back(other.tables[i]->clone()); //Making a deep copy of the elements from the other tablesList
    }
    for (int i=0; i<actionsLog.size(); i++) {
        this->actionsLog.push_back(other.actionsLog[i]->clone()); ////Making a deep copy of the elements from the other actionsLog
    }
    menu.clear();
    for(const Dish& dish : other.menu){
        this->menu.push_back(dish);
    }
    open = other.open;
    return *this;
}

Restaurant::~Restaurant() {
    clear();
}

void Restaurant::clear() {
    if (tables.size() > 0) {
        for (int i = 0; i < tables.size(); i++) { //Delete the elements from the old tablesList
            delete tables[i];
            tables[i] = nullptr;
        }
        tables.clear();
        for (int i = 0; i < actionsLog.size(); i++) { //Delete the elements from the old actionsLog
            delete actionsLog[i];
            actionsLog[i] = nullptr;
        }
        actionsLog.clear();
    }
    else {
        return;
    }
}

Restaurant::Restaurant(Restaurant &&other):open(other.open), tables(), menu(other.menu), actionsLog() {
    for (int i=0; i<tables.size(); i++) {
        tables.push_back(other.tables[i]->clone()); //Making a deep copy of the elements from the other tablesList
    }
    for (int i=0; i<actionsLog.size(); i++) {
        actionsLog.push_back(other.actionsLog[i]->clone()); ////Making a deep copy of the elements from the other actionsLog
    }
    other.clear();
}

Restaurant &Restaurant::operator=(Restaurant &&other) {//move operator
    if (this == &other) //Testing that we do not copy the same variable
        return *this;
    clear();
    menu.clear();
    actionsLog.clear();
    tables.clear();
    //we will copy the pointers to the tables, actions, and dishes from other
    for(auto i : other.tables)
        tables.push_back(i);
    for(auto i : other.menu)
        menu.push_back(i);
    for(auto i : other.actionsLog)
        actionsLog.push_back(i);

    other.open= false;
    other.tables.clear();
    other.actionsLog.clear();
    other.menu.clear();

    other.clear();
    return *this;
}

Restaurant::Restaurant(const std::string &configFilePath): open(true),tables(),menu(), actionsLog() {//constructor
    ifstream conf(configFilePath);
    std::string line;
    vector<string> lines;
    if(conf.is_open()){
        while(getline(conf, line)){
            if(line[line.find_first_not_of(" \t")]!='#'){
                lines.push_back(line);
            }
        }
    }
    conf.close();

    initTables(lines[0], lines[1]);
    lines.erase(lines.begin(), lines.begin()+2);
    initMenu(lines);
}


void Restaurant::start() {
    std::cout << "Restaurant is now open!" << std::endl;
    std::string line;
    std::string word;
    int tableId = 0;
    int customerId = 0;


    while (line != "closeall") {
        getline(std::cin, line);
        std::istringstream str(line);
        str >> word;
        std::vector<Customer *> customerList;
        if (word == "open") {
            std::string customerType;
            std::string customerName;
            str >> tableId;
                while (str.good() && str >> word) {
                    customerName = word.substr(0, word.find(','));
                    customerType = word.substr(word.find(',') + 1, word.size());     ////
                    if (customerType == "veg") {
                        Customer *t = new VegetarianCustomer(customerName, customerId);
                        customerList.push_back(t);
                        ++customerId;
                    } else if (customerType == "chp") {
                        Customer *t = new CheapCustomer(customerName, customerId);
                        customerList.push_back(t);
                        ++customerId;
                    } else if (customerType == "spc") {
                        Customer *t = new SpicyCustomer(customerName, customerId);
                        customerList.push_back(t);
                        ++customerId;

                    } else if (customerType == "alc") {
                        Customer *t = new AlchoholicCustomer(customerName, customerId);
                        customerList.push_back(t);
                        ++customerId;
                    }
                }

                OpenTable *openTable = new OpenTable(tableId, customerList);
                openTable->act(*this);

                actionsLog.push_back(openTable);
                ++tableId;



        } else if (word == "order") {
            str >> tableId;
            Order *order = new Order(tableId);
            order->act(*this);
            actionsLog.push_back(order);


        } else if (word == "move") {
            str >> word;
            int src = std::stoi(word);
            str >> word;
            int dest = std::stoi(word);
            str >> customerId;
            MoveCustomer *move = new MoveCustomer(src, dest, customerId);
            move->act(*this);
            actionsLog.push_back(move);

        } else if (word == "close") {
            str >> word;
            tableId = std::stoi(word);
            Close *close = new Close(tableId);
            close->act(*this);
            actionsLog.push_back(close);

        } else if (word == "closeall") {
            CloseAll *closeAll = new CloseAll();
            closeAll->act(*this);
            delete closeAll;
            return;

        } else if (word == "menu") {
            PrintMenu *printMenu = new PrintMenu();
            printMenu->act(*this);
            actionsLog.push_back(printMenu);

        } else if (word == "status") {
            str >> word;
            tableId = std::stoi(word);
            PrintTableStatus *printTableStatus = new PrintTableStatus(tableId);
            printTableStatus->act(*this);
            actionsLog.push_back(printTableStatus);
        }
          else if (word == "log") {
            PrintActionsLog *printActionsLog = new PrintActionsLog();
            printActionsLog->act(*this);
            delete printActionsLog;

        } else if (word == "backup") {
            BackupRestaurant *backupRestaurant = new BackupRestaurant();
            backupRestaurant->act(*this);
            actionsLog.push_back(backupRestaurant);


        } else if (word == "restore") {
            RestoreResturant *restoreRestaurant=new RestoreResturant;
            restoreRestaurant->act(*this);
            actionsLog.push_back(restoreRestaurant);
        }
        word = "";

    }
}
std::vector<std::string>& Restaurant::split(const std::string& s, char delimiter){
    std::vector<std::string> *tokens = new std::vector<std::string>;
    std::string token;
    std::istringstream tokenstream(s);
    while (std::getline(tokenstream, token, delimiter)){
        tokens->push_back(token);
    }
    return *tokens;
}

int Restaurant::getNumOfTables() const {
    return static_cast<int>(this->tables.size());
}


Table* Restaurant::getTable(int ind) {
    if(ind>=tables.size()||ind<0)
        return nullptr;
    return tables[ind];
}


const std::vector<BaseAction*>& Restaurant::getActionsLog() const{
    return this->actionsLog;
}

std::vector<Dish> &Restaurant::getMenu() {
    return this->menu;
}
void Restaurant::close() {
    this->open= false;
}
void Restaurant::initTables(string &number, string &sizes) {
    string token;
    tables.reserve(stoul(number));

    istringstream is(sizes);

    while(getline(is, token, ',')){
        tables.push_back(new Table(stoi(token)));
    }
}
void Restaurant::initMenu(vector<string> &Dishes) {
    string Name;
    string Type;
    string Price;
    DishType Etype = VEG;

    for (int j = 0; j < Dishes.size(); j++) {
        istringstream is(Dishes[j]);
        getline(is, Name, ',');
        getline(is, Type, ',');
        getline(is, Price, ',');

        if (Type == "VEG")
            Etype = VEG;

        if (Type == "SPC")
            Etype = SPC;
        if (Type == "BVG")
            Etype = BVG;
        if (Type == "ALC")
            Etype = ALC;


        menu.push_back(Dish(j, Name, stoi(Price), Etype));
    }
}

