#include <iostream>
#include <memory>

namespace thdlst
{
class Node
{
public:
    Node *mNext;
    Node *mPrev;
    int mData;

};



void insert(Node **head, int new_data)
{
    std::cout << "In insert\n";
    Node *node = new Node;
    Node *last = *head;
    std::cout << "Trying to set data\n";
    node->mData = new_data;
    std::cout << "After setting data\n";
    node->mNext = NULL;
    if (*head == NULL)
    {
        std::cout << "head is null\n";
        node->mPrev = NULL;
        *head = node;
        return;
    }   
    while (last->mNext != NULL)
    {
        last = last->mNext;
    }
    last->mNext = node;
    node->mPrev = last;

}

void displayList(Node* node) {
    struct Node* last;
  
    while (node != NULL) {
        std::cout<<node->mData<<"<==>";
        last = node;
        node = node->mNext;
    }
    if(node == NULL)
    {
        std::cout<<"NULL\n";
    }

}




}

int main()
{

    thdlst::Node *head = NULL;
    thdlst::insert(&head, 3);
    thdlst::insert(&head, 6);
    thdlst::insert(&head, 10);

    thdlst::displayList(head);
    return 0;

}