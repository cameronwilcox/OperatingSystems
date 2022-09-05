#include <iostream>
#include <vector>
#include <queue>

namespace MemManager
{

struct Job
{
public:
    Job(int mem_size, int id) : mMemSize(mem_size), mID(id)
    {}
    int mMemSize = 0;
    int mID = -99;
};



class MemorySystem
{
public:

void add_job(Job job)
{

}

void remove_job()
{

}



private:

std::queue<Job>  mInsertQueue;
std::vector<int> mMemory;
int              mIDLastUsed = -99;

};



};




void repl()
{

}

int main()
{
    return 0;
}