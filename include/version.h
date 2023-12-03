#pragma once
#include "comparator.h"
#include "slice.h"
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct SSTableMetaData {
    uint64_t _fileNumber;
    std::string _start;
    std::string _end;

    SSTableMetaData(uint64_t fileNumber, std::string start, std::string end)
        : _fileNumber(fileNumber), _start(std::move(start)),
          _end(std::move(end)) {}
};

SSTableMetaData DecodeSSTableMetaData(std::string, size_t& length);

class Version {
  public:
    Version() : _max_number(0) {}
    bool RestoreVersion(std::string db, uint64_t fileNumber);
    void AddNewTable(uint8_t level, uint64_t fileNumber, std::string start, std::string end);
    uint64_t NextSSTableFileIndex();
    void SetNextNumber();
    void SetSequenceNumber(uint64_t sequence_number);
    bool DecodeSSTableMetaDatas(const char* s, size_t length);
    std::string EncodeSSTableMetaDatas();
    uint64_t FindSSTable(const Slice& key); 
  private:
    std::map<uint8_t, std::vector<SSTableMetaData>> _stables;
    uint64_t _max_number;
    uint64_t _sequence_number;
    KeyComparator _cmp;
};
