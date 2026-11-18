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

struct QueueNode
{
    Node *treeNode;
    QueueNode *next;

    QueueNode(Node *node)
    {
        treeNode = node;
        next = nullptr;
    }
};

class Queue
{
private:
    QueueNode *frontNode;
    QueueNode *rearNode;
    int currentSize;

public:
    Queue()
    {
        frontNode = nullptr;
        rearNode = nullptr;
        currentSize = 0;
    }

    ~Queue()
    {
        while (!isEmpty())
        {
            dequeue();
        }
    }

    bool isEmpty()
    {
        return frontNode == nullptr;
    }

    int size()
    {
        return currentSize;
    }

    void enqueue(Node *node)
    {
        QueueNode *newNode = new QueueNode(node);
        if (rearNode == nullptr)
        {
            frontNode = rearNode = newNode;
        }
        else
        {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        currentSize++;
    }

    Node *dequeue()
    {
        if (isEmpty())
        {
            return nullptr;
        }

        QueueNode *temp = frontNode;
        Node *dataNode = temp->treeNode;

        frontNode = frontNode->next;
        if (frontNode == nullptr)
        {
            rearNode = nullptr;
        }

        delete temp;
        currentSize--;
        return dataNode;
    }

    Node *front()
    {
        if (isEmpty())
        {
            return nullptr;
        }
        return frontNode->treeNode;
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

int getBalance(Node *node)
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
    {
        return rightRotate(node);
    }

    if (balance < -1 && key > node->right->data)
    {
        return leftRotate(node);
    }

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

Node *minValueNode(Node *node)
{
    Node *current = node;
    while (current && current->left != nullptr)
    {
        current = current->left;
    }
    return current;
}

Node *deleteNode(Node *root, int key)
{
    if (root == nullptr)
    {
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
        return rightRotate(root);
    }

    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
    {
        return leftRotate(root);
    }

    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void printLevelOrder(Node *root)
{
    if (root == nullptr)
    {
        std::cout << "Tree is empty.\n";
        return;
    }

    Queue q;
    q.enqueue(root);
    int currentLevel = 1;

    std::cout << "--------------------------------------------------------\n";
    std::cout << "Breadth-First Level-Order Traversal:\n";
    std::cout << "--------------------------------------------------------\n";

    while (!q.isEmpty())
    {
        int levelNodeCount = q.size();
        std::cout << "Level " << currentLevel << ": ";

        for (int i = 0; i < levelNodeCount; i++)
        {
            Node *current = q.dequeue();
            int bf = getBalance(current);

            std::cout << "[" << current->data
                      << " | Height: " << current->height
                      << " | BF: " << bf << "]  ";

            if (current->left != nullptr)
            {
                q.enqueue(current->left);
            }
            if (current->right != nullptr)
            {
                q.enqueue(current->right);
            }
        }
        std::cout << "\n";
        currentLevel++;
    }
    std::cout << "--------------------------------------------------------\n";
}

bool verifyBalanceProperty(Node *root)
{
    if (root == nullptr)
    {
        return true;
    }
    int bf = getBalance(root);
    if (bf < -1 || bf > 1)
    {
        return false;
    }
    return verifyBalanceProperty(root->left) && verifyBalanceProperty(root->right);
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
    std::cout << "  Question 04: Queue Implementation & Level-Order BFS\n";
    std::cout << "========================================================\n\n";

    Node *root = nullptr;

    std::cout << ">>> Step 1: Performing 5 Insertions <<<\n";
    int insertSequence[5] = {30, 20, 40, 10, 25};
    for (int i = 0; i < 5; i++)
    {
        std::cout << "  Inserting: " << insertSequence[i] << "\n";
        root = insert(root, insertSequence[i]);
    }

    std::cout << "\nLevel-Order Traversal after 5 Insertions:\n";
    printLevelOrder(root);

    std::cout << "\nVisual Tree Structure:\n";
    printTreeSideways(root);
    std::cout << "\n--------------------------------------------------------\n\n";

    std::cout << ">>> Step 2: Performing 2 Deletions <<<\n";
    int deleteSequence[2] = {10, 40};
    for (int i = 0; i < 2; i++)
    {
        std::cout << "  Deleting node: " << deleteSequence[i] << "\n";
        root = deleteNode(root, deleteSequence[i]);
    }

    std::cout << "\nLevel-Order Traversal after 2 Deletions:\n";
    printLevelOrder(root);

    std::cout << "\nVisual Tree Structure:\n";
    printTreeSideways(root);

    std::cout << "\n========================================================\n";
    std::cout << "Verification Analysis:\n";
    bool isBalanced = verifyBalanceProperty(root);
    if (isBalanced)
    {
        std::cout << "  [SUCCESS] All nodes have |Height(Left) - Height(Right)| <= 1.\n";
        std::cout << "  The AVL balance invariant is strictly maintained after all operations!\n";
    }
    else
    {
        std::cout << "  [FAILED] Tree is not balanced.\n";
    }
    std::cout << "========================================================\n";

    return 0;
}
