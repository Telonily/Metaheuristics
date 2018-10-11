#pragma once

#include <ostream>

class Item {
private:
    int profit, weight, assignedNode;
public:
    Item(int profit, int weight, int assignedNode);

    int getProfit() const;

    int getWeight() const;

    int getAssignedNode() const;

    friend std::ostream &operator<<(std::ostream &os, const Item &item);

    bool operator==(const Item& other) const;
};
