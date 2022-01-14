#ifndef EX2_HASHTABLE_H
#define EX2_HASHTABLE_H

#include <iostream>
#include "ListNode.h"
#include <cmath>

template<typename K, typename T>
class HashTable {
private:
    int structure_size;
    int actual_size;
    const int INITIAL_SIZE = 101;
    const float load_factor_ceil = .75f;
    const float load_factor_floor = .25f;
    ListNode<K, T> **arr;

    /**
     * Insert a node to the list within the index in arr
     * @param node
     * @param index
     */
    void chain(ListNode<K, T> *node, int index) {
        ListNode<K, T> *temp = arr[index];
        if (temp != nullptr) {
            while (temp->getNext() != nullptr) {
                temp = temp->getNext();
            }
            temp->setNext(node);
            node->setPrev(temp);
        } else {
            arr[index] = node;
        }
    }

    /**
    * Puts all the object nodes inside a regular array
    * @param temp_arr
    */
    void toFlatArray(ListNode<K, T> ***temp_arr) {
        ListNode<K, T> *temp_node, *temp_next;
        int index = 0, arr_index = 0;
        while (index < actual_size && arr_index < structure_size) {
            temp_node = arr[arr_index];
            if (temp_node != nullptr) {
                (*temp_arr)[index] = temp_node;
                index++;
                temp_next = temp_node->getNext();
                temp_node->setNext(nullptr);
                temp_node = temp_next;
                while (temp_node != nullptr) {
                    temp_node->setPrev(nullptr);
                    (*temp_arr)[index] = temp_node;
                    index++;
                    temp_next = temp_node->getNext();
                    temp_node->setNext(nullptr);
                    temp_node = temp_next;
                }
            }
            arr[arr_index] = nullptr;
            arr_index++;
        }
    }

    /**
     * Changes the structure size according to size_factor and re-insert the nodes
     * @param size_factor - 2 / 0.5
     */
    void rehash(float size_factor) {
        auto **temp_arr = new ListNode<K, T> *[actual_size]();
        toFlatArray(&temp_arr);

        delete[] arr;
        structure_size *= size_factor;
        structure_size++;
        arr = new ListNode<K, T> *[structure_size]();

        transfer(&temp_arr);
        delete[] temp_arr;
    }

    /**
     * Transfer the nodes from the flat nodes array into the new hashtable array
     * @param temp_arr
     */
    void transfer(ListNode<K, T> ***temp_arr) {
        ListNode<K, T> *temp_node;
        int new_index;
        for (int i = 0; i < actual_size; i++) {
            temp_node = (*temp_arr)[i];
            if (temp_node == nullptr) {
                continue;
            }

            new_index = hashing(temp_node->getKey());
            chain(temp_node, new_index);
        }
    }

    /**
     * Returns the index for the key with the hashing function
     * @param key
     * @return
     */
    int hashing(int key) {
        return key % structure_size;
    }

    ListNode<K, T> *find(int key);

public:
    HashTable(const HashTable<T, K> &table) = delete;

    HashTable &operator=(const HashTable<K, T> &table) = delete;

    explicit HashTable() : actual_size(0) {
        structure_size = INITIAL_SIZE;
        arr = new ListNode<K, T> *[INITIAL_SIZE]();
    }

    ~HashTable() {
        ListNode<K, T> *next, *current;
        for(int i=0; i<structure_size; i++){
            next = arr[i];
            while(next != nullptr){
                current = next;
                next = next->getNext();
                current->setNext(nullptr);
                current->setPrev(nullptr);
                delete current;
            }
            arr[i] = nullptr;
        }

        delete[] arr;
        arr = nullptr;
    }


    T &get(int key);

    /**
     * Insert new object to the table, change table size if needed according to load_factor_ceil
     * @param object
     * @param key
     */
    void insert(int key, T object) {
        int index = hashing(key);
        auto *node = new ListNode<K, T>(key, object);
        chain(node, index);

        actual_size++;

        if ((float) actual_size / structure_size > load_factor_ceil) {
            rehash(2);
        }
    }

    /**
     * Remove an object from the table, change table size if needed according to load_factor_floor
     * @param key
     * @returns true if object was removed, false if object was never in the table
     */
    bool remove(int key) {
        int index = hashing(key);
        ListNode<K, T> *node = find(key);
        ListNode<K, T> *prev, *next;
        if (node != nullptr) {
            prev = node->getPrev();
            next = node->getNext();
            if (prev != nullptr) {
                prev->setNext(next);
                if (next != nullptr) {
                    next->setPrev(prev);
                }
            } else {
                arr[index] = next;
                if (next != nullptr) {
                    next->setPrev(nullptr);
                }
            }
            node->setPrev(nullptr);
            node->setNext(nullptr);
            actual_size--;

            if ((float) actual_size / structure_size < load_factor_floor) {
                rehash(.5f);
            }

            delete node;

            return true;
        }

        return false;
    }

    bool contains(int key){
        return find(key) != nullptr;
    }
};

template<typename K, typename T>
ListNode<K, T> *HashTable<K, T>::find(int key) {
    int index = hashing(key);
    ListNode<K, T> *temp = arr[index];
    while (temp != nullptr) {
        if (temp->getKey() == key) {
            return temp;
        }
        temp = temp->getNext();
    }

    return nullptr;
}

template<typename K, typename T>
T &HashTable<K, T>::get(int key) {
    return find(key)->getData();
}

#endif //EX2_HASHTABLE_H
