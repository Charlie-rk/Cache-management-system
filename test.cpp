#include <bits/stdc++.h>
using namespace std;

struct CacheBlock {
    string state;
    string tag;
    vector<int> data;
    CacheBlock() : state("I"), data(4, -1) {}
};

// Global Constants and Variables
const int MEMORY_BLOCKS = 100;
const int CACHE_SETS = 64;
const int WAYS = 8;
const int BLOCK_SIZE = 4;
vector<vector<int>> MainMemory(MEMORY_BLOCKS, vector<int>(BLOCK_SIZE, -1));
vector<vector<CacheBlock>> Cache(CACHE_SETS, vector<CacheBlock>(WAYS));

void initMainMemory() {
    for (int i = 0; i < MEMORY_BLOCKS; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            MainMemory[i][j] = rand() % 100 + 1; // Random values between 1-100
        }
    }
}

void initCache() {
    for (int i = 0; i < CACHE_SETS; i++) {
        for (int j = 0; j < WAYS; j++) {
            Cache[i][j] = CacheBlock(); // Initialize each block with default values
        }
    }
}

class CacheHelper {
public:
    int accessCache(int address, const string& type, int writeData = 0) {
        int blockAddress = address / BLOCK_SIZE;
        if (blockAddress >= MEMORY_BLOCKS) {
            cerr << "Error: Address out of bounds!" << endl;
            return -1;
        }

        int setIndex = blockAddress % CACHE_SETS;
        int tagInt = blockAddress / CACHE_SETS;
        string tag = bitset<32>(tagInt).to_string();
        int blockOffset = address % BLOCK_SIZE;

        auto &setWay = Cache[setIndex];
        for (auto &block : setWay) {
            if (block.state == "V" && block.tag == tag) {
                cout << "Cache Hit: Data = " << block.data[blockOffset] << endl;
                if (type == "WRITE") {
                    block.data[blockOffset] = writeData;
                    MainMemory[blockAddress][blockOffset] = writeData;
                    cout << "Write Hit - Updated Cache and Main Memory" << endl;
                }
                return block.data[blockOffset];
            }
        }

        return handleMiss(setIndex, tag, blockAddress, blockOffset, type, writeData);
    }

private:
    int handleMiss(int setIndex, const string& tag, int blockAddress, int blockOffset, const string& type, int writeData) {
        cout << "Cache Miss - Fetching from Main Memory" << endl;

        int evictionWay = rand() % WAYS;
        CacheBlock &evictionBlock = Cache[setIndex][evictionWay];

        evictionBlock.tag = tag;
        evictionBlock.state = "MP";  // Set state to "Miss Pending" during fetch

        if (type == "WRITE") {
            MainMemory[blockAddress][blockOffset] = writeData;
            cout << "Write Request: Updated Main Memory directly" << endl;
        }

        evictionBlock.data = MainMemory[blockAddress];
        evictionBlock.state = "V"; // Set valid after fetch
        cout << "Fetched from Memory: Data = " << evictionBlock.data[blockOffset] << endl;
        return evictionBlock.data[blockOffset];
    }
};

void testCache() {
    CacheHelper cacheHelper;
    for (int i = 0; i < 200; i++) {
        cout << "------------------------------------------------------------" << endl;
        int address = rand() % (MEMORY_BLOCKS * BLOCK_SIZE);
        string accessType = (rand() % 2 == 0) ? "READ" : "WRITE";
        int writeData = (accessType == "WRITE") ? rand() % 100 + 1 : 0;

        cout << "Access Type: " << accessType << ", Address: " << address;
        if (accessType == "WRITE") cout << ", Data to Write: " << writeData;
        cout << endl;

        cacheHelper.accessCache(address, accessType, writeData);
        cout << "------------------------------------------------------------" << endl;
    }
}

void printMainMemory() {
    for (int i = 0; i < MEMORY_BLOCKS; i++) {
        cout << "Block " << i << ": ";
        for (int j = 0; j < BLOCK_SIZE; j++) {
            cout << MainMemory[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    freopen("output.txt", "w", stdout);
    srand(time(0));
    cout << "Initializing Main Memory and Cache..." << endl;
    initMainMemory();
    initCache();
    testCache();
    cout << "Cache Test Completed." << endl;
    return 0;
}
