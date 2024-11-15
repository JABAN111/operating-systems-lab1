//
// Created by Михаил Нягин on 14.11.2024.
//
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

std::vector<long long> primeFactorize(long long n) {
    std::vector<long long> factors;

    // Убираем четные множители
    while (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
    }

    // Проверяем нечетные числа от 3 до √n
    for (long long i = 3; i <= std::sqrt(n); i += 2) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }

    // Если после этого осталось простое число больше 2
    if (n > 2) {
        factors.push_back(n);
    }

    return factors;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <число для разложения на множетил> <число запусков программы>" << std::endl;
        return 1;
    }

    // Число для разложения и количество повторений
    long long number = std::atoll(argv[1]);
    int iterations = std::atoi(argv[2]);

    for (int i = 0; i < iterations; ++i) {
        std::vector<long long> factors = primeFactorize(number);

        std::cout << "Iteration " << i + 1 << ": ";
        for (const auto& factor : factors) {
            std::cout << factor << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Factorization completed." << std::endl;
    return 0;
}