#ifndef EXTENSION_NODE
#define EXTENSION_NODE

#include "Node.hpp"
#include <vector>
#include <cstdint>
#include <string>
#include <memory>

class ExtensionNode : public Node
{
public:
    std::vector<uint8_t> sharedNibble;
    std::shared_ptr<Node> child;

    ExtensionNode() { type = NodeType::EXTENSION; }
    ExtensionNode(const std::vector<uint8_t> &_sharedNibble, const std::shared_ptr<Node> &_child) : sharedNibble(_sharedNibble), child(_child) { type = NodeType::EXTENSION; }
};

#endif