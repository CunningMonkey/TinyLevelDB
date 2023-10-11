#include <atomic>
#include <cstdlib>
#include <limits>
#include <memory>
#include <cassert>
#include <iostream>
#include <mutex>

const int MAX_LEVEL = 5;

template <typename Key, typename Value, typename Comparator>
class SkipList
{
private:
    struct Node
    {
        Key key;
        Value value;
        Node *nexts[MAX_LEVEL];

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
    std::mutex _mutex;

public:
    SkipList(const Comparator &cmp = Comparator())
        : _head(Node(0, 0)), _tail(Node(0, 0)), _comparator(cmp)
    {
        std::srand(std::time(nullptr));
        for (int i = 0; i < MAX_LEVEL; ++i)
        {
            _head.nexts[i] = &_tail;
        }
    }

    ~SkipList() {}

    Value *get(const Key &key);
    void put(const Key &key, const Value &value);

    bool remove(const Key &key);

    void print();

private:
    int randomLevel()
    {
        int lvl = 1;
        while ((std::rand() & 1) && lvl < MAX_LEVEL)
        {
            lvl++;
        }
        return lvl;
    }

    Node *findLessThanKey(const Key &key, Node* prevs[MAX_LEVEL])
    {
        int level = MAX_LEVEL;
        Node *n = _head.nexts[level - 1];
        Node *prev = &_head;
        for (int level = MAX_LEVEL-1; level >= 0; --level)
        {
            while (true)
            {
                if (n == &_tail ||  _comparator(n->key, key) >= 0)
                {
                    n = prev;
                    prevs[level] = n;
                    break;
                }
                prev = n;
                n = n->nexts[level];
            }
        }
        return n;
    }
};

template <typename Key, typename Value, typename Comparator>
Value *SkipList<Key, Value, Comparator>::get(const Key &key)
{
    std::lock_guard<std::mutex> lock(_mutex);
    Node* prevs[MAX_LEVEL];
    Node *n = findLessThanKey(key, prevs);
    Node *next = n->nexts[0];
    assert(next != nullptr);
    if (next == &_tail) {
        return nullptr;
    }
    int tmp = _comparator(next->key, key);
    if (tmp == 0)
    {
        return &next->value;
    }
    return nullptr;
}

template <typename Key, typename Value, typename Comparator>
void SkipList<Key, Value, Comparator>::put(const Key &key, const Value &value)
{
    std::lock_guard<std::mutex> lock(_mutex);
    Node* prevs[MAX_LEVEL];
    Node *n = findLessThanKey(key, prevs);
    Node *next = n->nexts[0];
    assert(next != nullptr);
    if (next != &_tail && _comparator(next->key, key) == 0) {
        next->value = value;
        return;
    }

    int level = randomLevel();
    std::unique_ptr<Node> new_node(new Node(key, value));
    for (int i = 0; i < level; ++i)
    {
        next = prevs[i]->nexts[i];
        prevs[i]->nexts[i] = new_node.get();
        new_node->nexts[i] = next;
    }

    new_node.release();
}

template <typename Key, typename Value, typename Comparator>
bool SkipList<Key, Value, Comparator>::remove(const Key &key)
{
    std::lock_guard<std::mutex> lock(_mutex);
    Node* prevs[MAX_LEVEL];
    Node *n = findLessThanKey(key, prevs);
    Node *next = n->nexts[0];
    assert(next != nullptr);
    if (next == &_tail) {
        return false;
    }

    if (_comparator(next->key, key) == 0)
    {
        for (int level = 0; level < MAX_LEVEL; ++level) {
            if (next->nexts[level] != nullptr) {
                prevs[level]->nexts[level] = next->nexts[level];
            }
        }
        delete next;
        return true;
    }
    return false;
}

template <typename Key, typename Value, typename Comparator>
void SkipList<Key, Value, Comparator>::print()
{
    for (int i = MAX_LEVEL - 1; i >= 0; --i) {
        std::cout << "head-";
        Node* n = _head.nexts[i];
        while(n && n != &_tail) {
            std::cout << n->key << ":" << n->value << "--";
            n = n->nexts[i];
        }
        std::cout << "tail" << std::endl;
    }
}
