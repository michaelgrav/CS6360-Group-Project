#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <iostream>
#include <chrono>

uint32_t decodeDFE(uint32_t encodedValue) {
    int totalBits = 32;
    int shiftFieldLength = 6; 
    int valueFieldLength = totalBits - shiftFieldLength;

    int numLeadingZeros = (encodedValue >> (totalBits - shiftFieldLength)) & ((1 << shiftFieldLength) - 1);

    // Calculate the actual shift needed to restore the original bits
    int actualShift = totalBits - numLeadingZeros - 8; // 8 is the number of bits in the original number

    // Mask to extract the significant bits then shift them right
    uint32_t mask = ((1 << (valueFieldLength - numLeadingZeros)) - 1) << numLeadingZeros;
    uint32_t value = (encodedValue & mask) >> numLeadingZeros;

    return value;
}

class ByteSlice {
private:
    std::vector<std::vector<uint8_t>> slices; // 32 slices for 32-bit integers
    size_t numValues; // Number of values

    // Helper method to get the byte and bit position within the byte for the slice
    void getByteAndBitPosition(size_t valueIndex, size_t& byteIndex, size_t& bitPos) const {
        byteIndex = valueIndex / 8;
        bitPos = 7 - (valueIndex % 8); // Reverse bit order within byte for easier comparison
    }

public:
    // Constructor initializes 32 slices for 32-bit integers
    ByteSlice(size_t numValues) : numValues(numValues), slices(32, std::vector<uint8_t>((numValues + 7) / 8, 0)) {}

    // Add a value to the ByteSlice structure
    void addValue(uint32_t value, size_t index) {
        for (size_t i = 0; i < 32; ++i) {
            size_t byteIndex, bitPos;
            getByteAndBitPosition(index, byteIndex, bitPos);
            if (value & (1 << i)) { // Check if the ith bit is set
                slices[i][byteIndex] |= (1 << bitPos);
            } else {
                slices[i][byteIndex] &= ~(1 << bitPos);
            }
        }
    }

    // Search for values less than the given threshold
    std::vector<size_t> scanLessThan(uint32_t threshold) const {
        std::vector<size_t> results;
        for (size_t i = 0; i < numValues; ++i) {
            bool lessThan = true;
            for (int j = 31; j >= 0 && lessThan; --j) { // Compare from the most significant bit
                size_t byteIndex, bitPos;
                getByteAndBitPosition(i, byteIndex, bitPos);
                bool valueBit = slices[j][byteIndex] & (1 << bitPos);
                bool thresholdBit = threshold & (1 << j);

                // If value bit is 0 and threshold bit is 1, then value is less than threshold
                if (!valueBit && thresholdBit) {
                    break;
                }
                // If value bit is 1 and threshold bit is 0, then value is not less than threshold
                if (valueBit && !thresholdBit) {
                    lessThan = false;
                }
            }
            if (lessThan) {
                results.push_back(i);
            }
        }
        return results;
    }
};


//void loadDFEData(const std::string& filename, ByteSlice& byteSlice) {
//    std::ifstream inFile(filename, std::ios::binary);
//    uint32_t encodedValue;
//    size_t index = 0; // Add a counter to keep track of the current index
//
//    while (inFile.read(reinterpret_cast<char*>(&encodedValue), sizeof(encodedValue))) {
//        uint32_t decodedValue = decodeDFE(encodedValue);
//        byteSlice.addValue(decodedValue, index++); // Pass both the decoded value and the index
//    }
//}
void loadDFEData(const std::string& filename, ByteSlice& byteSlice) {
    std::ifstream inFile(filename, std::ios::binary);
    uint32_t encodedValue;
    size_t index = 0; // Add a counter to keep track of the current index

    // Print a small sample of the data for verification
    const size_t sampleSize = 10;

    while (inFile.read(reinterpret_cast<char*>(&encodedValue), sizeof(encodedValue))) {
        uint32_t decodedValue = decodeDFE(encodedValue);
        byteSlice.addValue(decodedValue, index); // Pass both the decoded value and the index

//        // Print out the first few decoded values
//        if (index < sampleSize) {
//            std::cout << "Sample decoded value[" << index << "]: " << decodedValue << std::endl;
//        }

        index++;
    }

    // Check if the file read was successful
    if (!inFile.eof() && inFile.fail()) {
        std::cerr << "Error occurred while reading the file." << std::endl;
    }
}


//int main() {
//    const std::string filename = "/Users/yuliang/CLionProjects/BytesliceDFE/donations-compressed-dfe"; // Use the actual path to your file
//    std::ifstream inFile(filename, std::ios::binary | std::ios::ate);
//    if (!inFile) {
//        std::cerr << "Unable to open file: " << filename << std::endl;
//        return EXIT_FAILURE;
//    }
//
//    // Determine the number of values by the file size
//    std::streamsize fileSize = inFile.tellg();
//    inFile.seekg(0, std::ios::beg);
//    size_t numValues = fileSize / sizeof(uint32_t);
//
//    // Initialize ByteSlice structure with the number of values
//    ByteSlice byteSlice(numValues);
//
//    // Load and decode data
//    loadDFEData(filename, byteSlice);
//
//    // Perform the search and capture the results
//    std::vector<size_t> results = byteSlice.scanLessThan(1000); // Example search threshold
//
//    // Print the results
//    for (size_t index : results) {
//        std::cout << "Value at position " << index << " is less than 1000" << std::endl;
//    }
//
//    return 0;
//}
int main() {
    const std::string filename = "/Users/yuliang/CLionProjects/BytesliceDFE/donations-compressed-dfe"; 
    std::ifstream inFile(filename, std::ios::binary | std::ios::ate);
    if (!inFile) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return EXIT_FAILURE;
    }

    // Determine the number of values by the file size
    std::streamsize fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);
    size_t numValues = fileSize / sizeof(uint32_t);

    // Initialize ByteSlice structure with the number of values
    ByteSlice byteSlice(numValues);

    // Load and decode data with debugging
    loadDFEData(filename, byteSlice);
    inFile.close();

    // Print a message to indicate that data loading is complete
    std::cout << "Data loading complete. Number of values: " << numValues << std::endl;

    // Start timing
    auto startTime = std::chrono::high_resolution_clock::now();

    // Perform the search and capture the results
    std::vector<size_t> results = byteSlice.scanLessThan(1000); // threshold

    // Stop timing
    auto endTime = std::chrono::high_resolution_clock::now();

    // Calculate the running time
    auto runningTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    // Check if the results vector is empty
    if (results.empty()) {
        std::cout << "No values found that are less than the threshold." << std::endl;
    } else {
        // Print the results
        for (size_t index : results) {
            std::cout << "Value at position " << index << " is less than 1000" << std::endl;
        }
    }

    // Print the running time
    std::cout << "Running time: " << runningTime.count() << " milliseconds" << std::endl;

    return 0;
}
