#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>
#include <bitset>

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

class BitweavingH {
private:
    std::vector<uint32_t> bitPackedData;
    size_t numValues;
    int valueBitSize;

public:
    BitweavingH(size_t numValues, int valueBitSize)
            : numValues(numValues), valueBitSize(valueBitSize), bitPackedData(valueBitSize * ((numValues + 31) / 32), 0) {}

    void addValue(uint32_t value, size_t index) {
        for (int bit = 0; bit < valueBitSize; ++bit) {
            size_t pos = index * valueBitSize + bit;
            if (value & (1u << bit)) {
                bitPackedData[pos / 32] |= (1u << (pos % 32));
            }
        }
    }

    std::vector<size_t> scanLessThan(uint32_t threshold) const {
        std::vector<size_t> results;
        for (size_t i = 0; i < numValues; ++i) {
            uint32_t value = 0;
            for (int bit = 0; bit < valueBitSize; ++bit) {
                size_t pos = i + bit * numValues;
                if (bitPackedData[pos / 32] & (1u << (pos % 32))) {
                    value |= (1u << bit);
                }
            }

            uint32_t decodedValue = decodeEDFE(value);
            if (decodedValue < threshold) {
                results.push_back(i);
            }
        }
        return results;
    }
};

int main() {
    const std::string filename = "/Users/yuliang/CLionProjects/BitweavingHEDFE/donations-compressed-edfe";  
    const uint32_t threshold = 1000;  // Threshold for the scan operation

    std::ifstream inFile(filename, std::ios::binary);
    if (!inFile) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return EXIT_FAILURE;
    }

    inFile.seekg(0, std::ios::end);
    size_t fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    size_t numValues = fileSize / sizeof(uint32_t);

    BitweavingH bitweaving(numValues, 32);  // 32 bits per value

    uint32_t encodedValue;
    size_t index = 0;
    while (inFile.read(reinterpret_cast<char*>(&encodedValue), sizeof(encodedValue))) {
        bitweaving.addValue(encodedValue, index++);
    }
    inFile.close();

    auto startTime = std::chrono::high_resolution_clock::now();
    auto results = bitweaving.scanLessThan(threshold);
    auto endTime = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = endTime - startTime;
    std::cout << "Scan completed in " << duration.count() << " ms." << std::endl;
//    for (size_t result : results) {
//        std::cout << "Position " << result << " is less than " << threshold << std::endl;
//    }

    return 0;
}
