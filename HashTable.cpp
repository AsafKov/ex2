#include "ListNode.h"
#include <cmath>

template <typename T>
class HashMap{
private:
    int current_size;
    float current_hash_parameter;
    ListNode<T> **arr;
    float *frac_parameter;
public:
    HashMap(int k): current_size(k){
        arr = new T[k];
        frac_parameter = new float[100];
        init_hash_parameters();
    }

    ~HashMap(){
        //TODO
    }

    void insert(T object, int key){
        int index = hashing(key);
        ListNode<T> temp = arr[index];
        if(arr[index] != nullptr){
            while(temp != nullptr){
                temp = temp.getNext();
            }
            temp.setNext(new ListNode<T>(object));
        } else {
            arr[index] = new ListNode<T>(object);
        }
    }

    int hashing(int key){
        float val = (float) key * current_hash_parameter;
        return (int)(current_size * int(val));
    }

    void init_hash_parameters(){

    }
};