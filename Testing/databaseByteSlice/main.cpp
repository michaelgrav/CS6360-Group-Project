//#include <iostream>
//#include <vector>
//#include <cstdint>
//
//class ByteSlice {
//private:
//    std::vector<std::vector<uint8_t>> columns;
//    int valueBitSize;
//    size_t numRows;
//
//public:
//    ByteSlice(int bitSize, size_t initialCapacity = 1024) : valueBitSize(bitSize) {
//        size_t numSlices = (bitSize + 7) / 8;
//        columns.resize(numSlices, std::vector<uint8_t>(initialCapacity, 0));
//        numRows = 0;
//    }
//
//    void addValue(uint32_t value) {
//        for (size_t i = 0; i < columns.size(); ++i) {
//            if (numRows == columns[i].size()) {
//                columns[i].resize(columns[i].size() * 2);
//            }
//            columns[i][numRows] = (value >> (i * 8)) & 0xFF;
//        }
//        ++numRows;
//    }
//
//    void scanLessThan(uint32_t threshold) {
//        for (size_t row = 0; row < numRows; ++row) {
//            uint32_t reconstructedValue = 0;
//            for (size_t i = 0; i < columns.size(); ++i) {
//                reconstructedValue |= columns[i][row] << (i * 8);
//            }
//            if (reconstructedValue < threshold) {
//                std::cout << "Value " << reconstructedValue << " at row " << row << " is less than " << threshold << std::endl;
//            }
//        }
//    }
//};
//
//int main() {
//    ByteSlice column(32);
//
//    column.addValue(10);
//    column.addValue(20);
//    column.addValue(30);
//    column.addValue(40);
//
//    std::cout << "Performing scan for values less than 25:" << std::endl;
//    column.scanLessThan(25);
//
//    return 0;
//}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <chrono>
#include <limits>

class ByteSlice {
private:
    std::vector<uint64_t> data; // Storage for data, considering 64-bit words
    int bitsPerValue; // Bits per value

public:
    ByteSlice(int bitsPerValue)
            : bitsPerValue(bitsPerValue) {}

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
        int wordIndex = data.size();
        int bitOffset = 0;

        // Determine the number of words needed
        int wordsNeeded = (bitsPerValue + 63) / 64;

        // Add zero words if needed
        if (wordIndex + wordsNeeded > data.size()) {
            data.resize(wordIndex + wordsNeeded, 0);
        }

        // Mask the input value to fit the bitsPerValue and insert it
        uint64_t maskedValue = value & ((1ULL << bitsPerValue) - 1);
        data[wordIndex] |= (maskedValue << bitOffset);
    }

    void scanLessThan(uint64_t threshold) {
        for (size_t i = 0; i < data.size(); ++i) {
            uint64_t value = data[i];
            if (value < threshold) {
                std::cout << "Value " << value << " at position " << i << " is less than " << threshold << std::endl;
            }
        }
    }
};

int main() {
    auto startTime = std::chrono::high_resolution_clock::now();

    ByteSlice bs(32); // Initialize with a large enough number of bits per value

    bs.readValuesFromFile("/Users/yuliang/CLionProjects/databaseByteSlice/donations-compressed-reg-int"); // Ensure the filename matches your actual data file
    bs.scanLessThan(1000); // Use the maximum value of uint32_t as the threshold

    // End the timer
    auto endTime = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    // Print the duration
    std::cout << "Overall time taken: " << duration << " milliseconds" << std::endl;

    return 0;
}
