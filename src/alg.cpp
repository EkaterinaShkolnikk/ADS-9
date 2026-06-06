// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <locale>
#include <cstdlib>
#include <cmath>
#include "tree.h"

static void collectPerms(const std::shared_ptr<TreeNode>& node,
                         std::vector<char>& current,
                         std::vector<std::vector<char>>& result) {
    current.push_back(node->value);

    if (node->children.empty()) {
        // Лист — текущий путь от корня является полной перестановкой
        result.push_back(current);
    } else {
        for (const auto& child : node->children) {
            collectPerms(child, current, result);
        }
    }
 
    current.pop_back();
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
    std::vector<std::vector<char>> result;
    std::vector<char> current;

    for (const auto& root : tree.getRoots()) {
        collectPerms(root, current, result);
    }

    return result;
}

std::vector<char> getPerm1(PMTree& tree, int num) {
    if (num <= 0) return {};
 
    auto all = getAllPerms(tree);
 
    if (num > static_cast<int>(all.size())) return {};
 
    return all[num - 1];
}

static long long factorial(int n) {
    long long f = 1;
    for (int i = 2; i <= n; ++i) f *= i;
    return f;
}

std::vector<char> getPerm2(PMTree& tree, int num) {
    if (num <= 0) return {};

    int depth = tree.depth();
    long long total = factorial(depth);

    if (num > total) return {};

    std::vector<char> result;
    long long idx = num - 1;  // переводим в 0-based индекс

    const std::vector<std::shared_ptr<TreeNode>>* level = &tree.getRoots();
    std::shared_ptr<TreeNode> current;
 
    for (int lvl = depth; lvl >= 1; --lvl) {

        long long subtreeSize = factorial(lvl - 1);

        long long childIdx = idx / subtreeSize;
        idx = idx % subtreeSize;

        current = (*level)[childIdx];
        result.push_back(current->value);

        level = &current->children;
    }

    return result;
}
