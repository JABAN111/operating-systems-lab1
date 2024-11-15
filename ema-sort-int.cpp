#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <string>

constexpr size_t BLOCK_SIZE = 32 * 1024 * 1024 / sizeof(int);

void generateRandomFile(const std::string& filename, const size_t count) {
    std::ofstream outfile(filename);
    if (!outfile.is_open()) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        exit(1);
    }
    for (size_t i = 0; i < count; ++i) {
        int num = rand() % 10000; // случайное число
        outfile << num << std::endl;
    }
    outfile.close();
}

std::vector<int> readBlock(std::ifstream& infile, size_t blockSize) {
    std::vector<int> data;
    data.reserve(blockSize);
    int num;
    while (blockSize-- && infile >> num) {
        data.push_back(num);
    }
    return data;
}

void writeBlock(const std::string& filename, const std::vector<int>& data) {
    std::ofstream outfile(filename);
    for (const auto& num : data) {
        outfile << num << std::endl;
    }
}

void mergeFiles(const std::vector<std::string>& sortedFiles, const std::string& outputFile) {
    std::ofstream outfile(outputFile);
    std::vector<std::ifstream> streams;
    for (const auto& file : sortedFiles) {
        streams.emplace_back(file);
    }

    auto compare = [](std::pair<int, int>& a, std::pair<int, int>& b) { return a.first > b.first; };
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, decltype(compare)> minHeap(compare);

    for (int i = 0; i < streams.size(); ++i) {
        int num;
        if (streams[i] >> num) {
            minHeap.emplace(num, i);
        }
    }

    while (!minHeap.empty()) {
        auto [value, index] = minHeap.top();
        minHeap.pop();
        outfile << value << std::endl;
        int num;
        if (streams[index] >> num) {
            minHeap.emplace(num, index);
        }
    }

    outfile.close();
    for (auto& stream : streams) {
        stream.close();
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <file_for_writing_random_files> <sorted> <num_elements>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];
    size_t numElements = std::stoul(argv[3]);

    generateRandomFile(inputFile, numElements);

    std::ifstream infile(inputFile);
    if (!infile.is_open()) {
        std::cerr << "Ошибка при работе с файлом: " << inputFile << std::endl;
        return 1;
    }

    std::vector<std::string> tempFiles;
    size_t fileCounter = 0;

    while (!infile.eof()) {
        std::vector<int> data = readBlock(infile, BLOCK_SIZE);
        if (data.empty()) break;

        std::sort(data.begin(), data.end());

        std::string tempFile = "temp_" + std::to_string(fileCounter++) + ".txt";
        writeBlock(tempFile, data);
        tempFiles.push_back(tempFile);
    }

    infile.close();

    mergeFiles(tempFiles, outputFile);

    for (const auto& tempFile : tempFiles) {
        remove(tempFile.c_str());
    }

    std::cout << "External memory sorting completed. Sorted data saved to " << outputFile << std::endl;
    return 0;
}