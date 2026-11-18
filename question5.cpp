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

int getBalance(Node *root)
{
    if (root == nullptr)
    {
        return 0;
    }
    return getHeight(root->left) - getHeight(root->right);
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

Node *minValueNode(Node *node)
{
    Node *current = node;
    while (current && current->left != nullptr)
    {
        current = current->left;
    }
    return current;
}

Node *insert(Node *node, int key)
{
    if (node == nullptr)
    {
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
        return node;
    }

    node->height = 1 + getMax(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && key < node->left->data)
        return rightRotate(node);
    if (balance < -1 && key > node->right->data)
        return leftRotate(node);
    if (balance > 1 && key > node->left->data)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && key < node->right->data)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node *deleteNode(Node *root, int key)
{

    if (root == nullptr)
    {
        std::cout << "  Record with ID " << key << " not found in database.\n";
        return root;
    }

    if (key < root->data)
    {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->data)
    {
        root->right = deleteNode(root->right, key);
    }
    else
    {

        std::cout << "  Found record ID " << key << ". Performing standard deletion...\n";

        if (root->left == nullptr || root->right == nullptr)
        {
            Node *temp = root->left ? root->left : root->right;

            if (temp == nullptr)
            {

                temp = root;
                root = nullptr;
            }
            else
            {

                *root = *temp;
            }
            delete temp;
        }
        else
        {

            Node *temp = minValueNode(root->right);

            root->data = temp->data;

            root->right = deleteNode(root->right, temp->data);
        }
    }

    if (root == nullptr)
    {
        return root;
    }

    root->height = 1 + getMax(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
    {
        std::cout << "  [!] Deletion caused LL Imbalance at node (" << root->data
                  << ") | BF = " << balance << "\n";
        std::cout << "      -> Restoring AVL property via Right Rotation...\n";
        return rightRotate(root);
    }

    if (balance > 1 && getBalance(root->left) < 0)
    {
        std::cout << "  [!] Deletion caused LR Imbalance at node (" << root->data
                  << ") | BF = " << balance << "\n";
        std::cout << "      -> Step 1: Left Rotation on child (" << root->left->data << ")...\n";
        root->left = leftRotate(root->left);
        std::cout << "      -> Step 2: Right Rotation on parent (" << root->data << ")...\n";
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
    {
        std::cout << "  [!] Deletion caused RR Imbalance at node (" << root->data
                  << ") | BF = " << balance << "\n";
        std::cout << "      -> Restoring AVL property via Left Rotation...\n";
        return leftRotate(root);
    }

    if (balance < -1 && getBalance(root->right) > 0)
    {
        std::cout << "  [!] Deletion caused RL Imbalance at node (" << root->data
                  << ") | BF = " << balance << "\n";
        std::cout << "      -> Step 1: Right Rotation on child (" << root->right->data << ")...\n";
        root->right = rightRotate(root->right);
        std::cout << "      -> Step 2: Left Rotation on parent (" << root->data << ")...\n";
        return leftRotate(root);
    }

    return root;
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
    std::cout << "[" << root->data << " | BF:" << getBalance(root) << "]\n";
    printTreeSideways(root->left, space);
}

int main()
{
    std::cout << "========================================================\n";
    std::cout << "  Question 05: AVL Node Deletion with Four-Case Balancing\n";
    std::cout << "========================================================\n\n";

    int records[] = {9, 5, 10, 0, 6, 11, -1, 1, 2};
    int totalRecords = sizeof(records) / sizeof(records[0]);

    Node *root = nullptr;
    std::cout << "Initial Database Record Insertion:\n";
    for (int i = 0; i < totalRecords; i++)
    {
        root = insert(root, records[i]);
    }

    std::cout << "Initial AVL-Indexed Database Tree:\n";
    std::cout << "Inorder: ";
    printInorder(root);
    std::cout << "\nVisual Structure (Data | Balance Factor):";
    printTreeSideways(root);
    std::cout << "\n--------------------------------------------------------\n\n";

    std::cout << ">>> Action 1: Administrator deletes old record 10 <<<\n";
    root = deleteNode(root, 10);

    std::cout << "\nTree State After Deleting 10:\n";
    std::cout << "Inorder: ";
    printInorder(root);
    std::cout << "\nVisual Structure:";
    printTreeSideways(root);
    std::cout << "\n--------------------------------------------------------\n\n";

    std::cout << ">>> Action 2: Administrator deletes old record 11 <<<\n";
    root = deleteNode(root, 11);

    std::cout << "\nTree State After Deleting 11:\n";
    std::cout << "Inorder: ";
    printInorder(root);
    std::cout << "\nVisual Structure:";
    printTreeSideways(root);
    std::cout << "\n--------------------------------------------------------\n\n";

    std::cout << ">>> Action 3: Administrator deletes old record 1 (Two Children) <<<\n";
    root = deleteNode(root, 1);

    std::cout << "\nTree State After Deleting 1:\n";
    std::cout << "Inorder: ";
    printInorder(root);
    std::cout << "\nVisual Structure:";
    printTreeSideways(root);

    std::cout << "\n========================================================\n";
    std::cout << "Summary of Results:\n";
    std::cout << "  - Standard BST delete executed cleanly for all cases.\n";
    std::cout << "  - Height and getBalance(root) updated at each step.\n";
    std::cout << "  - Automatic rebalancing restored AVL property across all operations.\n";
    std::cout << "========================================================\n";

    return 0;
}
