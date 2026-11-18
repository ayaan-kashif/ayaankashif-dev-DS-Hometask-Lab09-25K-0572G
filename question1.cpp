#include <iostream>
#include <string>

struct Node
{
    int data;
    Node *left;
    Node *right;

    Node(int val)
    {
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

struct ListNode
{
    int val;
    ListNode *next;

    ListNode(int v)
    {
        val = v;
        next = nullptr;
    }
};

Node *insertBST(Node *root, int val)
{
    if (root == nullptr)
    {
        return new Node(val);
    }
    if (val < root->data)
    {
        root->left = insertBST(root->left, val);
    }
    else if (val > root->data)
    {
        root->right = insertBST(root->right, val);
    }
    return root;
}

int getHeight(Node *root)
{
    if (root == nullptr)
    {
        return 0;
    }
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

void storeInorder(Node *root, ListNode *&head, ListNode *&tail)
{
    if (root == nullptr)
    {
        return;
    }

    storeInorder(root->left, head, tail);

    ListNode *newNode = new ListNode(root->data);
    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }

    storeInorder(root->right, head, tail);
}

Node *buildBalancedTree(ListNode *head)
{
    if (head == nullptr)
    {
        return nullptr;
    }

    if (head->next == nullptr)
    {
        return new Node(head->val);
    }

    ListNode *prev = nullptr;
    ListNode *slow = head;
    ListNode *fast = head;

    while (fast != nullptr && fast->next != nullptr)
    {
        fast = fast->next->next;
        prev = slow;
        slow = slow->next;
    }

    if (prev != nullptr)
    {
        prev->next = nullptr;
    }

    Node *root = new Node(slow->val);

    root->left = buildBalancedTree(head);

    root->right = buildBalancedTree(slow->next);

    return root;
}

Node *balanceExistingBST(Node *root)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    ListNode *head = nullptr;
    ListNode *tail = nullptr;

    storeInorder(root, head, tail);

    return buildBalancedTree(head);
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
    std::cout << "  Question 01: Balancing a Skewed BST Using Linked List\n";
    std::cout << "========================================================\n\n";

    int userIDs[] = {101, 102, 103, 104, 105, 106, 107};
    int n = sizeof(userIDs) / sizeof(userIDs[0]);

    Node *skewedRoot = nullptr;
    for (int i = 0; i < n; i++)
    {
        skewedRoot = insertBST(skewedRoot, userIDs[i]);
    }

    std::cout << "Original Legacy Database BST (Skewed):\n";
    std::cout << "Inorder Traversal: ";
    printInorder(skewedRoot);
    std::cout << "\n";

    int heightBefore = getHeight(skewedRoot);
    std::cout << "Height Before Balancing: " << heightBefore << " (Inefficient O(N) search)\n";

    std::cout << "\n--- Visualizing Skewed Tree Structure ---";
    printTreeSideways(skewedRoot);

    std::cout << "\n--------------------------------------------------------\n";
    std::cout << "Executing balanceExistingBST(skewedRoot)...\n";
    Node *balancedRoot = balanceExistingBST(skewedRoot);
    std::cout << "Tree successfully balanced!\n";
    std::cout << "--------------------------------------------------------\n\n";

    std::cout << "Balanced BST:\n";
    std::cout << "Inorder Traversal: ";
    printInorder(balancedRoot);
    std::cout << "\n";

    int heightAfter = getHeight(balancedRoot);
    std::cout << "Height After Balancing: " << heightAfter << " (Optimal O(log N) search)\n";

    std::cout << "\n--- Visualizing Balanced Tree Structure ---";
    printTreeSideways(balancedRoot);

    std::cout << "\n========================================================\n";
    std::cout << "Summary of Results:\n";
    std::cout << "  - Initial Height : " << heightBefore << "\n";
    std::cout << "  - Balanced Height: " << heightAfter << "\n";
    std::cout << "  - Height Reduction: " << (heightBefore - heightAfter) << " levels saved!\n";
    std::cout << "========================================================\n";

    return 0;
}
