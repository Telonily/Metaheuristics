#pragma once

#include <ostream>

class Item {
private:
    int profit, weight, assignedNode, id;
public:
    Item(int profit, int weight, int assignedNode, int id);

    int getProfit() const;

    int getWeight() const;

    int getAssignedNode() const;

    int getId() const;

    friend std::ostream &operator<<(std::ostream &os, const Item &item);

    bool operator==(const Item& other) const;

    float getRatio() const { return (float)profit/weight;};

    bool operator<(const Item &rhs) const { return getRatio() > rhs.getRatio(); }
};



