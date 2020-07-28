#include "../include/Customer.h"
#include <cmath>
//
// Created by nehoray on 11/5/18.
//
using namespace std;
Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id) {}

std::string Customer::getName() const {
    return this->name;
}
int Customer::getId() const {
    return this->id;
}
VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(name,id){}// This costumer orders only vegetarian dishes with the smallest id+ the most expensive
// none-alcoholic beverage.
std::vector<int> VegetarianCustomer::order(const std::vector<Dish> &menu){
    vector<int> output;
    int length=INT32_MAX;
    int index=-1;
    for(int j=0;j<(signed)menu.size();j++){//a loop to find the shortest vegetarian dish
        if(menu[j].getType()==VEG && menu[j].getId()<length){
            index = j;
            length = menu[j].getId();
        }
    }

    int price=-1;
    int B_index=-1;
    for(int j=0;j<(signed)menu.size();j++){//a loop to find the most expensive beverage
        if(menu[j].getType()==BVG && menu[j].getPrice()>price){
            price=menu[j].getPrice();
            B_index=j;
        }
    }
    if(index!=-1 and B_index!=-1){
        output.push_back(menu[index].getId());
        output.push_back(menu[B_index].getId());
        return output;
    }
    else
    {
        output.clear();
        return output;
    }
}

std::string VegetarianCustomer::type(){
    return "veg";
}
std::string VegetarianCustomer::toString() const{
    return this->getName()+",veg";
}
VegetarianCustomer* VegetarianCustomer::clone(){
    return new VegetarianCustomer(*this);
}

CheapCustomer::CheapCustomer(std::string name, int id):Customer(name,id),didOrder(false){}
std::vector<int> CheapCustomer::order(const std::vector<Dish> &menu) {
if(didOrder)//if he ordered already
    return vector<int>();

didOrder=true;

    int price=INT32_MAX;
    int index=-1;
    for(int i=0;i<(signed)menu.size();i++){
        if(menu[i].getPrice()<price){//makes sure that this is a cheaper dish
            price=menu[i].getPrice();
            index=i;
        }
    }
    if(index==-1) {
        return vector<int>();
    }

    vector<int> output;
    output.push_back(index);
    return output;
}

std::string CheapCustomer::type() {
    return "chp";
}
std::string CheapCustomer::toString() const{

    return this->getName()+",chp";
}

CheapCustomer* CheapCustomer::clone(){
    return new CheapCustomer(*this);
}

SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(name,id), OrdersCounter(0){

}
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu){
    int price=0;
    int index=-1;
    if(OrdersCounter==0){
        for(int i=0;i<(signed)menu.size();i++) {//finds the most expensive spicy dish

            if (menu[i].getType() == SPC and menu[i].getPrice() > price) {
                price = menu[i].getPrice();
                index = i;
            }
        }

        vector<int> output;
        output.push_back(index);
        OrdersCounter++;
        return output;
    }
    int nPrice=INT32_MAX;
    for(int i=0;i<(signed)menu.size();i++){
        if(menu[i].getType()==BVG and menu[i].getPrice()<nPrice){
            nPrice=menu[i].getPrice();
            index=i;
        }
    }
    vector<int> output;
    output.push_back(index);
    OrdersCounter++;
    return output;
}

std::string SpicyCustomer::type(){
    return "spc";
}
std::string SpicyCustomer::toString() const{
    return this->getName()+",spc";
}

SpicyCustomer* SpicyCustomer::clone(){
    return new SpicyCustomer(*this);
}


AlchoholicCustomer::AlchoholicCustomer(std::string name, int id):Customer(name,id), lastId(0),lastPrice(-1){}
std::vector<int> organizeALC(std::vector<Dish> &menu){// gets the menu,and returns a vector of the alchoholic beverages' id in order
    std::vector<Dish> alc;
    for(int i=0;i<(signed)menu.size();i++){
        if(menu[i].getType()==ALC)
            alc.push_back(menu[i]);
    }
    vector<int> ID(alc.size());
    int n=alc.size();
    for(int j=0;j<n;j++){
        int minimalIndex=0;
        for(int i=0;i<n;i++){
            if (alc[i].getPrice()<alc[minimalIndex].getPrice())
                minimalIndex=i;
        }
        ID.push_back(alc[minimalIndex].getId());
        delete &alc[minimalIndex];
    }
    return ID;

};
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu){

    vector<int> output;
    const Dish *dish = nullptr;
    if (lastPrice == -1) { //First run
        for (const Dish &menuDish : menu) {
            if (menuDish.getType() == ALC) {
                if (dish == nullptr || dish->getPrice() > menuDish.getPrice())
                    dish = &menuDish;
            }
        }
    }
    else {
        for (const Dish &menuDish : menu) {
            if (menuDish.getType() == ALC) {
                if (dish == nullptr) {
                    if (menuDish.getId() != lastId &&
                        (menuDish.getPrice() > lastPrice || (menuDish.getPrice() == lastPrice && menuDish.getId() > lastId)))
                        dish = &menuDish;
                }
                else {
                    if ((menuDish.getPrice() < dish->getPrice() && menuDish.getPrice() > lastPrice) && menuDish.getId() != lastId)
                        dish = &menuDish;
                }
            }
        }
    }
    if (dish != nullptr) {
        output.push_back(dish->getId());
        lastId = dish->getId();
        lastPrice = dish->getPrice();
    }

    return output;
}

std::string AlchoholicCustomer::type() {
    return "alc";
}
std::string AlchoholicCustomer::toString() const{
    return this->getName()+",alc";
}

std::vector<int> AlchoholicCustomer::organizeALC(const std::vector<Dish>& ALC) {
    return vector<int>();
}

AlchoholicCustomer* AlchoholicCustomer::clone(){
    return new AlchoholicCustomer(*this);
}

Customer::~Customer() {
}

