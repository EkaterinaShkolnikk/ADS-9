// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_
 
#include <vector>
#include <memory>
#include <algorithm>


struct TreeNode {
    char value;
    std::vector<std::shared_ptr<TreeNode>> children;

    explicit TreeNode(char v) : value(v) {}
};

class PMTree {
 private:
    std::vector<std::shared_ptr<TreeNode>> roots;

    void buildTree(std::shared_ptr<TreeNode> node,
                   const std::vector<char>& remaining) {
        if (remaining.empty()) return;

        for (char c : remaining) {
            auto child = std::make_shared<TreeNode>(c);
            node->children.push_back(child);

            std::vector<char> next;
            for (char r : remaining) {
                if (r != c) next.push_back(r);
            }
            buildTree(child, next);
        }
    }

 public:

    explicit PMTree(const std::vector<char>& in) {

        std::vector<char> sorted = in;
        std::sort(sorted.begin(), sorted.end());

        for (char c : sorted) {
            auto root = std::make_shared<TreeNode>(c);
            roots.push_back(root);
 
            std::vector<char> remaining;
            for (char r : sorted) {
                if (r != c) remaining.push_back(r);
            }
            buildTree(root, remaining);
        }
    }

    const std::vector<std::shared_ptr<TreeNode>>& getRoots() const {
        return roots;
    }

    int depth() const {
        return static_cast<int>(roots.size());
    }
};

std::vector<std::vector<char>> getAllPerms(const PMTree& tree);

std::vector<char> getPerm1(PMTree& tree, int num);

std::vector<char> getPerm2(PMTree& tree, int num);
 
#endif
