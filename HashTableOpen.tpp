template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(int i) {
    // TODO
    if (i <= 0) {
        throw string("Hash table size must be greater than 0.");
    }

    M = i;
    ht = new LinkedList<Record>*[M];

    for (int j = 0; j < M; j++) {
        ht[j] = new LinkedList<Record>;
    }
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>::HashTableOpen(const HashTableOpen<Key, Val>& copyObj)
: M(0), ht(nullptr) {
    copy(copyObj);
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>& HashTableOpen<Key, Val>::operator=
(const HashTableOpen<Key, Val>& rightObj) {
    if (this != &rightObj) {
        clear();
        copy(rightObj);
    }
    return *this;
}

template <typename Key, typename Val>
HashTableOpen<Key, Val>::~HashTableOpen() {
    // TODO
    for (int i = 0; i < M; i++) {
        delete ht[i];
    }
    delete[] ht;
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::clear() {
    for (int i = 0; i < M; i++) {
        ht[i]->clear();
    }
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::copy(const HashTableOpen<Key, Val>& copyObj) {
    if (M != copyObj.M) {
        LinkedList<Record>** tmp = new LinkedList<Record>*[copyObj.M];
        if (M < copyObj.M) {
            for (int i = 0; i < M; i++) {
                tmp[i] = ht[i];
            }
            for (int i = M; i < copyObj.M; i++) {
                tmp[i] = new LinkedList<Record>;
            }
        }
        else if (M > copyObj.M) {
            for (int i = 0; i < copyObj.M; i++) {
                tmp[i] = ht[i];
            }
            for (int i = copyObj.M; i < M; i++) {
                delete ht[i];
            }
        }

        M = copyObj.M;
        delete[] ht;
        ht = tmp;
    }

    LinkedList<Record>* myList      = nullptr;
    LinkedList<Record>* copyList    = nullptr;
    int                 myListLen   = 0;
    int                 copyListLen = 0;

    for (int i = 0; i < M; i++) {
        myList      = ht[i];
        myListLen   = myList->getLength();
        copyList    = copyObj.ht[i];
        copyListLen = copyList->getLength();

        if (myListLen < copyListLen) {
            for (int j = 0; j < myListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
            for (int j = myListLen; j < copyListLen; j++) {
                if (myList->isEmpty()) {
                    myList->append(copyList->getElement(j));
                }
                else {
                    myList->insert(0, copyList->getElement(j));
                }
            }
        }
        else if (myListLen > copyListLen) {
            for (int j = 0; j < copyListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
            for (int j = copyListLen; j < myListLen; j++) {
                myList->remove(j);
            }
        }
        else {
            for (int j = 0; j < myListLen; j++) {
                myList->replace(j, copyList->getElement(j));
            }
        }
    }
}

template <typename Key, typename Val>
Val HashTableOpen<Key, Val>::find(const Key& k) const {
    // TODO
    int slot = hash(k);

    LinkedList<Record>* list = ht[slot];
    int len = list->getLength();

    for (int i = 0; i < len; i++) {
        Record r = list->getElement(i);
        if (r.k == k) {
            return r.v;
        }
    }

    throw string("find: key not found");
}

template <typename Key, typename Val>
int HashTableOpen<Key, Val>::hash(const Key& k) const {
    const int FOLD_LEN = 4;

    unsigned* ikey = (unsigned*) k.c_str();
    int ilen = k.length() / FOLD_LEN;

    unsigned sum = 0;

    for (int i = 0; i < ilen; i++) {
        sum += ikey[i];
    }

    int extra = k.length() - ilen * FOLD_LEN;

    char temp[FOLD_LEN];

    ikey    = (unsigned*) temp;
    ikey[0] = 0;

    for (int i = 0; i < extra; i++) {
        temp[i] = k[ilen * FOLD_LEN + i];
    }

    sum += ikey[0];

    int slot = sum % M;

    cout << k << "\thashes to slot " << slot << endl;

    return slot;
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::insert(const Key& k, const Val& v) {
    // TODO
    int slot = hash(k);

    Record newRecord(k, v);

    if (ht[slot]->isEmpty()) {
        ht[slot]->append(newRecord);
    }
    else {
        ht[slot]->insert(0, newRecord);
    }
}

template <typename Key, typename Val>
void HashTableOpen<Key, Val>::remove(const Key& k) {
    // TODO
    int slot = hash(k);

    LinkedList<Record>* list = ht[slot];
    int len = list->getLength();

    for (int i = 0; i < len; i++) {
        Record r = list->getElement(i);
        if (r.k == k) {
            list->remove(i);
            return;
        }
    }

    throw string("remove: key not found");
}

template <typename Key, typename Val>
int HashTableOpen<Key, Val>::size() const {
    // TODO
    int count = 0;

    for (int i = 0; i < M; i++) {
        count += ht[i]->getLength();
    }

    return count;
}
