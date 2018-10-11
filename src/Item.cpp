//
// Created by User on 10/11/2018.
//

#include "Item.h"

Item::Item(int profit, int weight, int assignedNode) : profit(profit), weight(weight), assignedNode(assignedNode) {}

int Item::getProfit() const {
    return profit;
}

std::ostream &operator<<(std::ostream &os, const Item &item) {
    os << "profit: " << item.profit << " weight: " << item.weight << " assignedNode: " << item.assignedNode;
    return os;
}

bool Item::operator==(const Item& other) const
{
    return profit == other.profit && weight == other.weight && assignedNode == other.assignedNode;
}

int Item::getWeight() const {
    return weight;
}

int Item::getAssignedNode() const {
    return assignedNode;
}
