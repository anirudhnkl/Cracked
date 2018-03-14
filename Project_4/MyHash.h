// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.


#ifndef MYHASH
#define MYHASH

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;

private:
    struct Node
    {
        KeyType   key;
        ValueType value;
        Node *    next;
    };
    
    Node ** hashTable;
    int numBuckets;
    int numItems;
    double maxLoadFactor = 0.5;
    
    int getBucketNumber(const KeyType & key) const;
    void init();
    void clearTable();
    void resize();
    void insert(int pos, const KeyType& key, const ValueType& value);
};

#endif // MYHASH_INCLUDED

template <class KeyType, class ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor): numItems(0), numBuckets(100), maxLoadFactor(maxLoadFactor)
{
    if(maxLoadFactor <= 0)
        this->maxLoadFactor = 0.5;
    else if(maxLoadFactor > 2)
        this->maxLoadFactor = 2;
    init();
};

template<class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::clearTable()
{
    Node * current = nullptr;
    Node * next = nullptr;
    
    for(int i = 0; i < numBuckets; i++)
    {
        current = hashTable[i];
        while(current != nullptr)
        {
            next = current->next;
            delete current;
            current = next;
        }
    }
    
    delete [] hashTable; //is this needed or no???
}

template<class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::init()
{
    hashTable = new Node * [numBuckets];
    
    numItems = 0;
    
    for(int i = 0; i < numBuckets; i++)
        hashTable[i] = nullptr;
};

template <class KeyType, class ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
    clearTable();
};

template <class KeyType, class ValueType>
int MyHash<KeyType, ValueType>::getBucketNumber(const KeyType & key) const
{
    unsigned int hash(const KeyType & key);
    
    int h = hash(key) % numBuckets;
    return h;
};

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    clearTable();
    numBuckets = 100;
    init();
};

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::insert(int pos, const KeyType& key, const ValueType& value)
{
    Node * current = hashTable[pos];
    bool exists = false;
    
    if(current == nullptr)
    {
        Node * node = new Node;
        node->key = key;
        node->value = value;
        node->next = nullptr;
        hashTable[pos] = node;
        numItems++;
    }
    else
    {
        Node * prev;
        while(current != nullptr)
        {
            if(current->key == key)
            {
                current->value = value;
                exists = true;
                break;
            }
            prev = current;
            current = current->next;
        }
        
        if(!exists)
        {
            Node * node = new Node;
            node->key = key;
            node->value = value;
            node->next = nullptr;
            prev->next = node;
            numItems++;
        }
    }
};

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    insert(getBucketNumber(key), key, value);
    
    if(getLoadFactor() > maxLoadFactor)
        resize();
};

template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::resize()
{
    Node ** tempTable = hashTable;
    numBuckets = 2*numBuckets;
    init();
    
    Node * current = nullptr;
    
    for(int i = 0; i < numBuckets/2; i++)
    {
        current = tempTable[i];
        while(current != nullptr)
        {
            insert(getBucketNumber(current->key), current->key, current->value);
            current = current->next;
        }
    }
    
    current = nullptr;
    Node * next = nullptr;
    
    for(int i = 0; i < numBuckets/2; i++)
    {
        current = tempTable[i];
        while(current != nullptr)
        {
            next = current->next;
            delete current;
            current = next;
        }
    }
    
    delete  [] tempTable; //needed?
};

template <class KeyType, class ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
    return numItems;
};

template <class KeyType, class ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
    return numItems/(1.0 * numBuckets);
};

template <class KeyType, class ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    int h = getBucketNumber(key);
    Node * current = hashTable[h];
    
    while(current != nullptr)
    {
        if(current->key == key)
            return &(current->value);
        
        current = current->next;
    }
    
    return nullptr;
};

//#ifndef MYHASH_INCLUDED
//#define MYHASH_INCLUDED
//
//#include <unordered_map>  // YOU MUST NOT USE THIS HEADER IN CODE YOU TURN IN
//#include <algorithm>
//
//// In accordance with the spec, YOU MUST NOT TURN IN THIS CLASS TEMPLATE,
//// since you are not allowed to use any STL containers, and this
//// implementation uses std::unordered_map.
//
//// This code is deliberately obfuscated.
//
//// If you can not get your own MyHash class template working, you may use
//// this one during development in order to let you proceed with implementing
//// the other classes for this project; you can then go back to working on
//// fixing your own MyHash class template.
//
//template<typename KeyType, typename ValueType>
//class MyHash
//{
//public:
//    MyHash(const MyHash&) = delete;
//    MyHash& operator=(const MyHash&) = delete;
//    using O=KeyType;using maxloadfactor=float;using O10=int;void reset(){
//        maxloadfactor max_1oad_factor=l01.max_load_factor();l01.clear();l01.
//        max_load_factor(max_1oad_factor);l01.rehash(doub1e+doub1e);}using String=
//    double;using l0=ValueType;using l1O=O10 const;MyHash(String d0uble=doub1e/10):
//    l01(doub1e+doub1e){l01.max_load_factor(std::min<maxloadfactor>(doub1e/5/5,
//                                                                   std::max<maxloadfactor>((double)doub1e/(doub1e+doub1e),d0uble)));}using l10=O
//    const;using Const=MyHash<O,l0>;String getLoadFactor()const{return l01.
//        load_factor();}using ll0=l0 const;O10 getNumItems()const{return l01.size(
//            );}using l00=Const const;void associate(l10&Using,ll0&first){l01[Using]=
//                first;}using l1=std::unordered_map<O,l0>;ll0*find(l10&l11)const{auto first(l01.
//                                                                                           find(l11));return(first!=l01.end()?&first->second:0);}l0*find(l10&l01){return(
//                                                                                                                                                                         l0*)(*(l00*)(this)).find(l01);}private:static l1O doub1e{50};l1 l01;
//};

//#endif // MYHASH_INCLUDED

