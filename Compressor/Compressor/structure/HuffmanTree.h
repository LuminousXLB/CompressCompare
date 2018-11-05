#pragma once

#include "common.h"

using namespace std;

template <class valType, class probType = double>
class HuffmanNodeBase {
    probType prob;

public:
    HuffmanNodeBase(probType _prob)
    {
        prob = _prob;
    }

    probType getProb()
    {
        return prob;
    }

    virtual bool isLeaf() = 0;
    virtual valType getVal()
    {
        throw "[ERROR] Calling getVal() at HuffmanNodeBase";
    }

    virtual HuffmanNodeBase*& operator[](size_t pos)
    {
        throw "[ERROR] Calling operator[] at HuffmanNodeBase";
    }
};

template <class valType, class probType = double>
class HuffmanNode : public HuffmanNodeBase<valType, probType> {
    array<HuffmanNodeBase<valType, probType>*, 2> children;

public:
    HuffmanNode(HuffmanNodeBase<valType, probType>* left, HuffmanNodeBase<valType, probType>* right)
        : HuffmanNodeBase<valType, probType>(left->getProb() + right->getProb())
    {
        children[0] = left;
        children[1] = right;
    }

    bool isLeaf()
    {
        return false;
    }

    HuffmanNodeBase<valType, probType>*& operator[](size_t pos)
    {
        return children[pos];
    }

    ~HuffmanNode()
    {
        delete children[0];
        delete children[1];
    }
};

template <class valType, class probType = double>
class HuffmanLeaf : public HuffmanNodeBase<valType, probType> {
    valType val;

public:
    HuffmanLeaf(probType _prob, valType _val)
        : HuffmanNodeBase<valType, probType>(_prob)
    {
        val = _val;
    }

    bool isLeaf()
    {
        return true;
    }

    valType getVal()
    {
        return val;
    };
};

template <class HNB>
class myComparison {
public:
    bool operator()(const HNB& lhs, const HNB& rhs) const
    {
        return (lhs->getProb() > rhs->getProb());
    }
};

template <class valType, class probType = double>
class HuffmanTree {
    using NodeBase = HuffmanNodeBase<valType, probType>;
    using HNode = HuffmanNode<valType, probType>;
    using HLeaf = HuffmanLeaf<valType, probType>;

    priority_queue<NodeBase*, vector<NodeBase*>, myComparison<NodeBase*>> pq;

public:
    HuffmanTree()
    {
    }

    void push(valType val, probType prob)
    {
        NodeBase* ptr = new HLeaf(prob, val);
        pq.push(ptr);
    }

    NodeBase* getRoot()
    {
        if (pq.size() == 1) {
            return pq.top();
        } else {
            return nullptr;
        }
    }

    NodeBase* build()
    {
        while (pq.size() > 1) {
            NodeBase* cl = pq.top();
            pq.pop();
            NodeBase* cr = pq.top();
            pq.pop();

            NodeBase* ptr = new HNode(cl, cr);
            pq.push(ptr);
        }

        return pq.top();
    }

    ~HuffmanTree()
    {
        delete pq.top();
    }
};
