////#include <vector>
////#include <iostream>
////#include <cmath>
////
////class BitWeavingV {
////private:
////    std::vector<uint64_t> data; // Stores data vertically
////    int valueBitSize; // Bit size of each value
////    int numValues; // Number of values stored
////    int wordsPerValue; // Number of 64-bit words used per value
////
////public:
////    BitWeavingV(int numValues, int bitSize)
////            : valueBitSize(bitSize), numValues(numValues) {
////        // Calculate the number of 64-bit words needed per value
////        wordsPerValue = std::ceil(static_cast<double>(valueBitSize) / 64);
////        data.resize(wordsPerValue * numValues, 0);
////    }
////
////    // Set a value at a specific position (simplified; assumes values fit in 64 bits)
////    void setValue(int position, uint64_t value) {
////        for (int bit = 0; bit < valueBitSize; ++bit) {
////            // Calculate the index in the data array and the bit position within that word
////            int index = (bit * numValues + position) / 64;
////            int bitPos = (bit * numValues + position) % 64;
////            // Set the bit if it is present in 'value'
////            if (value & (1ULL << bit)) {
////                data[index] |= (1ULL << bitPos);
////            }
////        }
////    }
////
////    // Perform a scan operation to find values less than 'threshold'
////    void scanLessThan(uint64_t threshold) {
////        for (int position = 0; position < numValues; ++position) {
////            uint64_t value = 0;
////            for (int bit = 0; bit < valueBitSize; ++bit) {
////                // Calculate the index in the data array and the bit position within that word
////                int index = (bit * numValues + position) / 64;
////                int bitPos = (bit * numValues + position) % 64;
////                // Retrieve the bit and add it to 'value'
////                if (data[index] & (1ULL << bitPos)) {
////                    value |= (1ULL << bit);
////                }
////            }
////            if (value < threshold) {
////                std::cout << "Value " << value << " at position " << position << " is less than " << threshold << std::endl;
////            }
////        }
////    }
////};
////
////int main() {
////    BitWeavingV bv(10, 32); // 10 values, each 8 bits
////
////    // Example: setting and scanning values
////    bv.setValue(0, 10);
////    bv.setValue(1, 50);
////    bv.setValue(2, 20);
////    bv.setValue(3, 15);
////    bv.setValue(4, 5);
////    bv.setValue(5, 5);
////    bv.setValue(6, 5);
////    bv.setValue(7, 5);
////    bv.setValue(8, 5);
////    bv.setValue(9, 5);
////
////    bv.scanLessThan(18); // Find values less than 18
////
////    return 0;
////}
////
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <vector>
//#include <bitset>
//#include <cmath>
//#include <chrono>
//
//class BitWeavingV {
//private:
//    std::vector<uint64_t> data; // Stores data vertically
//    int valueBitSize; // Bit size of each value
//    int numValues; // Number of values stored
//    int wordsPerValue; // Number of 64-bit words used per value
//
//public:
//    BitWeavingV(int numValues, int bitSize)
//            : valueBitSize(bitSize), numValues(numValues) {
//        wordsPerValue = std::ceil(static_cast<double>(valueBitSize) / 64);
//        data.resize(wordsPerValue * numValues, 0);
//    }
//
//    void readValuesFromFile(const std::string &filename) {
//        std::ifstream inFile(filename);
//        if (!inFile) {
//            std::cerr << "Unable to open file: " << filename << std::endl;
//            return;
//        }
//
//        std::string line;
//        int position = 0;
//        while (getline(inFile, line)) {
//            // Convert binary string to a uint32_t integer
//            uint32_t value = std::bitset<32>(line).to_ulong();
//            // std::cout << "Read and converted value: " << value << std::endl;  // Debug output to check values
//            if (position >= numValues) break;
//            setValue(position++, value);
//        }
//        inFile.close();
//    }
//
//    void setValue(int position, uint32_t value) {
//        if (position < numValues) {
//            // Assuming each position in data directly corresponds to a whole value
//            data[position] = value;  // This assumes a direct assignment is suitable
//        }
//    }
//
//    void scanLessThan(uint64_t threshold) {
//        for (int position = 0; position < numValues; ++position) {
//            uint64_t value = data[position];
//            if (value < threshold) {
//                std::cout << "Value " << value << " at position " << position << " is less than " << threshold << std::endl;
//            }
//        }
//    }
//};
//
//int main() {
//    auto startTime = std::chrono::high_resolution_clock::now();
//
//    BitWeavingV bv(1000000, 32); // 10 values, each 32 bits
//
//    bv.readValuesFromFile("/Users/yuliang/CLionProjects/databaseBitweavingV/donations-compressed-reg-int"); // Ensure the filename matches your actual data file
//    bv.scanLessThan(600); // Example threshold, adjust as necessary
//
//    // End the timer
//    auto endTime = std::chrono::high_resolution_clock::now();
//
//    // Calculate the duration
//    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
//
//    // Print the duration
//    std::cout << "Overall time taken: " << duration << " milliseconds" << std::endl;
//
//    return 0;
//}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cmath>
#include <chrono>
#include <limits>

class BitWeavingH {
private:
    std::vector<uint64_t> data; // Storage for data, considering 64-bit words
    int bitsPerValue; // Bits per value, not including the delimiter
    int valuesPerWord; // How many values fit in a 64-bit word
    size_t currentValueIndex; // Tracks the current index for the next value insertion

public:
    BitWeavingH(int bitsPerValue)
            : bitsPerValue(bitsPerValue),
              valuesPerWord(64 / (bitsPerValue + 1)), // +1 for the delimiter bit
              currentValueIndex(0) {
        data.resize(1, 0); // Initialize with a single word
    }

    void readValuesFromFile(const std::string &filename) {
        std::ifstream inFile(filename);
        if (!inFile) {
            std::cerr << "Unable to open file: " << filename << std::endl;
            return;
        }

        std::string line;
        while (getline(inFile, line)) {
            // Convert binary string to a uint32_t integer
            uint32_t value = std::bitset<32>(line).to_ulong();
            addValue(value);
        }
        inFile.close();
    }

    void addValue(uint32_t value) {
        int wordIndex = currentValueIndex / valuesPerWord;
        int bitOffset = (currentValueIndex % valuesPerWord) * (bitsPerValue + 1);

        if (wordIndex >= data.size()) {
            data.push_back(0);
        }

        // Mask the input value to fit
        uint64_t maskedValue = value & ((1ULL << bitsPerValue) - 1);
        data[wordIndex] |= (maskedValue << bitOffset);

        currentValueIndex++;
    }

    void scanLessThan(uint64_t threshold) {
        for (size_t i = 0; i < currentValueIndex; ++i) {
            int wordIndex = i / valuesPerWord;
            int bitOffset = (i % valuesPerWord) * (bitsPerValue + 1);
            uint64_t value = (data[wordIndex] >> bitOffset) & ((1ULL << bitsPerValue) - 1);

            if (value < threshold) {
                std::cout << "Value " << value << " at position " << i << " is less than " << threshold << std::endl;
            }
        }
    }
};

int main() {
    auto startTime = std::chrono::high_resolution_clock::now();

    BitWeavingH bwh(32);

    bwh.readValuesFromFile("/Users/yuliang/CLionProjects/databaseBitweavingV/donations-compressed-reg-int"); // Ensure the filename matches your actual data file
    bwh.scanLessThan(1000);

    // End the timer
    auto endTime = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Print the duration
    std::cout << "Overall time taken: " << duration << " milliseconds" << std::endl;

    return 0;
}
