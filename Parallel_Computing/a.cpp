#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ratio>
#include <string>
#include <omp.h>

static int num_files, amount_calculation;

auto test_o() -> std::chrono::duration<double, std::micro> {
    decltype(std::chrono::high_resolution_clock::now()) start, end;

#pragma omp parallel
    {
#pragma omp single
        start = std::chrono::high_resolution_clock::now();

#pragma omp for
        for (int i = 0; i < num_files; ++i) {
            std::ofstream ofile{std::to_string(i) + ".txt",
                                std::ios_base::out
                                | std::ios_base::trunc};
            while (std::rand() % amount_calculation != 0)
                ofile << std::rand();
        }

#pragma omp single
        end = std::chrono::high_resolution_clock::now();
    }

    return end - start;
}

auto test_io() -> decltype(test_o()) {
    decltype(std::chrono::high_resolution_clock::now()) start, end;

#pragma omp parallel
    {
#pragma omp single
        start = std::chrono::high_resolution_clock::now();

#pragma omp for
        for (int i = 0; i < num_files; ++i)
            if (std::fstream file{std::to_string(i) + ".txt",
                                  std::ios_base::in
                                  | std::ios_base::out}; i % 2 == 0)
                while (std::rand() % amount_calculation != 0)
                    file << std::rand();
            else
                for (char tmp; std::rand() % amount_calculation != 0; )
                    file >> tmp;

#pragma omp single
        end = std::chrono::high_resolution_clock::now();
    }

    return end - start;
}

auto test_i() -> decltype(test_o()) {
    decltype(std::chrono::high_resolution_clock::now()) start, end;

#pragma omp parallel
    {
#pragma omp single
        start = std::chrono::high_resolution_clock::now();

#pragma omp for
        for (int i = 0; i < num_files; ++i) {
            std::ifstream ifile{std::to_string(i) + ".txt",
                                std::ios_base::in};
            for (char tmp; std::rand() % amount_calculation != 0; )
                ifile >> tmp;
        }

#pragma omp single
        end = std::chrono::high_resolution_clock::now();
    }

    return end - start;
}

int main(int argc, char *argv[]) {
    num_files = std::stoi(argv[2]),
      amount_calculation = std::stoi(argv[3]);

    decltype(test_o()) time_elapsed;

    switch (std::stoi(argv[1])) {
        case 0:
            time_elapsed = test_o();
            break;
        case 1:
            time_elapsed = test_i();
            break;
        case 2:
            time_elapsed = test_io();
            break;
    }

    std::cout << time_elapsed / num_files;
}
