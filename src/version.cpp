#include "version.h"
#include <cstdint>
#include <vector>

SSTableMetaData DecodeSSTableMetaData(const char *s) {
    size_t number = *(reinterpret_cast<const size_t *const>(s));
    s += sizeof(size_t);
    size_t start_key_size = *(reinterpret_cast<const size_t *const>(s));
    s += sizeof(size_t);
    std::string start_key(s, start_key_size);
    s += start_key_size;
    size_t end_key_size = *(reinterpret_cast<const size_t *const>(s));
    s += start_key_size;
    std::string end_key(s, end_key_size);
    return SSTableMetaData(number, start_key, end_key);
}

bool Version::RestoreVersion(std::string db, uint64_t fileNumber) {
    std::string manifestFilename = db + "/MANIFEST_" + std::to_string(fileNumber);
    std::ifstream inputFile(manifestFilename);
    if (!inputFile.is_open()) {
        std::cerr << "Cannot open " << manifestFilename << std::endl;
        return false;
    }

    std::string line;
    
    while (std::getline(inputFile, line)) {
        const char *line_start = line.c_str();
        uint8_t level = *(reinterpret_cast<const uint8_t *const>(line_start));
        line_start += sizeof(uint8_t);
        SSTableMetaData data = DecodeSSTableMetaData(line_start);
        if (data._fileNumber > _max_number) {
            _max_number = data._fileNumber;
        }
        _stables[level].push_back(std::move(data));
    }

    inputFile.close();
    return true;
}


uint64_t Version::NextSSTableFileIndex() {
    return _max_number + 1;
}

void Version::SetNextNumber() {
    ++_max_number;
}

void Version::AddNewTable(uint8_t level, uint64_t fileNumber, std::string start, std::string end) {
    _stables[level].emplace_back(fileNumber, std::move(start), std::move(end));
}

std::string Version::DecodeSSTableMetaDatas() {
    
}
