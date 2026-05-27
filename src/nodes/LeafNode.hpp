#ifndef LEAF_NODE
#define LEAF_NODE

#include "Node.hpp"
#include <vector>
#include <cstdint>
#include <string>

class LeafNode : public Node
{
public:
    std::vector<uint8_t> keyEnd;
    std::string value;

    LeafNode() { type = NodeType::LEAF; }
    LeafNode(const std::string &_value, const std::vector<uint8_t> &_keyEnd) : value(_value), keyEnd(_keyEnd) { type = NodeType::LEAF; }
};

#endif