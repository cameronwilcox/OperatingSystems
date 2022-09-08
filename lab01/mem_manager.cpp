#include <iostream>
#include <vector>
#include <array>
#include <tuple>
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
    if (job.mNumBlocksNeeded > 16)
    {
        std::cerr << "Too big of a job\n";
        return;
    }
    bool inserted = false;
    int contiguous_free = 0;
    int begin_contig_index = 0;
    while (!inserted) //while the job has yet to be insterted
    {
        for (int i = 0; i < 16; i++) //loop over memory slots
        {
            if(std::get<0>(mMemory[i]) == 0) // if there is no job at the slot
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
                if (job.mMemSize % 4096 > 0 && i == (job.mNumBlocksNeeded + begin_contig_index - 1))
                {
                    mMemory[i] = std::make_tuple(job.mID, job.mMemSize % 4096);
                }
                else
                {
                    mMemory[i] = std::make_tuple(job.mID, 4096);
                }
                
            }
            mLeastRecent.push(job);
        }
        else
        {
            auto to_be_removed = mLeastRecent.front();
            begin_contig_index = 0;
            contiguous_free = 0;
            remove_job(to_be_removed);
        }
    }
}

void remove_job(Job job)
{
    bool removed = false;
    for (auto &block : mMemory)
    {
        if (std::get<0>(block) == job.mID)
        {
            block = {0,0};
            removed = true;
        }
    }
    if (removed)
    {
        mLeastRecent.pop();
    }
}


void print_memory()
{
    for(int i = 0; i < 16; i++)
    {
        std::cout << "Block: " << i << "    PID: " << std::get<0>(mMemory[i]) << "      Size: " << std::get<1>(mMemory[i])<< '\n';
    }
}



private:

std::queue<Job>                      mLeastRecent;                                          //Tracks what job has been in the longest
std::array<std::tuple<int, int>, 16> mMemory = {};                      //Track what jobs are in what slot based on ID

};



};




void repl()
{

}

int main()
{
    MemManager::Job job1(8000, 1);
    MemManager::Job job2(8000, 2);
    MemManager::Job job3(65535, 3);

    MemManager::MemorySystem system;

    system.add_job(job1);
    system.add_job(job2);

    std::cout << "Before: \n";
    system.print_memory();

    system.add_job(job3);

    std::cout << "After: \n";
    system.print_memory();

    // system.remove_job(job1);

    // std::cout << "After: \n";

    // system.print_memory();

    return 0;
}