#include "HashTable.cpp"

bool test_hash_table(){
    int size = 10000;
    HashTable<int, int> table;
    int *arr = new int[size];
    int val;
    bool isUnique;
    for(int i=0; i<size; i++){
        isUnique = false;
        while(!isUnique){
            isUnique = true;
            val = rand()%100000000;
            for(int j=0; j<=i; j++){
                if(arr[j] == val){
                    isUnique = false;
                    break;
                }
            }
        }
        arr[i] = val;
        table.insert(val, val);
    }
    table.printTable();

    for(int i=size/2; i<size; i++){
        table.remove(arr[i]);
        if(table.remove(arr[i])){
            std::cout << "Remove failed at val " << arr[i] << " index: " << i << "\n";
            return false;
        }
    }
    table.printTable();

    for(int i=size/4; i<size/2; i++){
        table.remove(arr[i]);
        if(table.remove(arr[i])){
            std::cout << "Remove failed at val " << arr[i] << " index: " << i << "\n";
            return false;
        }
    }

    table.printTable();
    for(int i=size/8; i<size/4; i++){
        table.remove(arr[i]);
        if(table.remove(arr[i])){
            std::cout << "Remove failed at val " << arr[i] << " index: " << i << "\n";
            return false;
        }
    }

    table.printTable();
    for(int i=size/16; i<size/8; i++){
        table.remove(arr[i]);
        if(table.remove(arr[i])){
            std::cout << "Remove failed at val " << arr[i] << " index: " << i << "\n";
            return false;
        }
    }

    table.printTable();
    for(int i=size/32; i<size/16; i++){
        table.remove(arr[i]);
        if(table.remove(arr[i])){
            std::cout << "Remove failed at val " << arr[i] << " index: " << i << "\n";
            return false;
        }
    }

    table.printTable();
    for(int i=size/64; i<size/32; i++){
        table.remove(arr[i]);
        if(table.remove(arr[i])){
            std::cout << "Remove failed at val " << arr[i] << " index: " << i << "\n";
            return false;
        }
    }

    table.printTable();
    for(int i=size/128; i<size/64; i++){
        table.remove(arr[i]);
        if(table.remove(arr[i])){
            std::cout << "Remove failed at val " << arr[i] << " index: " << i << "\n";
            return false;
        }
    }

    table.printTable();
    for(int i=size/256; i<size/128; i++){
        table.remove(arr[i]);
        if(table.remove(arr[i])){
            std::cout << "Remove failed at val " << arr[i] << " index: " << i << "\n";
            return false;
        }
    }

    table.printTable();
    for(int i=size/512; i<size/256; i++){
        table.remove(arr[i]);
        if(table.remove(arr[i])){
            std::cout << "Remove failed at val " << arr[i] << " index: " << i << "\n";
            return false;
        }
    }

    table.printTable();
    return true;
}

int main() {
    test_hash_table();
    return 0;
}


