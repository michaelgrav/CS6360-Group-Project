#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <cmath>
#include <cstdint>
#include <chrono>

const int ENCODED_INT_BITS = 32;

// Decode function for DFE
uint32_t decodeDFE(uint32_t encodedValue) {
    int totalBits = 32;
    int shiftFieldLength = 6; // Assuming the first 6 bits for the shift value
    int valueFieldLength = totalBits - shiftFieldLength;

    // The first 6 bits indicate the number of positions the significant bits are shifted left
    int numLeadingZeros = (encodedValue >> (totalBits - shiftFieldLength)) & ((1 << shiftFieldLength) - 1);

    // Calculate the actual shift needed to restore the original bits
    int actualShift = totalBits - numLeadingZeros - 8; // 8 is the number of bits in the original number (assuming 8 significant bits)

    // Mask to extract the significant bits then shift them right
    uint32_t mask = ((1 << (valueFieldLength - numLeadingZeros)) - 1) << numLeadingZeros;
    uint32_t value = (encodedValue & mask) >> numLeadingZeros;

    return value;
}

class BitWeavingV {
private:
    std::vector<std::vector<bool>> columns;
    size_t numValues;

public:
    BitWeavingV(size_t numValues)
            : numValues(numValues), columns(ENCODED_INT_BITS, std::vector<bool>(numValues, false)) {}

    void addValue(uint32_t value, size_t rowIndex) {
        for (size_t bitIndex = 0; bitIndex < ENCODED_INT_BITS; ++bitIndex) {
            columns[bitIndex][rowIndex] = (value & (1 << bitIndex)) != 0;
        }
    }

    std::vector<size_t> searchLessThan(uint32_t threshold) {
        std::vector<size_t> resultIndices;
        for (size_t rowIndex = 0; rowIndex < numValues; ++rowIndex) {
            uint32_t value = 0;
            for (size_t bitIndex = 0; bitIndex < ENCODED_INT_BITS; ++bitIndex) {
                if (columns[bitIndex][rowIndex]) {
                    value |= (1 << bitIndex);
                }
            }
            if (value < threshold) {
                resultIndices.push_back(rowIndex);
            }
        }
        return resultIndices;
    }
};

int main() {
    const std::string filename = "/Users/yuliang/CLionProjects/BitweavingVDFE/donations-compressed-dfe";
    std::ifstream inFile(filename, std::ios::binary | std::ios::ate);
    if (!inFile) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return EXIT_FAILURE;
    }

    size_t fileSize = inFile.tellg();
    size_t numValues = fileSize / sizeof(uint32_t);

    BitWeavingV bitWeaving(numValues);

    inFile.seekg(0, std::ios::beg);
    uint32_t encodedValue;
    size_t index = 0;
    while (inFile.read(reinterpret_cast<char*>(&encodedValue), sizeof(encodedValue))) {
        bitWeaving.addValue(encodedValue, index++);
    }
    inFile.close();

    uint32_t searchThreshold = 1000;  // Set threshold to 1000

    auto startTime = std::chrono::high_resolution_clock::now();
    std::vector<size_t> results = bitWeaving.searchLessThan(searchThreshold);
    auto endTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

//    if (results.empty()) {
//        std::cout << "No values found that are less than the threshold." << std::endl;
//    } else {
//        for (size_t position : results) {
//            std::cout << "Value at position " << position << " is less than 1000" << std::endl;
//        }
//    }

    std::cout << "Search completed in " << duration << " milliseconds." << std::endl;

    return 0;
}