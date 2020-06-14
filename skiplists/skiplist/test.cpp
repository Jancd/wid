#include <iostream>
#include "skiplist.h"

using namespace std;

int main(){
    SkipList<int, int> skipList(0x7fffffff);

    int length = 10;
    for (int i = 1; i <= length; ++i) {
        skipList.insert(i, i + 200);
    }
    cout << "The number of elements in skip list is:" << skipList.size() << endl;

    if (skipList.size() != length) {
        cout << "insert failure." << endl;
    } else {
        cout << "insert success." << endl;
    }

    // search test
    int value = -1;
    int key = 9;
    Node<int, int> *searchResult = skipList.search(key);
    if (searchResult != nullptr) {
        value = searchResult->getValue();
        cout << "search result for key " << key << ":" << value << endl;
    } else {
        cout << "search failure for key " << key << endl;
    }

    // reset value
    value = -1;

    key = 6;
    cout<<endl<<"start to remove"<<endl;
    bool removeResult = skipList.remove(key, value);
    if (removeResult) {
        cout << "removed node whose key is " << key << " and value is " << value << endl;
    } else {
        cout << "removed failure" << endl;
    }

    return 0;
}