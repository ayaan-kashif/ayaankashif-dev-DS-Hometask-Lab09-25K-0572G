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
        std::cout << "  -> Inserted sensor reading: " << key << "\n";
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

        std::cout << "  -> Duplicate reading " << key << " ignored.\n";
        return node;
    }

    node->height = 1 + getMax(getHeight(node->left), getHeight(node->right));

    int balance = getBalanceFactor(node);

    if (balance > 1 && key < node->left->data)
    {
        std::cout << "  [!] Detected Left-Left (LL) imbalance at node (" << node->data
                  << ") with Balance Factor = " << balance << "\n";
        std::cout << "      -> Performing Right Rotation on node (" << node->data << ")...\n";
        return rightRotate(node);
    }

    if (balance < -1 && key > node->right->data)
    {
        std::cout << "  [!] Detected Right-Right (RR) imbalance at node (" << node->data
                  << ") with Balance Factor = " << balance << "\n";
        std::cout << "      -> Performing Left Rotation on node (" << node->data << ")...\n";
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
    std::cout << "  Question 02: Temperature Sensor AVL Tree Rotations\n";
    std::cout << "========================================================\n\n";

    std::cout << ">>> TEST 1: Inserting strictly increasing sequence {30, 35, 45} <<<\n";
    int seq1[] = {30, 35, 45};
    Node *root1 = nullptr;

    for (int i = 0; i < 3; i++)
    {
        std::cout << "Inserting " << seq1[i] << "...\n";
        root1 = insert(root1, seq1[i]);
    }

    std::cout << "\nTest 1 Resulting Tree:\n";
    std::cout << "  - Final Root Node: " << root1->data << "\n";
    std::cout << "  - Root Height: " << root1->height << "\n";
    std::cout << "  - Inorder Traversal: ";
    printInorder(root1);
    std::cout << "\n  - Tree Visual Structure:";
    printTreeSideways(root1);
    std::cout << "\n--------------------------------------------------------\n\n";

    std::cout << ">>> TEST 2: Inserting strictly decreasing sequence {13, 10, 5} <<<\n";
    int seq2[] = {13, 10, 5};
    Node *root2 = nullptr;

    for (int i = 0; i < 3; i++)
    {
        std::cout << "Inserting " << seq2[i] << "...\n";
        root2 = insert(root2, seq2[i]);
    }

    std::cout << "\nTest 2 Resulting Tree:\n";
    std::cout << "  - Final Root Node: " << root2->data << "\n";
    std::cout << "  - Root Height: " << root2->height << "\n";
    std::cout << "  - Inorder Traversal: ";
    printInorder(root2);
    std::cout << "\n  - Tree Visual Structure:";
    printTreeSideways(root2);

    std::cout << "\n========================================================\n";
    std::cout << "Verification Summary:\n";
    std::cout << "  Sequence {30, 35, 45} triggered RR imbalance -> Left Rotate -> Root is " << root1->data << " (Expected: 35)\n";
    std::cout << "  Sequence {13, 10, 5}  triggered LL imbalance -> Right Rotate -> Root is " << root2->data << " (Expected: 10)\n";
    std::cout << "========================================================\n";

    return 0;
}
