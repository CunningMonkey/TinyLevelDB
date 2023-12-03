#include "version.h"
#include "comparator.h"
#include <cstdint>
#include <sstream>
#include <vector>

SSTableMetaData DecodeSSTableMetaData(const char *s, size_t &length) {
    size_t number = *(reinterpret_cast<const size_t *const>(s));
    s += sizeof(size_t);
    size_t start_key_size = *(reinterpret_cast<const size_t *const>(s));
    s += sizeof(size_t);
    std::string start_key(s, start_key_size);
    s += start_key_size;
    size_t end_key_size = *(reinterpret_cast<const size_t *const>(s));
    s += sizeof(size_t);
    std::string end_key(s, end_key_size);
    s += end_key_size;
    length -= (sizeof(size_t) * 3 + start_key_size + end_key_size);
    return SSTableMetaData(number, start_key, end_key);
}

bool Version::RestoreVersion(std::string db, uint64_t fileNumber) {
    std::string manifestFilename =
        db + "/MANIFEST_" + std::to_string(fileNumber);
    std::ifstream inputFile(manifestFilename);
    if (!inputFile.is_open()) {
        std::cerr << "Cannot open " << manifestFilename << std::endl;
        return false;
    }
    std::stringstream buffer;
    buffer << inputFile.rdbuf();
    std::string content = buffer.str();
    inputFile.close();

    const char *s = content.c_str();
    DecodeSSTableMetaDatas(s, content.size());

    return true;
}

uint64_t Version::NextSSTableFileIndex() { return _max_number + 1; }

void Version::SetNextNumber() { ++_max_number; }

void Version::AddNewTable(uint8_t level, uint64_t fileNumber, std::string start,
                          std::string end) {
    _stables[level].emplace_back(fileNumber, std::move(start), std::move(end));
}

uint64_t Version::FindSSTable(const Slice &key) {
    KeyComparator cmp;
    for (const auto &pair : _stables) {
        for (auto it = pair.second.rbegin(); it != pair.second.rend(); ++it) {
                        
        }
    }
    return 0;
}

bool Version::DecodeSSTableMetaDatas(const char *s, size_t length) {

    _sequence_number = *(reinterpret_cast<uint64_t const *>(s));
    s += sizeof(uint64_t);

    while (s && length > 0) {
        uint8_t level = *(reinterpret_cast<const uint8_t *const>(s));
        s += sizeof(uint8_t);
        SSTableMetaData data = DecodeSSTableMetaData(s, length);
        _stables[level].push_back(std::move(data));
        if (data._fileNumber > _max_number) {
            _max_number = data._fileNumber;
        }
    }
    return true;
}

void Version::SetSequenceNumber(uint64_t sequence_number) {
    _sequence_number = sequence_number;
}

std::string Version::EncodeSSTableMetaDatas() {
    std::string buffer;
    buffer.append(reinterpret_cast<char *>(&_sequence_number),
                  sizeof(_sequence_number));
    for (auto &pair : _stables) {
        for (auto &stable : pair.second) {
            buffer.append(reinterpret_cast<const char *>(&(pair.first)),
                          sizeof(pair.first));
            buffer.append(reinterpret_cast<const char *>(&(stable._fileNumber)),
                          sizeof(stable._fileNumber));
            const size_t start_key_size = stable._start.size();
            const size_t end_key_size = stable._end.size();
            buffer.append(reinterpret_cast<const char *>(&start_key_size),
                          sizeof(size_t));
            buffer.append(stable._start.c_str(), start_key_size);
            buffer.append(reinterpret_cast<const char *>(&end_key_size),
                          sizeof(size_t));
            buffer.append(stable._end.c_str(), end_key_size);
        }
    }
    return buffer;
}
