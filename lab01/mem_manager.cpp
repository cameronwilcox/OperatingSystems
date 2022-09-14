#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <tuple>
#include <queue>

const std::string WHITESPACE = " \n\r\t\f\v";

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
    int mMemSize         = 0;       
    int mNumBlocksNeeded = -99;
    int mID              = -99;
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
                    begin_contig_index = i; // track the beginning of the free slots
                }
                contiguous_free++;
                if (contiguous_free >= job.mNumBlocksNeeded) // if we have reached the number of free blocks we need
                {
                    break;
                }
            }
            else
            {
                contiguous_free = 0; // reset if not the case
            }
        }

        if (contiguous_free >= job.mNumBlocksNeeded) // insert block
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
        else // if there are not enough free blocks we need to remove some
        {
            auto to_be_removed = mLeastRecent.front();
            begin_contig_index = 0;
            contiguous_free = 0;
            remove_job(to_be_removed);
        }
    }
}

void remove_job(Job job) // removes jobs, works on a first in first out basis
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

// ltrim, rtrim, and trim are code snippets I pulled from the internet to help with 
// trimming strings down. Not necessary to the logic of the program
std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
	return rtrim(ltrim(s));
}

void handle_insert(MemManager::MemorySystem &system, MemManager::Job &job)
{
    system.add_job(std::move(job));
}

void print_options()
{
    std::cout << "Type 'print' to print system memory | To enter a job type <ID> <Memory_Size> | To quit type 'quit'\n";
    std::cout << ">>";
}

void repl()
{
    MemManager::MemorySystem system;
    bool running = true;
    std::string input;
    while (running)
    {
        print_options();
        getline(std::cin, input);
        input = trim(input);
        if(input == "quit")
        {
            exit(EXIT_SUCCESS);
        }
        else if(input == "print")
        {
            system.print_memory();
        }
        else
        {
            try
            {
                int split = input.find(" ");
                int ID = std::stoi(input.substr(0,split));
                int size = std::stoi(input.substr(split));
                MemManager::Job temp(size, ID);
                handle_insert(system, temp);
                
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
        }
        
        

    }
    
}

int main()
{
    repl();
    return 0;
}