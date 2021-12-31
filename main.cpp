#include "HashTable.cpp"

bool test_hash_table(){
    int size = 10000;
    HashTable<int, int> table = HashTable<int, int>(10);
    int *arr = new int[size];
    int val;
    for(int i=0; i<size; i++){
        val = rand()%10000;
        arr[i] = i;
        table.insert(i, i);
    }
    table.printTable();

    int newsSize = size;
    for(int i=0; i<size; i++){
        if(arr[i] % 2 == 0) {
            table.remove(arr[i]);
            newsSize--;
        }
    }
    table.printTable();

    for(int i=0; i<newsSize/2; i++){
        table.remove(arr[i]);
    }

    table.printTable();
    return true;
}

int main() {
    test_hash_table();
    return 0;
}


