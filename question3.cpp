#include <iostream>
#include <string>

struct Node
{
    int data;
    Node *left;
    Node *right;
    int height;

    Node(int val)
    {
        data = val;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

int getHeight(Node *node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return node->height;
}

int getMax(int a, int b)
{
    return (a > b) ? a : b;
}

int getBalanceFactor(Node *node)
{
    if (node == nullptr)
    {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

Node *rightRotate(Node *y)
{
    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = getMax(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = getMax(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node *leftRotate(Node *x)
{
    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = getMax(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = getMax(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

Node *insert(Node *node, int key)
{

    if (node == nullptr)
    {
        std::cout << "  Inserted node with key: " << key << "\n";
        return new Node(key);
    }

    if (key < node->data)
    {
        node->left = insert(node->left, key);
    }
    else if (key > node->data)
    {
        node->right = insert(node->right, key);
    }
    else
    {
        std::cout << "  Key " << key << " already exists in dictionary.\n";
        return node;
    }

    node->height = 1 + getMax(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);

    if (balance > 1 && key < node->left->data)
    {
        std::cout << "  [LL Imbalance] Single Right Rotation on node " << node->data << "\n";
        return rightRotate(node);
    }

    if (balance < -1 && key > node->right->data)
    {
        std::cout << "  [RR Imbalance] Single Left Rotation on node " << node->data << "\n";
        return leftRotate(node);
    }

    if (balance > 1 && key > node->left->data)
    {
        std::cout << "\n  [!] Double Imbalance Detected: Left-Right (LR) at node (" << node->data << ")\n";
        std::cout << "      Step 1: Perform Left Rotation on child node (" << node->left->data << ")...\n";
        node->left = leftRotate(node->left);

        std::cout << "      Step 2: Perform Right Rotation on original unbalanced node (" << node->data << ")...\n";
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->data)
    {
        std::cout << "\n  [!] Double Imbalance Detected: Right-Left (RL) at node (" << node->data << ")\n";
        std::cout << "      Step 1: Perform Right Rotation on child node (" << node->right->data << ")...\n";
        node->right = rightRotate(node->right);

        std::cout << "      Step 2: Perform Left Rotation on original unbalanced node (" << node->data << ")...\n";
        return leftRotate(node);
    }

    return node;
}

void printInorder(Node *root)
{
    if (root == nullptr)
    {
        return;
    }
    printInorder(root->left);
    std::cout << root->data << " ";
    printInorder(root->right);
}

void printTreeSideways(Node *root, int space = 0)
{
    if (root == nullptr)
    {
        return;
    }
    space += 6;
    printTreeSideways(root->right, space);
    std::cout << "\n";
    for (int i = 6; i < space; i++)
    {
        std::cout << " ";
    }
    std::cout << "[" << root->data << "]\n";
    printTreeSideways(root->left, space);
}

int main()
{
    std::cout << "========================================================\n";
    std::cout << "  Question 03: AVL Tree Double Imbalances (LR & RL)\n";
    std::cout << "========================================================\n\n";

    std::cout << ">>> REQUIRED TEST: Inserting Sequence {13, 10, 11} (LR Double Rotation) <<<\n";
    int sequenceLR[] = {13, 10, 11};
    Node *rootLR = nullptr;

    for (int i = 0; i < 3; i++)
    {
        std::cout << "\n[Action] Inserting " << sequenceLR[i] << " into tree:\n";
        rootLR = insert(rootLR, sequenceLR[i]);
    }

    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "Verification of Sequence {13, 10, 11}:\n";
    std::cout << "  - Final Root Data : " << rootLR->data << "\n";
    std::cout << "  - Expected Root   : 11\n";
    if (rootLR->data == 11)
    {
        std::cout << "  - Result          : SUCCESS (Root is verified to be 11!)\n";
    }
    else
    {
        std::cout << "  - Result          : FAILED\n";
    }
    std::cout << "  - Inorder Traversal: ";
    printInorder(rootLR);
    std::cout << "\n  - Tree Visual Structure:";
    printTreeSideways(rootLR);
    std::cout << "\n--------------------------------------------------------\n\n";

    std::cout << ">>> ADDITIONAL TEST: Inserting Sequence {10, 15, 12} (RL Double Rotation) <<<\n";
    int sequenceRL[] = {10, 15, 12};
    Node *rootRL = nullptr;

    for (int i = 0; i < 3; i++)
    {
        std::cout << "\n[Action] Inserting " << sequenceRL[i] << " into tree:\n";
        rootRL = insert(rootRL, sequenceRL[i]);
    }

    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "Verification of Sequence {10, 15, 12}:\n";
    std::cout << "  - Final Root Data : " << rootRL->data << "\n";
    std::cout << "  - Expected Root   : 12\n";
    if (rootRL->data == 12)
    {
        std::cout << "  - Result          : SUCCESS (Root is verified to be 12!)\n";
    }
    else
    {
        std::cout << "  - Result          : FAILED\n";
    }
    std::cout << "  - Inorder Traversal: ";
    printInorder(rootRL);
    std::cout << "\n  - Tree Visual Structure:";
    printTreeSideways(rootRL);

    std::cout << "\n========================================================\n";
    std::cout << "All double rotation tests completed successfully.\n";
    std::cout << "========================================================\n";

    return 0;
}
