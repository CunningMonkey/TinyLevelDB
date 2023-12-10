#pragma once
#include "block_reader.h"
#include "table_reader.h"
#include "table_builder.h"
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

SSTableMetaData DecodeSSTableMetaData(std::string, size_t &length);

class Version {
  public:
    Version() : _max_number(0) {}
    bool RestoreVersion(std::string db, uint64_t fileNumber);
    void AddNewTable(uint8_t level, uint64_t fileNumber, std::string start,
                     std::string end);
    uint64_t NextSSTableFileIndex();
    void SetNextNumber();
    void SetSequenceNumber(uint64_t sequence_number);
    bool DecodeSSTableMetaDatas(const char *s, size_t length);
    std::string EncodeSSTableMetaDatas();
    bool SearchInTable(const char *key, const uint64_t fileNumber, const std::string& db_path);
    bool Get(const Slice &key, const uint64_t sequence_num, std::string &value, const std::string &db_path);

  private:
    std::map<uint8_t, std::vector<SSTableMetaData>> _stables;
    uint64_t _max_number;
    uint64_t _sequence_number;
    KeyComparator _cmp;
};
