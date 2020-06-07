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

#ifndef SKIPLIST_NODE_H
#define SKIPLIST_NODE_H

template<typename K, typename V>
class SkipList;

template<typename K, typename V>
class Node {
    friend class SkipList<K, V>;

public:
    Node() {}

    Node(K k, V v);

    ~Node();

    K getKey() const;

    V getValue() const;

private:
    K key;
    V value;
    Node<K, V> **previous;
    int nodeLevel;
};

template<typename K, typename V>
Node<K, V>::Node(const K k, const V v) {
    key = k;
    value = v;
}

template<typename K, typename V>
Node<K, V>::~Node() {
    delete[]previous;
}

template<typename K, typename V>
K Node<K, V>::getKey() const {
    return key;
}

template<typename K, typename V>
V Node<K, V>::getValue() const {
    return value;
}

#endif