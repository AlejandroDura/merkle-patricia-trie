#ifndef PATRICIA_TRIE
#define PATRICIA_TRIE

#include "nodes/Node.hpp"
#include "nodes/BranchNode.hpp"
#include <vector>
#include <cstdint>
#include <string>
#include <memory>

class PatriciaTrie
{
public:
    std::shared_ptr<Node> root;
    PatriciaTrie();

    void insert(const std::vector<uint8_t> &key, const std::string &value);
    void printKeys();

private:
    void insert_rec(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value);

    void _node_leaf_handler(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value);
    void _node_extension_handler(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value);

    void _node_leaf_handler_v2(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value);
    void _node_extension_handler_v2(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value);
    void _node_branch_handler_v2(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value);

    void attachLeafToBranch(std::shared_ptr<BranchNode> &branch, uint8_t dir, const std::string &value, const std::vector<uint8_t> &keyEnd);

    std::vector<std::vector<uint8_t>> _getAllKeys(const std::shared_ptr<Node> &currentNode);
};

#endif