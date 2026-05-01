#pragma once
#include <string>

enum class ItemType { FUEL, REPAIR_KIT };

struct Item {
    ItemType type;
    std::string name;
    int price;
    float effectValue; // 에너지 회복량(FUEL) 또는 HP 회복량(REPAIR_KIT)
};
