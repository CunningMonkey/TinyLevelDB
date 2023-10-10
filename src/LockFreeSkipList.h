#include <atomic>
#include <cstdlib>
#include <limits>
#include <memory>
#include <cassert>

const int MAX_LEVEL = 2;

template <typename Key, typename Value, typename Comparator>
class LockFreeSkipList
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

public:
    LockFreeSkipList(const Comparator &cmp = Comparator())
        : _head(Node(0, 0)), _tail(Node(0, 0)), _comparator(cmp)
    {
        for (int i = 0; i < MAX_LEVEL; ++i)
        {
            _head.nexts[i] = &_tail;
        }
    }

    ~LockFreeSkipList() {}

    Value *get(const Key &key);
    void put(const Key &key, const Value &value);

    bool remove(const Key &key);

private:
    int randomLevel()
    {
        int lvl = 1;
        while (std::rand() % 2 && lvl < MAX_LEVEL)
        {
            lvl++;
        }
        return lvl;
    }

    Node *findLessOrEqualNode(const Key &key)
    {
        int level = MAX_LEVEL;
        Node *n = _head.nexts[level - 1];
        Node *prev = &_head;
        for (int level = MAX_LEVEL; level > 0; --level)
        {
            while (true)
            {
                if (n == &_head)
                {
                    prev = n;
                    n = n->nexts[level - 1];
                }
                if (n == &_tail)
                {
                    n = prev;
                    break;
                }
                int tmp = _comparator(n->key, key);
                if (tmp == 1)
                {
                    n = prev;
                    break;
                }
                if (tmp == 0)
                {
                    return n;
                }
                if (tmp == -1)
                {
                    prev = n;
                    n = n->nexts[level - 1];
                }
            }
        }
        return n;
    }
};

template <typename Key, typename Value, typename Comparator>
Value *LockFreeSkipList<Key, Value, Comparator>::get(const Key &key)
{
    Node *n = findLessOrEqualNode(key);
    int tmp = _comparator(n->key, key);
    assert(n != nullptr && n != &_tail);
    if (n == &_head)
    {
        return nullptr;
    }

    if (tmp == 0)
    {
        return &n->value;
    }
    return nullptr;
}

template <typename Key, typename Value, typename Comparator>
void LockFreeSkipList<Key, Value, Comparator>::put(const Key &key, const Value &value)
{
    Node *n = findLessOrEqualNode(key);
    int tmp = _comparator(n->key, key);
    assert(n != nullptr && n != &_tail);
    int level = randomLevel();

    if (n != &_head && tmp == 0)
    {
        n->value = value;
        return;
    }

    Node *new_node = new Node(key, value);
    for (int i = 1; i <= level; i++)
    {
        Node *next = n->nexts[i - 1];
        n->nexts[i - 1] = new_node;
        new_node->nexts[i - 1] = next;
    }
}

template <typename Key, typename Value, typename Comparator>
bool LockFreeSkipList<Key, Value, Comparator>::remove(const Key &key)
{
    int level = MAX_LEVEL;
    Node *n = _head.nexts[level - 1];
    Node *prev = &_head;
    for (int level = MAX_LEVEL; level > 0; --level)
    {
        while (true)
        {
            if (n == &_head)
            {
                prev = n;
                n = n->nexts[level - 1];
            }
            if (n == &_tail)
            {
                n = prev;
                break;
            }
            int tmp = _comparator(n->key, key);
            if (tmp == 1)
            {
                n = prev;
                break;
            }
            if (tmp == 0)
            {
                for (int i = 0; i < MAX_LEVEL; ++i)
                {
                    if (n->nexts[i] != nullptr)
                    {
                        prev->nexts[i] = n->nexts[i];
                    }
                }
                delete n;
                return true;
            }
            if (tmp == -1)
            {
                prev = n;
                n = n->nexts[level - 1];
            }
        }
    }
    return false;
}
