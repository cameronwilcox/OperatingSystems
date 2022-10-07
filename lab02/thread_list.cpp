#include <iostream>
#include <pthread.h>

namespace thdlst
{
class Node
{
public:
    Node *mNext;
    Node *mPrev;
    int mData;

};

int counter = 0;
int getCounter()
{
    return counter;
}

int turn = 0;
bool running = true;

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
    Node* last;
  
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


void remove(Node **head)
{
    if (counter == 1)
    {
        (*head)->mNext = NULL;
        (*head)->mPrev = NULL;
        *head = NULL;
    }
    else
    {
        Node *tmp = *head;
        (*head) = (*head)->mNext;
        (*head)->mPrev = NULL;
        tmp = NULL;
    }
    counter--;
}

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;


void *evenAdd(void *head)
{
    while(running)
    {
        pthread_mutex_lock(&mutex1);

        if (counter < 50 && turn == 0)
        {
            counter += 2;
            if (counter >= 50)
            {
                turn = 1;
                std::cout << counter << '\n';
            }
        }

        pthread_mutex_unlock(&mutex1);
    }
}

void *oddAdd(void *head)
{
    while(running)
    {
        pthread_mutex_lock(&mutex1);

        if (counter < 50 && turn == 0)
        {
            counter++;
            if (counter >= 50)
            {
                turn = 1;
                std::cout << counter << '\n';
            }
        }

        pthread_mutex_unlock(&mutex1);
    }
}

void *evenRemove(void *head)
{
    while(running)
    {
        pthread_mutex_lock(&mutex1);

        if (counter > 0 && turn == 1)
        {
            counter -= 2;
            if (counter <= 0)
            {
                turn = 0;
                std::cout << counter << '\n';
            }
        }

        pthread_mutex_unlock(&mutex1);
    }
}

void *oddRemove(void *head)
{
    std::cout << "In remove\n";
    while(running)
    {
        pthread_mutex_lock(&mutex1);

        if (counter > 0 && turn == 1)
        {
            counter--;
            if (counter <= 0)
            {
                turn = 0;
                std::cout << counter << '\n';
            }
        }

        pthread_mutex_unlock(&mutex1);
    }
}




}

int main()
{

    thdlst::Node *head = NULL;
    // thdlst::insert(&head, 3);
    // thdlst::insert(&head, 6);
    // thdlst::insert(&head, 10);
    // thdlst::remove(&head);

    // thdlst::displayList(head);

    pthread_t thread1, thread2;
    auto id1 = pthread_create(&thread1, NULL, thdlst::oddAdd, &head);
    auto id2 = pthread_create(&thread2, NULL, thdlst::oddRemove, &head);

    return 0;

}