#ifndef NODE
#define NODE

enum NodeType
{
    LEAF,
    EXTENSION,
    BRANCH
};

class Node
{
public:
    virtual ~Node() = default;

    NodeType type;
};

#endif