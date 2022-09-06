#include <iostream>
#include <vector>
#include <queue>

namespace MemManager
{

// job class to track how much memory and blocks will be needed
struct Job
{
public:
    Job(int mem_size, int id) : mMemSize(mem_size), mID(id)
    {
        //initialize blocks needed
        mNumBlocksNeeded = mem_size / 4096;
        // account for extra space
        if (mem_size % 4096 > 0)
        {
            mNumBlocksNeeded++;
        }
    }
    int mMemSize = 0;
    int mNumBlocksNeeded = -99;
    int mID = -99;
};


//Memory system to track jobs
class MemorySystem
{
public:

void add_job(Job job)
{
    bool inserted = false;
    int contiguous_free = 0;
    int begin_contig_index = 0;
    while (!inserted) //while the job has yet to be insterted
    {
        for (int i = 0; i < 16; i++) //loop over memory slots
        {
            if(mMemory[i] == 0) // if there is no job at the slot
            {
                if (contiguous_free == 0) // if this is the first free memory slot after a full slot
                {
                    begin_contig_index = i;
                }
                contiguous_free++;
            }
            else
            {
                contiguous_free = 0;
            }
        }

        if (contiguous_free >= job.mNumBlocksNeeded)
        {
            inserted = true;
            for (int i = begin_contig_index; i < job.mNumBlocksNeeded + begin_contig_index; i++)
            {
                mMemory[i] = job.mID;
            }
        }
    }
    
}

void remove_job()
{
    
}


void print_memory()
{
    for(int i = 0; i < 16; i++)
    {
        std::cout << "Block: " << i << "    PID: " << mMemory[i] << '\n';
    }
}



private:

std::queue<Job>  mLeastRecent;          //Tracks what job has been in the longest
int mMemory[16] = {};                        //Track what jobs are in what slot based on ID
int              mIDLastUsed = -99;

};



};




void repl()
{

}

int main()
{
    MemManager::Job job1(8000, 1);
    MemManager::Job job2(8000, 2);

    MemManager::MemorySystem system;

    system.add_job(job1);
    system.add_job(job2);

    system.print_memory();

    return 0;
}