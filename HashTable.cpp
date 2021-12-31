#include "Node.h"
#include <cmath>

template <typename T>
class HashMap{
private:
    int current_size;
    float current_hash_parameter;
    T **arr;
    float *frac_parameter;
public:
    HashMap(int k): current_size(k){
        arr = new T[k];
        frac_parameter = new float[100];
    }

    ~HashMap(){
        //TODO
    }

    void insert(T object, int key){
        int index = hashing(key);
    }

    int hashing(int key){
        float val = (float) key * current_hash_parameter;
        return (int)(current_size * int(val));
    }
};