#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <chrono>

#include <cmath>
#include <cstdint>
#include <stdexcept>

// Helper function to calculate the base-2 logarithm
int log2(int x) {
    if (x <= 0) throw std::runtime_error("log2: non-positive input");
    return 31 - __builtin_clz(x); // Use builtin function to count leading zeros
}

uint16_t decodeEDFE(uint16_t encodedValue) {
    // If the encoded value is zero, return zero
    if (encodedValue == 0) return 0;

    // Extract the sign bit
    bool isNegative = encodedValue & 0x8000;

    // Remove the sign bit
    encodedValue &= 0x7FFF;

    // Initialize value to be returned
    uint16_t value;

    // Check if it's a large number (format bit set)
    if (encodedValue & 0x4000) {
        // It's a large number, so just remove the format bit
        value = encodedValue & 0x3FFF;
    } else {
        // It's a small number, count the zeroes after the format bit
        int leadingZeroes = 0;
        uint16_t mask = 0x2000; // Start checking from the bit after the format bit
        while (!(encodedValue & mask)) {
            leadingZeroes++;
            mask >>= 1;
        }
        // Shift left by the count of leading zeroes
        value = (encodedValue & (mask - 1)) << leadingZeroes;
    }

    // If the number is negative, convert it back to a negative integer
    if (isNegative) {
        value = static_cast<uint16_t>(-static_cast<int16_t>(value));
    }

    return value;
}

//uint32_t decodeEDFE(uint32_t encodedValue, int b) {
//    if (encodedValue == 0) {
//        // Encoded value is zero, just return zero
//        return 0;
//    }
//
//    int lenU = log2(b);
//    int lenL = b - lenU;
//
//    // Calculate l_mask for later use
//    uint32_t l_mask = (1u << (lenL - 2)) - 1;
//
//    // Check the format bit (second most significant bit)
//    if (encodedValue & (1u << (b - 2))) {
//        // Large number case, invert format bit to restore the original number
//        return encodedValue ^ (1u << (b - 2));
//    } else {
//        // Small number case, similar to DFE.
//        // Extract u and l from the encoded value
//        uint32_t u = encodedValue >> (lenL - 2);
//        uint32_t l = (encodedValue & l_mask) << 1; // Shift left to account for the implicit '1'
//
//        // Combine u and l to form the original number
//        uint32_t result = (u << (lenU - 2)) + l;
//
//        // If the original number was negative, we need to restore the sign by inverting the bits
//        if (u & (1u << (lenU - 2))) {
//            result = ~result;
//        }
//        return result;
//    }
//}

class ByteSlice {
private:
    std::vector<std::vector<uint8_t>> slices; // Slices for each bit
    size_t numValues; // Number of values
    int bitSize; // Number of bits in the encoded integers

    // Helper method to get the byte and bit position within the byte for the slice
    void getByteAndBitPosition(size_t valueIndex, size_t& byteIndex, size_t& bitPos) const {
        byteIndex = valueIndex / 8;
        bitPos = 7 - (valueIndex % 8); // Reverse bit order within byte for easier comparison
    }

public:
    ByteSlice(size_t numValues, int bitSize) : numValues(numValues), bitSize(bitSize), slices(bitSize, std::vector<uint8_t>((numValues + 7) / 8, 0)) {}

    // Add a value to the ByteSlice structure
    void addValue(uint32_t value, size_t index) {
        for (size_t i = 0; i < bitSize; ++i) {
            size_t byteIndex, bitPos;
            getByteAndBitPosition(i * numValues + index, byteIndex, bitPos); // Calculate the bit position for this specific value
            if (value & (1u << (bitSize - 1 - i))) { // Check if the (bitSize - 1 - i)th bit is set, starting from the MSB
                slices[i][byteIndex] |= (1u << bitPos);
            } else {
                slices[i][byteIndex] &= ~(1u << bitPos);
            }
        }
    }

    // Method to search for values less than the given threshold
    std::vector<size_t> scanLessThan(uint32_t threshold) const {
        std::vector<size_t> results;

        // Reconstruct each value from the slices and check if it's less than the threshold
        for (size_t i = 0; i < numValues; ++i) {
            uint32_t reconstructedValue = 0;
            for (size_t bit = 0; bit < bitSize; ++bit) {
                size_t byteIndex, bitPos;
                getByteAndBitPosition(bit * numValues + i, byteIndex, bitPos);

                if (slices[bit][byteIndex] & (1u << bitPos)) {
                    reconstructedValue |= (1u << (bitSize - 1 - bit));
                }
            }

            // Decode the value
            uint32_t decodedValue = decodeEDFE(reconstructedValue);

            // Check if the decoded value is less than the threshold
            if (decodedValue < threshold) {
                results.push_back(i);
            }
        }

        return results;
    }
};

void loadEDFEData(const std::string& filename, ByteSlice& byteSlice) {
    std::ifstream inFile(filename, std::ios::binary);
    uint32_t encodedValue;
    size_t index = 0; // Add a counter to keep track of the current index

    while (inFile.read(reinterpret_cast<char*>(&encodedValue), sizeof(encodedValue))) {
        uint32_t decodedValue = decodeEDFE(encodedValue); // Decode the EDFE encoded value
        byteSlice.addValue(decodedValue, index); // Pass both the decoded value and the index
        index++;
    }

    if (!inFile.eof() && inFile.fail()) {
        std::cerr << "Error occurred while reading the file." << std::endl;
    }
}

int main() {
    const std::string filename = "/Users/yuliang/CLionProjects/ByteSliceEDFE/donations-compressed-edfe";
    std::ifstream inFile(filename, std::ios::binary | std::ios::ate);
    if (!inFile) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return EXIT_FAILURE;
    }

    std::streamsize fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    size_t numValues = fileSize / sizeof(uint32_t);

    // 32 bits
    ByteSlice byteSlice(numValues, 32);

    // Load and decode EDFE data
    loadEDFEData(filename, byteSlice);
    inFile.close();

    // Print a message to indicate that data loading is complete
    std::cout << "Data loading complete. Number of values: " << numValues << std::endl;

    // Start timing
    auto startTime = std::chrono::high_resolution_clock::now();

    // Perform the search and capture the results
    std::vector<size_t> results = byteSlice.scanLessThan(1000); 

    // Stop timing
    auto endTime = std::chrono::high_resolution_clock::now();

    // Calculate the running time
    auto runningTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    // Check if the results vector is empty
//    if (results.empty()) {
//        std::cout << "No values found that are less than the threshold." << std::endl;
//    } else {
//        // Print the results
//        for (size_t index : results) {
//            std::cout << "Value at position " << index << " is less than 1000" << std::endl;
//        }
//    }

    // Print the running time
    std::cout << "Running time: " << runningTime.count() << " milliseconds" << std::endl;

    return 0;
}
