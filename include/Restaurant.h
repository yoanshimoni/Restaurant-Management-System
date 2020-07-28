//
// Created by yoan@wincs.cs.bgu.ac.il on 11/19/18.
//

#ifndef SPL_ASS_RESTAURANT_H
#define SPL_ASS_RESTAURANT_H


#include <vector>
#include <string>
#include "../include/Dish.h"
#include "../include/Table.h"
#include "../include/Action.h"
#include <iostream>
#include <fstream>


class Restaurant{
public:
    Restaurant();
    Restaurant (const Restaurant& other); //copy constructor
    Restaurant& operator=(const Restaurant& other); //copy assignment operator
    virtual ~Restaurant(); //destructor
    Restaurant(Restaurant &&other); //move constructor
    Restaurant& operator=(Restaurant&& other); //move assignment operator
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Dish>& getMenu();
    void close();
    void initTables(string &number, string &sizes);
    void initMenu(vector<string> &Dishes);
    std::vector<std::string>& split(const std::string& s, char delimiter);

private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
    void clear(); //private method for the destructor
    int id;//customer's id
};

#endif