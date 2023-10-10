#include <atomic>
#include <cstdlib>
#include <limits>
#include <memory>

const int MAX_LEVEL = 8;

template <typename Key, typename Value, typename Comparator = std::less<Key>>
class LockFreeSkipList
{
private:
    struct Node {
        Key key;
        Value value;
        Node* nexts[MAX_LEVEL];

        Node(Key key, Value value) : key(key), value(value)
        {
            for (int i = 0; i < MAX_LEVEL; ++i)
            {
                nexts[i] = nullptr;
            }
        }
    };

    Node _head;
    Node _tail;
    Comparator _comparator;

public:
    LockFreeSkipList(const Comparator& cmp=Comparator())
     : _head(Node(0, 0)), _tail(Node(0, 0)), _comparator(cmp){}
    
    ~LockFreeSkipList() {}

    Value* get(const Key& key);
    bool put(const Key& key, const Value& value);

    bool remove(const Key& key);

private:
    int randomLevel() {
        int lvl = 1;
        while (std::rand() % 2 && lvl < MAX_LEVEL)
        {
            lvl++;
        }
        return lvl;
    }
};

template<typename Key, typename Value, typename Comparator>
Value* LockFreeSkipList<Key, Value, Comparator>::get(const Key& key) {
    int level = MAX_LEVEL;
    Node* n = _head.nexts[level-1];
    Node* prev = _head;
    while(level > 0) {
        while(n != nullptr) {
            int tmp = _comparator(n->key, key)
            if (tmp == 0)
            {
                return &n->value;
            } else if (tmp == 1) {
                n = prev;
                break;
            } else {
                prev = n;
                n = n->nexts[level-1];
            }
        }
        --level;
    }
    return nullptr;
}

template <typename Key, typename Value, typename Comparator>
bool LockFreeSkipList<Key, Value, Comparator>::put(const Key &key, const Value &value)
{

    return false;
}

template <typename Key, typename Value, typename Comparator>
bool LockFreeSkipList<Key, Value, Comparator>::remove(const Key &key)
{
    int level = MAX_LEVEL;
    Node* n = _head.nexts[level-1];
    Node* prev = _head;
    while(level > 0) {
        while(n != nullptr) {
            int tmp = _comparator(n->key, key)
            if (tmp == 0)
            {
                n->value = value;
            } else if (tmp == 1) {
                n = prev;
                break;
            } else {
                prev = n;
                n = n->nexts[level-1];
            }
        }
        --level;
    }
    return false;
}
