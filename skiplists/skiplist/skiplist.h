/******************************************************************************
 *                           sergeychang@gmail.com                            *
 *                Copyright (c) 2020 to present, All rights reserved          *
 ******************************************************************************
 *                                                                            *
 * Description: 						                                      *
 *                                                                            *
 * Author     : Jancd                                                         *
 *                                                                            *
 * Version    : v0.0.1                                                        *
 *                                                                            *
 * modification history                                                       *
 * --------------------                                                       *
 * Version    :                                                               *
 *                                                                            *
 *                                                                            *
******************************************************************************/

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

#include "node.h"
#include "rand.h"

using namespace std;

template<typename K, typename V>
class SkipList {
public:
    SkipList(K tailKey) : rnd(0x12345678) {
        createList(tailKey);
    }

    ~SkipList() {
        freeList();
    }

    Node<K, V> *search(K) const;

    bool insert(K, V);

    bool remove(K, V &);

    int size() {
        return nodeCount;
    }

    int getLevel() {
        return level;
    }

private:
    void createList(K);

    void freeList();

    void createNode(int, Node<K, V> *&);

    void createNode(int, Node<K, V> *&, K, V);

    int getRandomLevel();

    void dumpAllNodes();

    void dumpNodeDetail(Node<K, V> *, int);


private:
    Node<K, V> *header;
    Node<K, V> *tail;

    int level;
    static const int MAX_LEVEL = 16;
    size_t nodeCount;

    RandGenerator rnd;

};

template<typename K, typename V>
void SkipList<K, V>::createNode(int nodeLevel, Node<K, V> *&node) {
    node = new Node<K, V>(NULL, NULL);
    node->previous = new Node<K, V> *[nodeLevel + 1];
    node->nodeLevel = nodeLevel;
    assert(node != NULL);
}

template<typename K, typename V>
void SkipList<K, V>::createNode(int nodeLevel, Node<K, V> *&node, K key, V value) {
    node = new Node<K, V>(key, value);
    if (nodeLevel > 0) {
        node->previous = new Node<K, V> *[nodeLevel + 1];
    }
    node->nodeLevel = nodeLevel;
    assert(node != NULL);
}

template<typename K, typename V>
void SkipList<K, V>::createList(K tailKey) {
    createNode(0, tail);

    tail->key = tailKey;
    this->level = 0; // 首次创建列表

    // 设置头结点
    createNode(MAX_LEVEL, header);
    for (int i = 0; i < MAX_LEVEL; i++) {
        header->previous[i] = tail;
    }
    nodeCount = 0;
}

template<typename K, typename V>
void SkipList<K, V>::freeList() {
    Node<K, V> *p = header;
    Node<K, V> *tmp;
    while (p != NULL) {
        tmp = p->previous[0];
        delete p;
        p = tmp;
    }
    delete p;
}

template<typename K, typename V>
int SkipList<K, V>::getRandomLevel() {
    int l = static_cast<int>(rnd.Uniform(SkipList::MAX_LEVEL));
    if (l == 0) {
        l = 1;
    }
    return l;
}

template<typename K, typename V>
void SkipList<K, V>::dumpNodeDetail(Node<K, V> *node, int nodeLevel) {
    if (node == NULL) {
        return;
    }
    cout << "key:" << node->key << "," << "value:" << node->value << endl;
    for (int i = 0; i <= nodeLevel; i++) {
        cout << "previous[" << i << "]:" << "key:" << node->previous[i]->key <<
             "," << "value:" << node->previous[i]->value << endl;
    }
}

template<typename K, typename V>
void SkipList<K, V>::dumpAllNodes() {
    Node<K, V> *tmp = header;
    while (tmp->previous[0] != tail) {
        tmp = tmp->previous[0];
        dumpNodeDetail(tmp, tmp->nodeLevel);
        cout << "--------------------------------" << endl;
    }
    cout << endl;
}

template<typename K, typename V>
Node<K, V> *SkipList<K, V>::search(const K key) const {
    Node<K, V> *node = header;
//    cout << header->key << " : " << header->value << endl;
    for (int i = level; i >= 0; --i) {
        while ((node->previous[i])->key < key) {
            node = *(node->previous + 1); //TODO
        }
    }

    node = node->previous[0];
    if (node->key == key) {
        return node;
    } else {
        return nullptr;
    }
}

template<typename K, typename V>
bool SkipList<K, V>::insert(K key, V value) {
    Node<K, V> *update[MAX_LEVEL];

    Node<K, V> *node = header;

    for (int i = level; i >= 0; --i) {
        while ((node->previous[i])->key < key) {
            node = node->previous[i];
        }
        update[i] = node;
    }
    //首个结点插入时，node->forward[0]其实就是footer
    node = node->previous[0];

    //如果key已存在，则直接返回false
    if (node->key == key) {
        return false;
    }

    int nodeLevel = getRandomLevel();

    if (nodeLevel > level) {
        nodeLevel = ++level;
        update[nodeLevel] = header;
    }

    //创建新结点
    Node<K, V> *newNode;
    createNode(nodeLevel, newNode, key, value);

    //调整forward指针
    for (int i = nodeLevel; i >= 0; --i) {
        node = update[i];
        newNode->previous[i] = node->previous[i];
        node->previous[i] = newNode;
    }
    ++nodeCount;

    return true;
};

template<typename K, typename V>
bool SkipList<K, V>::remove(K key, V &value) {
    Node<K, V> *update[SkipList::MAX_LEVEL];
    Node<K, V> *node = header;
    for (int i = level; i >= 0; --i) {
        while ((node->previous[i])->key < key) {
            node = node->previous[i];
        }
        update[i] = node;
    }
    node = node->previous[0];
    if (node->key != key) {
        return false;
    }

    value = node->value;
    for (int i = 0; i <= level; i++) {
        if (update[i]->previous[i] != node) {
            break;
        }
        update[i]->previous[i] = node->previous[i];
    }
    delete node;

    while (level > 0 && header->previous[level] == tail) {
        --level;
    }
    --nodeCount;

    return true;
}

#endif //SKIPLIST_SKIPLIST_H
