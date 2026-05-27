#include "PatriciaTrie.hpp"
#include "nodes/LeafNode.hpp"
#include "nodes/ExtensionNode.hpp"
#include "nodes/BranchNode.hpp"
#include "utils/chains.hpp"

#include <array>

PatriciaTrie::PatriciaTrie()
{
}

void PatriciaTrie::insert(const std::vector<uint8_t> &key, const std::string &value)
{
    if (root == nullptr)
    {
        root = std::make_shared<LeafNode>(value, key);
    }
    else
    {
        insert_rec(root, key, value);
    }
}

void PatriciaTrie::attachLeafToBranch(std::shared_ptr<BranchNode> &branch, uint8_t dir, const std::string &value, const std::vector<uint8_t> &keyEnd)
{
    branch->children[dir] = std::make_shared<LeafNode>(value, keyEnd);
}

void PatriciaTrie::insert_rec(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value)
{
    if (auto leaf = std::dynamic_pointer_cast<LeafNode>(currentNode))
    {
        _node_leaf_handler_v2(currentNode, currentKey, value);
    }
    else if (auto extension = std::dynamic_pointer_cast<ExtensionNode>(currentNode))
    {
        _node_extension_handler_v2(currentNode, currentKey, value);
    }
    else if (auto branch = std::dynamic_pointer_cast<BranchNode>(currentNode))
    {
        _node_branch_handler_v2(currentNode, currentKey, value);
    }
}

void PatriciaTrie::_node_leaf_handler(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value)
{
    auto leaf = std::dynamic_pointer_cast<LeafNode>(currentNode);

    if (!leaf)
    {
        return;
    }

    std::vector<uint8_t> nodeKey = leaf->keyEnd;
    const std::string &nodeValue = leaf->value;

    if (nodeKey == currentKey)
    {
        leaf->value = value;
    }
    else
    {
        std::shared_ptr<BranchNode> newBranch = std::make_shared<BranchNode>();
        uint8_t oldDir;
        uint8_t newDir;
        std::vector<uint8_t> oldSuffix;
        std::vector<uint8_t> newSuffix;

        if (currentKey.empty())
        {
            // EMpezar a usa .empty() en lugar de .size() ya que las claves si van consumiendo poco a poco
            // Que pasa si nodeKey tambien es empty?
            oldDir = nodeKey[0];
            oldSuffix = chains::strip_first_nibble(nodeKey);
            newBranch->value = value;
            attachLeafToBranch(newBranch, oldDir, nodeValue, oldSuffix);
            currentNode = newBranch;
        }
        else
        {
            if (nodeKey.empty())
            {
                newDir = currentKey[0];
                newSuffix = chains::strip_first_nibble(currentKey);
                newBranch->value = nodeValue;

                attachLeafToBranch(newBranch, newDir, value, newSuffix);
                currentNode = newBranch;
            }
            else
            {
                std::vector<uint8_t> prefix = chains::get_prefix_hex(nodeKey, currentKey);
                if (prefix.empty())
                {
                    oldDir = nodeKey[0];
                    newDir = currentKey[0];
                    oldSuffix = chains::strip_first_nibble(nodeKey);
                    newSuffix = chains::strip_first_nibble(currentKey);

                    attachLeafToBranch(newBranch, oldDir, nodeValue, oldSuffix);
                    attachLeafToBranch(newBranch, newDir, value, newSuffix);
                    currentNode = newBranch;
                }
                else // Create a extension
                {
                    oldSuffix = chains::get_sufix(prefix, nodeKey);
                    newSuffix = chains::get_sufix(prefix, currentKey);

                    if (!oldSuffix.empty() && !newSuffix.empty())
                    {
                        oldDir = oldSuffix[0];
                        newDir = newSuffix[0];

                        oldSuffix = chains::strip_first_nibble(oldSuffix);
                        newSuffix = chains::strip_first_nibble(newSuffix);

                        attachLeafToBranch(newBranch, oldDir, nodeValue, oldSuffix);
                        attachLeafToBranch(newBranch, newDir, value, newSuffix);
                    }
                    else if (oldSuffix.empty() && !newSuffix.empty())
                    {
                        newBranch->value = nodeValue;
                        newDir = newSuffix[0];
                        newSuffix = chains::strip_first_nibble(newSuffix);
                        attachLeafToBranch(newBranch, newDir, value, newSuffix);
                    }
                    else if (newSuffix.empty() && !oldSuffix.empty())
                    {
                        newBranch->value = value;
                        oldDir = oldSuffix[0];
                        oldSuffix = chains::strip_first_nibble(oldSuffix);
                        attachLeafToBranch(newBranch, oldDir, nodeValue, oldSuffix);
                    }

                    std::shared_ptr<ExtensionNode> newExtension = std::make_shared<ExtensionNode>(prefix, newBranch);
                    currentNode = newExtension;
                }
            }
        }
    }
}

void PatriciaTrie::_node_extension_handler(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value)
{
    auto extension = std::dynamic_pointer_cast<ExtensionNode>(currentNode);

    if (!extension)
    {
        return;
    }

    std::vector<uint8_t> nodeKey = extension->sharedNibble;
    auto nodeChild = std::dynamic_pointer_cast<BranchNode>(extension->child);
    uint8_t oldDir;
    uint8_t newDir;
    std::vector<uint8_t> oldSuffix;
    std::vector<uint8_t> newSuffix;

    if (nodeKey == currentKey)
    {
        if (!nodeChild)
        {
            return;
        }

        nodeChild->value = value;
    }
    else
    {
        std::shared_ptr<BranchNode> newBranch = std::make_shared<BranchNode>();
        if (currentKey.empty())
        {
            oldDir = nodeKey[0];
            oldSuffix = chains::strip_first_nibble(nodeKey);

            if (oldSuffix.empty())
            {
                newBranch->children[oldDir] = nodeChild;
            }
            else
            {
                extension->sharedNibble = oldSuffix;
                newBranch->children[oldDir] = extension;
            }

            newBranch->value = value;
            currentNode = newBranch;
        }
        else
        {
            std::vector<uint8_t> prefix = chains::get_prefix_hex(nodeKey, currentKey);

            if (prefix.empty())
            {
                oldDir = nodeKey[0];
                newDir = currentKey[0];
                oldSuffix = chains::strip_first_nibble(nodeKey);
                newSuffix = chains::strip_first_nibble(currentKey);

                if (oldSuffix.empty())
                {
                    newBranch->children[oldDir] = nodeChild;
                }
                else
                {
                    extension->sharedNibble = oldSuffix;
                    newBranch->children[oldDir] = extension;
                }

                attachLeafToBranch(newBranch, newDir, value, newSuffix);
                currentNode = newBranch;
            }
            else // Create new extension
            {

                oldSuffix = chains::get_sufix(prefix, nodeKey);
                newSuffix = chains::get_sufix(prefix, currentKey);

                if (!oldSuffix.empty() && !newSuffix.empty())
                {
                    std::shared_ptr<ExtensionNode> newExtension = std::make_shared<ExtensionNode>();
                    newExtension->sharedNibble = prefix;
                    newExtension->child = newBranch;

                    oldDir = oldSuffix[0];
                    newDir = newSuffix[0];

                    oldSuffix = chains::strip_first_nibble(oldSuffix);
                    newSuffix = chains::strip_first_nibble(newSuffix);

                    if (oldSuffix.empty())
                    {
                        newBranch->children[oldDir] = nodeChild;
                    }
                    else
                    {
                        extension->sharedNibble = oldSuffix;
                        newBranch->children[oldDir] = extension;
                    }

                    attachLeafToBranch(newBranch, newDir, value, newSuffix);
                    currentNode = newExtension;
                }
                else if (!oldSuffix.empty() && newSuffix.empty())
                {
                    nodeChild->value = value;
                }
                else if (oldSuffix.empty() && !newSuffix.empty()) // CurrentKey está dentro de  currentNode (current key entra dentro del extension actual)
                {
                    newDir = newSuffix[0];
                    newSuffix = chains::strip_first_nibble(newSuffix);

                    if (nodeChild->children[newDir] != nullptr)
                    {
                        // recursion al nodo que hay dentro de childen[newDir]
                    }
                    else
                    {
                        attachLeafToBranch(nodeChild, newDir, value, newSuffix);
                    }
                }
            }
        }
    }
}

void PatriciaTrie::_node_branch_handler_v2(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value)
{
    auto branch = std::dynamic_pointer_cast<BranchNode>(currentNode);

    if (!branch)
    {
        return;
    }

    if (currentKey.empty())
    {
        branch->value = value;
    }
    else
    {
        std::array<std::shared_ptr<Node>, 16> &children = branch->children;

        uint8_t dir = currentKey[0];
        std::vector<uint8_t> keySuffix = chains::strip_first_nibble(currentKey);

        if (children[dir])
        {
            insert_rec(children[dir], keySuffix, value);
        }
        else
        {
            std::shared_ptr<LeafNode> newLeaf = std::make_shared<LeafNode>();
            newLeaf->keyEnd = keySuffix;
            newLeaf->value = value;
            children[dir] = newLeaf;
        }
    }
}

void PatriciaTrie::_node_leaf_handler_v2(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value)
{
    auto leaf = std::dynamic_pointer_cast<LeafNode>(currentNode);

    if (!leaf)
    {
        return;
    }

    std::vector<uint8_t> nodeKey = leaf->keyEnd;
    const std::string &nodeValue = leaf->value;

    if (nodeKey == currentKey)
    {
        leaf->value = value;

        return;
    }

    std::shared_ptr<BranchNode> newBranch = std::make_shared<BranchNode>();
    uint8_t oldDir;
    uint8_t newDir;
    std::vector<uint8_t> oldSuffix;
    std::vector<uint8_t> newSuffix;

    if (nodeKey.empty())
    {
        newDir = currentKey[0];
        newSuffix = chains::strip_first_nibble(currentKey);
        newBranch->value = nodeValue;

        attachLeafToBranch(newBranch, newDir, value, newSuffix);
        currentNode = newBranch;
    }
    else if (currentKey.empty())
    {
        oldDir = nodeKey[0];
        oldSuffix = chains::strip_first_nibble(nodeKey);
        newBranch->value = value;

        attachLeafToBranch(newBranch, oldDir, nodeValue, oldSuffix);
        currentNode = newBranch;
    }
    else
    {
        std::vector<uint8_t> prefix = chains::get_prefix_hex(nodeKey, currentKey);

        if (prefix.empty())
        {
            oldDir = nodeKey[0];
            newDir = currentKey[0];
            oldSuffix = chains::strip_first_nibble(nodeKey);
            newSuffix = chains::strip_first_nibble(currentKey);

            attachLeafToBranch(newBranch, oldDir, nodeValue, oldSuffix);
            attachLeafToBranch(newBranch, newDir, value, newSuffix);

            currentNode = newBranch;
        }
        else
        {
            std::shared_ptr<ExtensionNode> newExtension = std::make_shared<ExtensionNode>();
            newExtension->sharedNibble = prefix;
            newExtension->child = newBranch;

            oldSuffix = chains::get_sufix(prefix, nodeKey);
            newSuffix = chains::get_sufix(prefix, currentKey);

            if (oldSuffix.empty())
            {
                newBranch->value = nodeValue;
            }
            else
            {
                oldDir = oldSuffix[0];
                oldSuffix = chains::strip_first_nibble(oldSuffix);
                attachLeafToBranch(newBranch, oldDir, nodeValue, oldSuffix);
            }

            // assert(!(oldSuffix.empty() && newSuffix.empty()));
            if (newSuffix.empty())
            {
                newBranch->value = value;
            }
            else
            {
                newDir = newSuffix[0];
                newSuffix = chains::strip_first_nibble(newSuffix);
                attachLeafToBranch(newBranch, newDir, value, newSuffix);
            }

            currentNode = newExtension;
        }
    }
}

void PatriciaTrie::_node_extension_handler_v2(std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey, const std::string &value)
{
    auto extension = std::dynamic_pointer_cast<ExtensionNode>(currentNode);

    if (!extension)
    {
        return;
    }

    std::vector<uint8_t> nodeKey = extension->sharedNibble;
    auto nodeChild = std::dynamic_pointer_cast<Node>(extension->child);
    uint8_t oldDir;
    uint8_t newDir;
    std::vector<uint8_t> oldSuffix;
    std::vector<uint8_t> newSuffix;

    if (nodeKey == currentKey)
    {
        if (!nodeChild)
        {
            return;
        }

        insert_rec(nodeChild, {}, value);
        return;
    }

    std::vector<uint8_t> prefix = chains::get_prefix_hex(nodeKey, currentKey);

    if (prefix.size() == nodeKey.size())
    {
        newSuffix = chains::get_sufix(prefix, currentKey);
        insert_rec(nodeChild, newSuffix, value);
    }
    else if (prefix.size() == 0)
    {
        std::shared_ptr<BranchNode> newBranch = std::make_shared<BranchNode>();

        if (currentKey.empty())
        {
            newBranch->value = value;
        }
        else
        {
            newDir = currentKey[0]; // Y si currentKey es empty?
            newSuffix = chains::strip_first_nibble(currentKey);
            attachLeafToBranch(newBranch, newDir, value, newSuffix);
        }

        oldDir = nodeKey[0];
        oldSuffix = chains::strip_first_nibble(nodeKey);

        if (oldSuffix.empty())
        {
            newBranch->children[oldDir] = nodeChild;
        }
        else
        {
            std::shared_ptr<ExtensionNode> newExtension = std::make_shared<ExtensionNode>();
            newExtension->sharedNibble = oldSuffix;
            newExtension->child = nodeChild;
            newBranch->children[oldDir] = newExtension;
        }

        currentNode = newBranch;
    }
    else if (prefix.size() > 0 && prefix.size() < nodeKey.size())
    {
        std::shared_ptr<ExtensionNode> newExtension = std::make_shared<ExtensionNode>();
        std::shared_ptr<BranchNode> newBranch = std::make_shared<BranchNode>();
        newExtension->sharedNibble = prefix;
        newExtension->child = newBranch;

        oldSuffix = chains::get_sufix(prefix, nodeKey);
        newSuffix = chains::get_sufix(prefix, currentKey);

        if (newSuffix.empty())
        {
            newBranch->value = value;
        }
        else
        {
            newDir = newSuffix[0];
            newSuffix = chains::strip_first_nibble(newSuffix);
            attachLeafToBranch(newBranch, newDir, value, newSuffix);
        }

        oldDir = oldSuffix[0];
        oldSuffix = chains::strip_first_nibble(oldSuffix);

        if (oldSuffix.empty())
        {
            newBranch->children[oldDir] = nodeChild;
        }
        else
        {
            std::shared_ptr<ExtensionNode> aux = std::make_shared<ExtensionNode>();
            aux->sharedNibble = oldSuffix;
            aux->child = nodeChild;

            newBranch->children[oldDir] = aux;
        }

        currentNode = newExtension;
    }
}

void PatriciaTrie::printKeys()
{
    std::vector<std::vector<uint8_t>> keys = _getAllKeys(root);

    for (size_t i = 0; i < keys.size(); i++)
    {
        chains::print_hex_nibble_chain(keys[i]);
    }
}
std::vector<std::vector<uint8_t>> PatriciaTrie::_getAllKeys(const std::shared_ptr<Node> &currentNode)
{
    if (currentNode == nullptr)
    {
        return {};
    }

    if (auto leaf = std::dynamic_pointer_cast<LeafNode>(currentNode))
    {
        return {leaf->keyEnd};
    }

    if (auto branch = std::dynamic_pointer_cast<BranchNode>(currentNode))
    {
        std::vector<std::vector<uint8_t>> res;
        std::array<std::shared_ptr<Node>, 16> children = branch->children;

        for (size_t i = 0; i < children.size(); i++)
        {
            std::vector<std::vector<uint8_t>> childKeys = _getAllKeys(children[i]);

            if (!childKeys.empty())
            {
                for (int j = 0; j < childKeys.size(); j++)
                {
                    childKeys[j].insert(childKeys[j].begin(), static_cast<uint8_t>(i));
                    res.push_back(childKeys[j]);
                }
            }
        }

        return res;
    }

    if (auto extension = std::dynamic_pointer_cast<ExtensionNode>(currentNode))
    {
        std::vector<std::vector<uint8_t>> res;

        res = _getAllKeys(extension->child);
        std::vector<uint8_t> sharedNibble = extension->sharedNibble;

        if (res.empty())
        {
            return {sharedNibble};
        }
        else
        {
            for (size_t j = 0; j < res.size(); j++)
            {
                res[j].insert(res[j].begin(), sharedNibble.begin(), sharedNibble.end());
            }
        }

        return res;
    }

    return {};
}

bool PatriciaTrie::contains(const std::vector<uint8_t> &currentKey)
{
    return _contains_rec(root, currentKey);
}

bool PatriciaTrie::_contains_rec(const std::shared_ptr<Node> &currentNode, const std::vector<uint8_t> &currentKey)
{
    if (!currentNode)
    {
        return false;
    }

    if (auto leaf = std::dynamic_pointer_cast<LeafNode>(currentNode))
    {
        return leaf->keyEnd == currentKey;
    }
    else if (auto extension = std::dynamic_pointer_cast<ExtensionNode>(currentNode))
    {
        const std::vector<uint8_t> &sharedNibble = extension->sharedNibble;

        if (currentKey.size() < sharedNibble.size())
        {
            return false;
        }

        for (size_t i = 0; i < sharedNibble.size(); i++)
        {
            if (currentKey[i] != sharedNibble[i])
            {
                return false;
            }
        }

        std::vector<uint8_t> newSuffix = chains::get_sufix(sharedNibble, currentKey);
        return true && _contains_rec(extension->child, newSuffix);
    }
    else if (auto branch = std::dynamic_pointer_cast<BranchNode>(currentNode))
    {
        if (currentKey.empty())
        {
            return true;
        }

        uint8_t dir = currentKey[0];
        std::vector<uint8_t> newSuffix = chains::strip_first_nibble(currentKey);
        return true && _contains_rec(branch->children[dir], newSuffix);
    }

    return false;
}