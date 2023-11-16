#pragma once

#include <cassert>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include "arena.h"
#include "random.h"

const int MAX_LEVEL = 5;

template <typename Key, typename Comparator>
class SkipList
{
private:
    struct Node
    {
        Key key;
        Node *nexts[MAX_LEVEL];

        Node(Key key) : key(key)
        {
            for (int i = 0; i < MAX_LEVEL; ++i)
            {
                nexts[i] = nullptr;
            }
        }

        Node *Next(int n)
        {
            assert(n >= 0);
            return nexts[n];
        }
    };

    Node _head;
    // Node _tail;
    Comparator _comparator;
    std::shared_mutex _mutex;
    Arena *const _arena;
    Random _rand;

public:
    SkipList(const Comparator &cmp, Arena *arena)
        : _arena(arena), _head(Node(0)), _comparator(cmp), _rand(0xdeadbeef)
    {
        std::srand(std::time(nullptr));
    }

    ~SkipList() {}

    Key *Get(const Key &key);
    void Put(const Key &key);
    bool Contains(const Key &key);
    Node *FindLast();

    void print();
    std::vector<Key *> iterate();

    class Iterator
    {
    public:
        explicit Iterator(const SkipList *list);

        bool Valid() const;
        const Key &key() const;
        void Next();
        void SeekToFirst();

    private:
        Node *_current;
        const SkipList *_skiplist;
    };

private:
    int randomLevel()
    {
        // Increase height with probability 1 in kBranching
        static const unsigned int kBranching = 4;
        int height = 1;
        while (height < MAX_LEVEL && _rand.OneIn(kBranching))
        {
            height++;
        }
        assert(height > 0);
        assert(height <= MAX_LEVEL);
        return height;
    }

    Node *findLessThan(const Key &key, Node *prevs[MAX_LEVEL])
    {
        Node *n = _head.nexts[MAX_LEVEL - 1];
        Node *prev = &_head;
        for (int level = MAX_LEVEL - 1; level >= 0; --level)
        {
            while (true)
            {
                if (n == &_head)
                {
                    n = _head.nexts[level];
                    prev = &_head;
                }
                if (n == nullptr || _comparator(n->key, key) >= 0)
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

    Node *newNode(const Key &key);
};

template <typename Key, typename Comparator>
inline SkipList<Key, Comparator>::Iterator::Iterator(const SkipList *list)
{
    _skiplist = list;
}

template <typename Key, typename Comparator>
inline bool SkipList<Key, Comparator>::Iterator::Valid() const
{
    return _current == nullptr;
}

template <typename Key, typename Comparator>
inline const Key &SkipList<Key, Comparator>::Iterator::key() const
{
    return _current->key;
}

template <typename Key, typename Comparator>
inline void SkipList<Key, Comparator>::Iterator::Next()
{
    assert(Valid());
    _current = _current->nexts[0];
}

template <typename Key, typename Comparator>
inline void SkipList<Key, Comparator>::Iterator::SeekToFirst()
{
    _current = (_skiplist->_head).nexts[0];
}

template <typename Key, typename Comparator>
Key *SkipList<Key, Comparator>::Get(const Key &key)
{
    std::shared_lock<std::shared_mutex> lock(_mutex);
    Node *prevs[MAX_LEVEL];
    Node *n = findLessThan(key, prevs);
    Node *next = n->nexts[0];
    if (next == nullptr)
    {
        return nullptr;
    }
    return &(next->key);
}

template <typename Key, typename Comparator>
void SkipList<Key, Comparator>::Put(const Key &key)
{
    std::lock_guard<std::shared_mutex> lock(_mutex);
    Node *prevs[MAX_LEVEL];
    Node *n = findLessThan(key, prevs);
    Node *next = n->nexts[0];

    int level = randomLevel();

    auto new_node = newNode(key);

    for (int i = 0; i < level; ++i)
    {
        next = prevs[i]->nexts[i];
        prevs[i]->nexts[i] = new_node;
        new_node->nexts[i] = next;
    }
}

template <typename Key, typename Comparator>
inline bool SkipList<Key, Comparator>::Contains(const Key &key)
{
    return !(Get(key) == nullptr);
}

template <typename Key, typename Comparator>
typename SkipList<Key, Comparator>::Node *SkipList<Key, Comparator>::FindLast()
{
    Node *n = &_head;
    while (n->nexts[0] != nullptr)
    {
        n = n->nexts[0];
    }
    return n;
}

template <typename Key, typename Comparator>
void SkipList<Key, Comparator>::print()
{
    for (int i = MAX_LEVEL - 1; i >= 0; --i)
    {
        std::cout << "head -> ";
        Node *n = _head.nexts[i];
        while (n && n != nullptr)
        {
            std::cout << n->key << " -> ";
            n = n->nexts[i];
        }
        std::cout << "tail" << std::endl;
    }
}

template <typename Key, typename Comparator>
std::vector<Key *> SkipList<Key, Comparator>::iterate()
{
    std::vector<Key *> keys;
    Node *current = _head.Next(0);

    while (current)
    {
        keys.push_back(&current->key);
        current = current->Next(0);
    }

    return keys;
}

template <typename Key, typename Comparator>
typename SkipList<Key, Comparator>::Node *SkipList<Key, Comparator>::newNode(
    const Key &key)
{
    char *_nodememory = _arena->AllocateAligned(sizeof(Node));
    return new (_nodememory) Node(key);
}
