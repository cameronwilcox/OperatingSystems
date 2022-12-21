#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

namespace thdlst
{
class Node
{
public:
    Node *mNext;
    Node *mPrev;
    int mData;

};

int counter = 0; // keep track of list length

int getData(Node** head) // return data of node
{
    return (*head)->mData;
}

int turn = 0;
bool running = true;

void insert(Node **head, int new_data)// insertion algorithm, adds at the end of the list
{
    counter++;
    Node *node = new Node;
    Node *last = *head;
    node->mData = new_data;
    node->mNext = NULL;
    if (*head == NULL)
    {
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


void remove(Node **head) // removal algorithm. removes from front of list
{
    if (counter == 1)
    {
        (*head)->mNext = NULL;
        (*head)->mPrev = NULL;
        *head = NULL;
    }
    else if(counter != 0) // do nothing in the event
    {
        Node *tmp = *head;
        (*head) = (*head)->mNext;
        (*head)->mPrev = NULL;
        tmp = NULL;
    }
    counter--;
}

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;


void *evenAdd(void *head) // add even numbers
{
    while(running)
    {
        pthread_mutex_lock(&mutex1);
        int even_random = 0 + rand()% (50 + 1);
        while ((even_random % 2) != 0)
        {
            even_random = 0 + rand()% (50 + 1);
        }

        if (counter < 10)
        {
            std::cout << "Even Adder is in\n";
            insert((Node**)head, even_random);
            displayList(*(Node**)(head));
        }

        pthread_mutex_unlock(&mutex1);
        sleep(.5);
    }
}

void *oddAdd(void *head) // add odd numbers
{
    while(running)
    {
        pthread_mutex_lock(&mutex1);
        int odd_random = 0 + rand()% (50 + 1);
        while ((odd_random % 2) == 0)
        {
            odd_random = 0 + rand()% (50 + 1);
        }

        if (counter < 10)
        {
            std::cout << "Odd Adder is in\n";
            insert((Node**)head, odd_random);
            displayList(*(Node**)(head));
        }

        pthread_mutex_unlock(&mutex1);
        sleep(.5);
    }
}

void *evenRemove(void *head) // remove even numbers
{
    while(running)
    {
        pthread_mutex_lock(&mutex1);
        
        std::cout << counter << '\n';
        if (counter > 0)
        {
            std::cout << "Even Remover is in\n";
            if(getData((Node**)head)%2 == 0)
            {
                remove((Node**)head);
            }
            displayList(*(Node**)(head));
        }

        pthread_mutex_unlock(&mutex1);
        sleep(.5);
    }
}

void *oddRemove(void *head) // remove odd numbers
{
    while(running)
    {
        pthread_mutex_lock(&mutex1);

        std::cout << counter << '\n';

        if (counter > 0)
        {
            std::cout << "Odd Remover is in\n";
            if(getData((Node**)head)%2 != 0)
            {
                remove((Node**)head);
            }
            displayList(*(Node**)(head));
        }

        pthread_mutex_unlock(&mutex1);
        sleep(.5);
    }
}

}

int main()
{

    thdlst::Node *head = NULL; // initialize list

    thdlst::insert(&head, 1);
    thdlst::insert(&head, 2);
    thdlst::insert(&head, 3);


    // create threads
    pthread_t thread1, thread2, thread3, thread4;
    auto id1 = pthread_create(&thread1, NULL, &thdlst::oddAdd, &head);
    if (id1 != 0)
    {
        std::cout << "Failed 1\n";
    }
    auto id2 = pthread_create(&thread2, NULL, &thdlst::evenAdd, &head);
    if (id2 != 0)
    {
        std::cout << "Failed 2\n";
    }
    auto id3 = pthread_create(&thread3, NULL, &thdlst::oddRemove, &head);
    if (id3 != 0)
    {
        std::cout << "Failed 3\n";
    }
    auto id4 = pthread_create(&thread4, NULL, &thdlst::evenRemove, &head);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    return 0;

}
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
