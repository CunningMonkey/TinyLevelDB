// db_imp.cpp
#include "db_imp.h"
#include "memtable.h"
#include "table_builder.h"
#include <fstream>
#include <iterator>
#include <memory>
#include <sstream>
#include <string>

DBImpl::DBImpl() {}

DBImpl::~DBImpl() {}

bool DBImpl::Open(const std::string name) {
    _memtable = new MemTable;
    _db = name;
    std::ifstream inputFile("CURRENT");
    if (!inputFile.is_open()) {
        std::cerr << "cannot open CURRENT file!" << std::endl;
        return false;
    }
    std::string line;
    if (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        if (iss >> _current) {
            std::cout << "The CURRENT is " << _current << std::endl;
        } else {
            std::cerr << "Invalid CURRENT!" << std::endl;
            return false;
        }
    } else {
        std::cerr << "The CURRENT is empty!" << std::endl;
        return false;
    }
    inputFile.close();
    _version.RestoreVersion(_db, _current);
    return true;
}

bool DBImpl::Put(const Slice &key, const Slice &value) {
    _memtable->Put(key, value, _sequence_num++);
    if (_memtable->ApproximateMemoryUsage() > _options.write_buffer_size) {
        {
            std::unique_lock<std::mutex> lk(_mtx);
            _cond_var.wait(lk,
                           [this] { return _immutable_memtable == nullptr; });
            _immutable_memtable = _memtable;
            _memtable = new MemTable();
        }
        _cond_var.notify_all();
    }
    return true;
}

bool DBImpl::Get(const Slice &key, std::string &value) {
    bool found = _memtable->Get(key, _sequence_num, value);
    if (found)
        return true;
    return false;
}

bool DBImpl::Delete(const Slice &key) {
    _memtable->Delete(key, _sequence_num++);
    return false;
}

bool DBImpl::Start() {
    std::thread compactionThread(&DBImpl::BackgroundCompaction, this);

    if (compactionThread.joinable()) {
        std::cout << "Start background compaction thread!" << std::endl;
        return true;
    }
    std::cout << "Fail to start background compaction thread!" << std::endl;
    return false;
}

void DBImpl::BackgroundCompaction() {
    while (true) {
        CompactLevel0Table();
    }
}

void DBImpl::CompactLevel0Table() {
    {
        std::unique_lock<std::mutex> lk(_mtx);
        _cond_var.wait(lk, [this] { return _immutable_memtable != nullptr; });
        uint64_t fileNumber = _version.NextSSTableFileIndex();
        std::string fileName = _db + "/MANIFEST_" + std::to_string(fileNumber);
        std::unique_ptr<TableBuilder> tableBuilder =
            std::make_unique<TableBuilder>(fileName);
        auto it = _immutable_memtable->NewIterator();
        it->SeekToFirst();
        Slice startKey = it->Value();
        Slice endKey;

        for (auto it = _immutable_memtable->NewIterator(); it->Valid();
             it->Next()) {
            Slice key = it->Key();
            Slice value = it->Value();
            tableBuilder->Add(key, value);
            endKey = key;
        }
        tableBuilder->Finish();
        delete _immutable_memtable;
        _version.AddNewTable(0, fileNumber, std::move(startKey.ToString()),
                             std::move(endKey.ToString()));
    }
    _cond_var.notify_all();
}

void DBImpl::FlushManifest() {
    ++_current;
    std::string fileName = _db + "/MANIFEST_" + std::to_string(_current);
    std::ofstream writeFile(fileName);
    
}
