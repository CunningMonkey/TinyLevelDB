#include <atomic>
#include <cstdlib>
#include <limits>
#include <memory>

const int MAX_LEVEL = 8;

template <typename Key, typename Value>
class LockFreeSkipList
{
private:
    struct Node {
        Key key;
        Value value;
        Node* forward[MAX_LEVEL];

        Node(Key key, Value value) : key(key), value(value)
        {
            for (int i = 0; i < MAX_LEVEL; ++i)
            {
                forward[i] = nullptr;
            }
        }
    };

    Node _head;
    Node _tail;

public:
    LockFreeSkipList() : _head(Node(0, 0)), _tail(Node(0, 0)){}
    
    ~LockFreeSkipList() {}

    bool contains(Key key) {return true;}

    bool insert(Key key, Value value) {return true;}

    bool remove(Key key) {return true;}

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
