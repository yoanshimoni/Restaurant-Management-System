#include "../include/Dish.h"
//
// Created by nehoray on 11/5/18.
//


Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type): id(d_id), name(d_name), price(d_price), type(d_type){}

Dish::~Dish(){}

int Dish::getId() const {
    return this->id;
}

std::string Dish::getName() const {
    return this->name;
}

int Dish::getPrice() const {
    return this->price;
}
DishType Dish::getType() const {
    return this-> type;
}
Dish* Dish::clone() {
    return new Dish(*this);
}

