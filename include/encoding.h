#pragma once

#include <cstdint>
#include <string>

enum ValueType
{
    kTypeDeletion = 0x0,
    kTypeValue = 0x1
};

const inline char *GetVarint32PtrFallback(const char *p, const char *limit,
                                          uint32_t *value)
{
    uint32_t result = 0;
    for (uint32_t shift = 0; shift <= 28 && p < limit; shift += 7)
    {
        uint32_t byte = *(reinterpret_cast<const uint8_t *>(p));
        p++;
        if (byte & 128)
        {
            // More bytes are present
            result |= ((byte & 127) << shift);
        }
        else
        {
            result |= (byte << shift);
            *value = result;
            return reinterpret_cast<const char *>(p);
        }
    }
    return nullptr;
}

// 变长编码，每8位为一组，最高位用来表示是否为最后一个字节，低7位用来表示数值
inline const char *GetVarint32Ptr(const char *p, const char *limit,
                                  uint32_t *value)
{
    if (p < limit)
    {
        uint32_t result = *(reinterpret_cast<const uint8_t *>(p));
        if ((result & 128) == 0)
        {
            *value = result;
            return p + 1;
        }
    }
    return GetVarint32PtrFallback(p, limit, value);
};

