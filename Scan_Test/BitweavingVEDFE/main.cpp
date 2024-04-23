#include <iostream>
#include <vector>
#include <bitset>
#include <fstream>
#include <chrono>

// Helper function to count the leading zeros
int countLeadingZeros(uint16_t value) {
    if (value == 0) return 16;
    int count = 0;
    for (int i = 15; i >= 0; --i) {
        if ((value >> i) & 1) break;
        ++count;
    }
    return count;
}

uint16_t decodeEDFE(uint16_t encodedValue) {
    // If the value is zero, return zero.
    if (encodedValue == 0) return 0;

    bool isNegative = encodedValue & 0x8000; // Sign bit
    bool isLargeNumber = encodedValue & 0x4000; // Format bit

    encodedValue &= 0x3FFF; // Clear sign and format bit

    uint16_t value = 0;
    if (isLargeNumber) {
        // For large numbers
        value = encodedValue;
    } else {
        // For small numbers
        int leadingZeros = countLeadingZeros(encodedValue) - 1; // Exclude the format bit
        value = (encodedValue << leadingZeros) & 0x3FFF; // Align to the rightmost bits
    }

    if (isNegative) {
        value = ~value + 1;
    }

    return value;
}

class BitweavingV {
private:
    std::vector<uint32_t> bitPackedData;
    size_t numValues;
    int valueBitSize;

public:
    BitweavingV(size_t numValues, int valueBitSize)
            : numValues(numValues), valueBitSize(valueBitSize), bitPackedData((numValues * valueBitSize + 31) / 32, 0) {}

    void addValue(uint16_t value, size_t index) {
        for (int bit = 0; bit < valueBitSize; ++bit) {
            size_t pos = index * valueBitSize + bit;
            if (value & (1 << bit)) {
                bitPackedData[pos / 32] |= (1 << (pos % 32));
            }
        }
    }

    std::vector<size_t> scanLessThan(uint16_t threshold) const {
        std::vector<size_t> results;
        for (size_t i = 0; i < numValues; ++i) {
            uint16_t value = 0;
            for (int bit = 0; bit < valueBitSize; ++bit) {
                size_t pos = i * valueBitSize + bit;
                if (bitPackedData[pos / 32] & (1 << (pos % 32))) {
                    value |= (1 << bit);
                }
            }

            uint16_t decodedValue = decodeEDFE(value);
            if (decodedValue < threshold) {
                results.push_back(i);
            }
        }
        return results;
    }
};

int main() {
    const std::string filename = "/Users/yuliang/CLionProjects/BitweavingVEDFE/donations-compressed-edfe"; 
    std::ifstream inFile(filename, std::ios::binary);

    if (!inFile) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return EXIT_FAILURE;
    }

    // Get the number of values in the file
    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    size_t numValues = fileSize / sizeof(uint16_t); 

    // Create a BitweavingV object with the correct number of values
    BitweavingV bitweaving(numValues, 32); // 32 bits values

    // Read and decode values from the file
    uint16_t encodedValue;
    size_t index = 0;
    while (inFile.read(reinterpret_cast<char*>(&encodedValue), sizeof(encodedValue))) {
        uint16_t decodedValue = decodeEDFE(encodedValue);
        bitweaving.addValue(decodedValue, index++);
    }
    inFile.close();

    uint16_t threshold = 1000; // The threshold for the scan operation
    auto startTime = std::chrono::high_resolution_clock::now();
    auto results = bitweaving.scanLessThan(threshold);
    auto endTime = std::chrono::high_resolution_clock::now();


    std::chrono::duration<double, std::milli> duration = endTime - startTime;
    std::cout << "Scan completed in " << duration.count() << " ms." << std::endl;

    // Output the results
//    for (size_t resultIndex : results) {
//        std::cout << "Value at position " << resultIndex << " is less than " << threshold << std::endl;
//    }

    return 0;
}
