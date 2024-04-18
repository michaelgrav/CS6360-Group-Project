#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

string getLineAtPosition(const string& filename, int position) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Error opening file.");
    }

    string line;
    for (int i = 0; i < position; ++i) {
        if (!getline(file, line)) {
            throw out_of_range("Position out of file range.");
        }
    }

    file.close();
    return line;
}

int main() {
    string filename = "./donations-compressed-edfe";
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    int lineCount = 0;
    string line;
    while (getline(file, line)) {
        lineCount++;
    }

    file.close();

    std::ofstream logfile("./timing_log.txt");
    if (!logfile.is_open()) {
        std::cerr << "Error: Could not open log file." << std::endl;
        return 1;
    }

    logfile << "run_num, dfe_fetch_time, reg_fetch_time" << std::endl;

    for (int i = 0; i < 10 ; i++) {
        srand(time(nullptr));

        // Test DFE integer encoding
        int randomNumber = rand() % lineCount;
        string encoded_line = getLineAtPosition(filename, randomNumber).erase(0, 2);
        auto start = std::chrono::high_resolution_clock::now();
        // Convert the encoded value back to integer
        int encoded_int = std::stoi(encoded_line, nullptr, 2);

        // Calculate the number of salient bits in the lower field
        int len_u = std::ceil(std::log2(32));
        int len_l = 32 - len_u;

        // Extract the upper and lower fields from the encoded value
        int u = encoded_int >> len_l;
        int l = encoded_int & ((1 << len_l) - 1);

        // Calculate the number of leading zeros and the original value
        int clz = 32 - std::to_string(abs(l)).length() - 1;
        int original_value;
        if (clz <= 1) {
            throw std::invalid_argument("Number out of bounds for decoding.");
        } else if (clz <= len_u) {
            original_value = l ^ (1 << (32 - 2));
        } else {
            original_value = (l >> (clz - len_u - 1));
        }

        // Handle negative numbers
        if (u & (1 << (len_u - 1))) {
            original_value *= -1;
        }

        auto stop = std::chrono::high_resolution_clock::now();
        auto duration_dfe = duration_cast<std::chrono::nanoseconds>(stop - start);

        // Test regular integer encoding
        filename = "./donations-compressed-reg-int";

        randomNumber = rand() % lineCount;
        encoded_line = getLineAtPosition(filename, randomNumber);
        start = std::chrono::high_resolution_clock::now();
        int decoded = std::stoi(encoded_line, nullptr, 2);
        stop = std::chrono::high_resolution_clock::now();
        auto duration_reg = duration_cast<std::chrono::nanoseconds>(stop - start);

        logfile << i + 1 << ", " << duration_dfe.count() << ", " << duration_reg.count() << std::endl;
    }

    logfile.close();
    return 0;
}









