#include "Node.h"

template <typename T>
class HashMap{
private:
    int currentSize;
    T **arr;
    float *frac_parameter;
public:
    HashMap(int k): currentSize(k){
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
        return key;
    }
};