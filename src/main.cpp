// Copyright 2022 NNTU-CS
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
#include "tree.h"

static void printPerm(const std::vector<char>& perm) {
    for (char c : perm) std::cout << c;
    std::cout << "\n";
}

static int64_t factorial(int n) {
    int64_t f = 1;
    for (int i = 2; i <= n; ++i) f *= i;
    return f;
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "=== Example: {'1','2','3'} ===\n";
    {
        std::vector<char> in = {'1', '2', '3'};
        PMTree tree(in);

        std::cout << "getAllPerms:\n";
        auto perms = getAllPerms(tree);
        for (int i = 0; i < static_cast<int>(perms.size()); ++i) {
            std::cout << "  " << i + 1 << ": ";
            printPerm(perms[i]);
        }

        std::cout << "getPerm1(tree,1): "; printPerm(getPerm1(tree, 1));
        std::cout << "getPerm1(tree,2): "; printPerm(getPerm1(tree, 2));
        std::cout << "getPerm2(tree,1): "; printPerm(getPerm2(tree, 1));
        std::cout << "getPerm2(tree,2): "; printPerm(getPerm2(tree, 2));
    }

    std::cout << "\n=== Example: {'1','3','5','7'} ===\n";
    {
        std::vector<char> in = {'1', '3', '5', '7'};
        PMTree tree(in);

        auto perms = getAllPerms(tree);
        std::cout << "Total permutations: " << perms.size() << "\n";
        for (int i = 0; i < static_cast<int>(perms.size()); ++i) {
            std::cout << "  " << i + 1 << ": ";
            printPerm(perms[i]);
        }

        std::cout << "getPerm2(tree,1): "; printPerm(getPerm2(tree, 1));
        std::cout << "getPerm2(tree,2): "; printPerm(getPerm2(tree, 2));
    }

    std::cout << "\n=== Timing experiment ===\n";

    const int N_MAX = 9;

    std::ofstream csv("/home/claude/project/result/timing.csv");
    csv << "n,getAllPerms_us,getPerm1_us,getPerm2_us\n";

    for (int n = 2; n <= N_MAX; ++n) {
        std::vector<char> in;
        for (int i = 0; i < n; ++i) in.push_back(static_cast<char>('1' + i));

        PMTree tree(in);

        int64_t total = factorial(n);
        int randNum = static_cast<int>(1 + std::rand() % total);

        const int REPS = 5;

        double t_all = 0;
        for (int r = 0; r < REPS; ++r) {
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile auto res = getAllPerms(tree);
            auto t1 = std::chrono::high_resolution_clock::now();
            t_all += std::chrono::duration<double, std::micro>(t1 - t0).count();
        }
        t_all /= REPS;

        double t1f = 0;
        for (int r = 0; r < REPS; ++r) {
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile auto res = getPerm1(tree, randNum);
            auto t1 = std::chrono::high_resolution_clock::now();
            t1f += std::chrono::duration<double, std::micro>(t1 - t0).count();
        }
        t1f /= REPS;

        double t2f = 0;
        for (int r = 0; r < REPS; ++r) {
            auto t0 = std::chrono::high_resolution_clock::now();
            volatile auto res = getPerm2(tree, randNum);
            auto t1 = std::chrono::high_resolution_clock::now();
            t2f += std::chrono::duration<double, std::micro>(t1 - t0).count();
        }
        t2f /= REPS;

        std::cout << "n=" << n
                  << "  getAllPerms=" << t_all   << " us"
                  << "  getPerm1="   << t1f      << " us"
                  << "  getPerm2="   << t2f      << " us"
                  << "  (perm #" << randNum << ")\n";

        csv << n << "," << t_all << "," << t1f << "," << t2f << "\n";
    }

    csv.close();
    std::cout << "\nTiming data saved to result/timing.csv\n";
    return 0;
}
