//
// Created by yoan@wincs.cs.bgu.ac.il on 11/19/18.
//

#ifndef SPL_ASS_DISH_H
#define SPL_ASS_DISH_H

#include <string>

enum DishType{
    VEG, SPC, BVG, ALC
};

class Dish{
public:
    Dish(int d_id, std::string d_name, int d_price, DishType d_type);
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    DishType getType() const;
    Dish* clone();
    ~Dish();
private:
    const int id;
    const std::string name;
    const int price;
    const DishType type;
};


#endif