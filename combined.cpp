#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <queue>
#include <filesystem>
#include <unistd.h>
#include <random>

#include <sys/wait.h>

// ---- Первое задание: Разложение числа на множители ----

std::vector<long long> primeFactorize(long long n) {
    std::vector<long long> factors;

    while (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
    }

    for (long long i = 3; i <= std::sqrt(n); i += 2) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }

    if (n > 2) {
        factors.push_back(n);
    }

    return factors;
}

void startFactorize(const long long number, const int iterations) {
    for (int i = 0; i < iterations; ++i) {
        std::vector<long long> factors = primeFactorize(number);

        std::cout << "Iteration " << i + 1 << ": ";
        for (const auto& factor : factors) {
            std::cout << factor << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Factorization completed." << std::endl;
}

// ---- Второе задание: Сортировка массива во внешней памяти ----

constexpr size_t BLOCK_SIZE = 32 * 1024 * 1024 / sizeof(int);

// Предыдущая версия
// void generateRandomFile(const std::string& filename, const size_t count) {
//     std::ofstream outfile(filename);
//     if (!outfile.is_open()) {
//         std::cerr << "Error opening file for writing: " << filename << std::endl;
//         exit(1);
//     }
//     for (size_t i = 0; i < count; ++i) {
//         int num = rand() % 10000; // случайное число
//         outfile << num << std::endl;
//     }
//     outfile.close();
// }

void generateRandomFile(const std::string& filename, const size_t count) {
    try {
        // Устанавливаем флаги для выбрасывания исключений при ошибках
        std::ofstream outfile(filename);
        outfile.exceptions(std::ios::failbit | std::ios::badbit);

        // Генератор случайных чисел
        std::random_device rd;  // Источник случайности
        std::mt19937 gen(rd());  // Генератор случайных чисел
        std::uniform_int_distribution<> dis(0, 9999);  // Равномерное распределение от 0 до 9999

        // Запись случайных чисел в файл
        for (size_t i = 0; i < count; ++i) {
            int num = dis(gen);  // Генерация случайного числа
            outfile << num << std::endl;
        }
    } catch (const std::ios_base::failure& e) {
        std::cerr << "I/O error: " << e.what() << std::endl;
        exit(1);
    }
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

void writeBlock(const std::filesystem::path& filename, const std::vector<int>& data) {
    try {
        std::ofstream outfile(filename);
        if (!outfile.is_open()) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            exit(1);
        }

        for (const auto& num : data) {
            outfile << num << std::endl;
        }
    } catch (const std::ios_base::failure& e) {
        std::cerr << "I/O error: " << e.what() << std::endl;
        exit(1);
    }
}


void mergeFiles(const std::vector<std::string>& sortedFiles, const std::string& outputFile) {
    using Pair = std::pair<int, int>;  // Псевдоним для std::pair<int, int>

    std::ofstream outfile(outputFile);
    std::vector<std::ifstream> streams;
    for (const auto& file : sortedFiles) {
        streams.emplace_back(file);
    }

    auto compare = [](const Pair& a, const Pair& b) { return a.first > b.first; };  // Лямбда-функция для сравнения
    std::priority_queue<Pair, std::vector<Pair>, decltype(compare)> minHeap(compare);  // Очередь с приоритетом

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

void startEmaSort(const std::string& inputFile, const std::string& outputFile, size_t numElements) {
    generateRandomFile(inputFile, numElements);

    std::ifstream infile(inputFile);
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

    std::cout << "Sorting completed. Data saved to " << outputFile << std::endl;
}

// ---- Основная программа ----

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <number_to_factorize> <iterations> <num_elements_for_sorting>" << std::endl;
        return 1;
    }

    const long long number = std::stoll(argv[1]);
    const int iterations = std::stoi(argv[2]);
    const size_t numElements = std::stoul(argv[3]);
    const std::string sortInputFile = "unsorted_file.txt";
    const std::string sortOutputFile = "sorted_output.txt";

    const pid_t pid1 = vfork();
    if (pid1 == 0) {
        startFactorize(number, iterations);
        return 0;
    }

    const pid_t pid2 = vfork();
    if (pid2 == 0) {
        startEmaSort(sortInputFile, sortOutputFile, numElements);
        return 0;
    }

    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    std::cout << "All tasks completed." << std::endl;
    return 0;
}