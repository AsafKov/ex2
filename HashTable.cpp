#include "ListNode.h"

template <typename K, typename T> //TODO: Key is no generic for our hashing function (?)
class HashMap{
private:
    int current_size;
    int actual_size;
    const float load_factor = .75f;
    float current_hash_parameter;
    ListNode<K, T> **arr;
public:
    HashMap(int k): current_size(k){
        arr = new T[k];
        current_hash_parameter = (float)(rand()) / ((float)(RAND_MAX));
    }

    ~HashMap(){
        //TODO
    }

    /**
     * Insert new object to the table, change table size if needed according to load_factor
     * @param object
     * @param key
     */
    void insert(T object, int key){
        int index = hashing(key, current_size);
        auto *node = new ListNode<K, T>(key, object);
        chain(node, index);

        actual_size++;

        if((float) actual_size / current_size > load_factor){
            rehash(2);
        }
    }

    /**
     * Remove an object from the table, change table size if needed according to load_factor
     * @param key
     */
    void remove(int key){
        int index = hashing(key);
        ListNode<K, T> *temp = arr[index];

        while(temp != nullptr){
            if(temp->getKey() == key){
                if(temp->getPrev() != nullptr){
                    temp->getPrev()->setNext(temp->getNext());
                } else {
                    arr[index] = temp->getNext();
                }
                delete temp;
                break;
            }
            temp = temp->getNext();
        }

        if((float) actual_size / current_size < load_factor){
            rehash(.5f);
        }
    }

    /**
     * Insert a node to the list within the index in arr
     * @param node
     * @param index
     */
    void chain(ListNode<K, T> *node, int index){
        ListNode<K, T> temp = arr[index];
        if(arr[index] != nullptr){
            while(temp != nullptr){
                temp = temp.getNext();
            }
            temp.setNext(node);
        } else {
            arr[index] = node;
        }
    }

    /**
     * Puts all the object nodes inside a regular array
     * @param temp_arr
     */
    void toFlatArray(T ***temp_arr){
        ListNode<K, T> *temp_node;
        int index = 0, arr_index = 0;
        while(index < actual_size && arr_index < current_size){
            temp_node = arr[arr_index];
            if(temp_node != nullptr){
                *temp_arr[index] = temp_node;
                index++;
                temp_node = temp_node->getNext();
                while (temp_node != nullptr){
                    *temp_arr[index] = temp_node;
                    index++;
                    temp_node = temp_node->getNext();
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
    void rehash(float size_factor){
        T **temp_arr = new T[actual_size];
        toFlatArray(temp_arr);

        delete[] arr;
        current_size *= size_factor;
        arr = new T[current_size];

        transfer(&temp_arr);
        delete[] temp_arr;
    }

    /**
     * Transfer the nodes from the flat nodes array into the new hashtable array
     * @param temp_arr
     */
    void transfer(T ***temp_arr){
        ListNode<K, T> *temp_node;
        int new_index;
        for(int i=0; i < actual_size; i++){
            temp_node = temp_arr[i];
            if(temp_node == nullptr){
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
    int hashing(int key){
        float val = (float) key * current_hash_parameter;
        return (int)(current_size * int(val));
    }
};