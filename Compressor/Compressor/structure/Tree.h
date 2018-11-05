#pragma once

#include "common.h"

using namespace std;

template <class T, size_t CNT = 2>
class TreeNode {
private:
    T value;
    array<TreeNode<T, CNT>*, CNT> children;

public:
    TreeNode(T val)
        : value(val)
        , children({ nullptr })
    {
    }

    TreeNode(T val, const array<TreeNode<T, CNT>*, CNT>& children_list)
        : value(val)
        , children(children_list)
    {
    }

    TreeNode<T, CNT>* addChild(size_t idx, T val)
    {
        children[idx] = new TreeNode<T, CNT>(val);
        return children[idx];
    }

    TreeNode<T, CNT>* get(size_t idx)
    {
        return children[idx];
    }

    const T& getValue()
    {
        return value;
    }

    TreeNode<T, CNT>* operator[](size_t idx)
    {
        return children[idx];
    }

    ~TreeNode()
    {
        for (size_t i = 0; i < CNT; i++) {
            delete children[i];
        }
    }
};

template <class T, size_t CNT = 2>
class Tree {
private:
    TreeNode<T, CNT>* root;

public:
    Tree()
        : root(nullptr)
    {
    }

    Tree(T val)
    {
        root = new TreeNode<T, CNT>(val);
    }

    TreeNode<T, CNT>* getRoot()
    {
        return root;
    }

    ~Tree()
    {
        delete root;
    }
};
