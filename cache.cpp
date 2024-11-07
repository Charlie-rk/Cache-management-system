#include <bits/stdc++.h>
using namespace std;

struct cache_Block
{
    string state;
    string tag;
    vector<int> data;
    cache_Block()
    {
        state = "I";
        data.resize(4, -1);
    }
};

int MEMORY_BLOCK = 16834;
int CACHE_BLOCK = 512, N = 512;
int WAYS = 8, W = 8;
int CACHE_SET = 64, S = 64;
int BLOCK_SIZE = 64, B = 64;

// Initialize main memory (MM) as a 2D vector with default values of -1
vector<vector<int>> MM(16834, vector<int>(4, -1));
// Initialize cache as a 3D vector with 64 sets, 8 ways, and cache_Block objects
vector<vector<cache_Block>> Cache(64, vector<cache_Block>(8));

void init_MainMemory()
{
    // Main Memory
    // no of block M/B= 1MB/64B=16384
    for (int i = 0; i < MEMORY_BLOCK; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            MM[i][j] = rand() % 100 + 1; // 1-100
        }
    }
}

class Cache_helper
{
public:
    int Miss_Hit(int Address, string type, int write_data) // type read or write
    {
        int A = Address;
        int BA = A / B; // for memory access
        if (BA >= 16834){
            cout << "Error: Address out of bounds!" << endl;
            return -1;
        }
            
        int index = BA % S;   // for cache access  this is set index
        int tag_INT = BA / S; // for cache tag
        bitset<32> bitset(tag_INT);
        string tag = bitset.to_string();
        int BO = A % 4;               // exact location  for data
                                     
        auto &set_way = Cache[index]; // get particular set
        for (auto &B : set_way)
        { // search in set
            if (B.state == "V" && B.tag == tag)
            {  
                cout<<endl;
                cout<<"🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯🎯"<<endl;
                cout << "Cache Hit: Data = " << B.data[BO] << endl;

                if (type == "WRITE")
                {
                    // update the cache
                    B.data[BO] = write_data;
                    // update the Main Memory
                    MM[BA][BO] = write_data;
                    cout << "Write Hit - Updated Cache and Main Memory" << endl;
                }
                return B.data[BO]; // return element
            }
            if (B.state == "MP" && B.tag == tag)
            { // not encounter here as per code
                cout << "Wait Request " << endl;
                return -1; // empty for wait
            }
        }

        cout << "Cache Miss - Fetching from Main Memory" << endl;

         // evication random s
        
        int evication_way = rand() % W;      // get random index between 0 and 8 for evication or target block within set
        set_way[evication_way].tag = tag;    // update the tag
        set_way[evication_way].state = "MP"; // For the time being, update the state

        /* 
        point of using thread for that so that at this point the program change their context 
         */

        // UPDATE befor fetching the data
        if (type == "WRITE")
        {
            MM[BA][BO] = write_data;
            cout << "Write Request: Updated Main Memory directly" << endl;
        }
        auto MemReq = MM[BA]; // requesting from memory
        set_way[evication_way].data = MemReq;
        set_way[evication_way].state = "V";

        cout << "Fetched from Memory: Data = " << set_way[evication_way].data[BO] << endl;
        return set_way[evication_way].data[BO];
    }
};


void test_Run()
{
    int count = 0;
    Cache_helper ch;
    while (count <16834)
    {
        cout << "--------------------------------------------------------------------------------------------------------------------------" << endl;
        int A = rand();
       
        string type = (rand() % 2 == 0) ? "read" : "WRITE";
        cout << "Access Type: " << type << ", Address: " << A;
        int write_data = (type == "WRITE") ? rand() % 100 + 1 : 0;
         if (type == "WRITE") cout << ", Data to Write: " << write_data<<endl;
         else cout<<endl;
        ch.Miss_Hit(A, type, write_data);
        count++;

        cout << endl;
        cout << "==========================================================================================================================" << endl;
        cout << endl;

    }
    cout << " `````````````````````````````````````````````````````````````````````````````````````````````" << endl;
    cout << "                                  CATCH TEST FINISHED                                         " << endl;
    cout << " `````````````````````````````````````````````````````````````````````````````````````````````" << endl;
}

void print_MainMemory()
{
    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            cout << MM[i][j] << "  ";
        }
        cout << endl;
    }
}

int main()
{
    freopen("output.txt", "w", stdout);
    srand(time(0));
    cout << " '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''" << endl;
    cout << "                            Initializing Main Memory and Cache...                               " << endl;
    cout<<".................................................................................................. "<<endl;
    cout<<endl;
    cout<<endl;
    cout<<endl;
    init_MainMemory();
    // print_MainMemory();
    test_Run();
    return 0;
}