#ifndef BRANCH_NODE
#define BRANCH_NODE

#include "Node.hpp"
#include <vector>
#include <array>
#include <cstdint>
#include <string>
#include <memory>

class BranchNode : public Node
{
public:
    std::array<std::shared_ptr<Node>, 16> children;
    std::string value;

    BranchNode() { type = NodeType::BRANCH; }
    BranchNode(const std::string &_value, const std::array<std::shared_ptr<Node>, 16> &_children) : value(_value), children(_children) { type = NodeType::BRANCH; }
};

#endif