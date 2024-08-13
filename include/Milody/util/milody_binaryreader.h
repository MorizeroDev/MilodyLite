#ifndef MILODY_MILODY_BINARYREADER_H
#define MILODY_MILODY_BINARYREADER_H

#include "milody_bitflip.h"
#include <Milody/common/milody_result.h>
#include <bit>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

namespace milody::util {
template <std::endian E>
class BinaryReader {
public:
    BinaryReader(const uint8_t* data, uint64_t dataSize) : data(data), dataSize(dataSize), cntIdx(0) {
    }

    template <bitflip::UInt T>
    inline Result<T, std::string> readUint() {
        auto g = remainsGuard<T>();
        if (g.isErr()) {
            return Err(g.unwrapErr());
        }
        T ret = 0;
        memcpy(&ret, data + cntIdx, sizeof(ret));
        if constexpr (std::endian::native != E) {
            ret = bitflip::bswap(ret);
        }
        cntIdx += sizeof(T);
        return Ok(ret);
    }

    inline Result<uint64_t, std::string> readUint64() {
        return readUint<uint64_t>();
    }

    inline Result<uint32_t, std::string> readUint32() {
        return readUint<uint32_t>();
    }

    inline Result<std::vector<uint8_t>, std::string> readBytes(uint64_t length) {
        if (remainLessThan(length)) {
            return Err(std::string("remains length is less than given string length"));
        }
        std::vector<uint8_t> buf(length);
        memcpy(buf.data(), data + cntIdx, length);
        auto ret = Ok(std::move(buf));
        cntIdx += length;
        return ret;
    }

    inline Result<std::string, std::string> readString(uint64_t length) {
        if (remainLessThan(length)) {
            return Err(std::string("remains length is less than given string length"));
        }
        auto ret = Ok(std::string(reinterpret_cast<const char* const>(data + cntIdx), length));
        cntIdx += length;
        return ret;
    }

    template <typename T>
    [[nodiscard]] inline Result<void, std::string> remainsGuard() {
        if (remainLessThan(sizeof(T))) {
            return Err(std::string("remains size is smaller than sizeof(") + typeid(T).name() + ")");
        }
        return Ok();
    }

    [[nodiscard]] inline Result<void, std::string> remainsGuard(uint64_t length) const {
        if (remainLessThan(length)) {
            std::stringstream s;
            s << "remains size is smaller than " << length;
            return Err(s.str());
        }
        return Ok();
    }

    [[nodiscard]] inline bool remainGreaterThan(uint64_t required) const {
        return remains() >= required;
    }

    [[nodiscard]] inline bool remainLessThan(uint64_t required) const {
        return !remainGreaterThan(required);
    }

    [[nodiscard]] inline uint64_t remains() const {
        return dataSize - cntIdx;
    }

    inline const uint8_t* cntPointer() {
        return data + cntIdx;
    }

    inline Result<void, std::string> skip(uint64_t length) {
        auto err = remainsGuard(length);
        if (err.isErr()) {
            return err;
        }
        cntIdx += length;
        return Ok();
    }


private:
    const uint8_t* data;
    uint64_t dataSize;
    uint64_t cntIdx;
};

} // namespace milody::util

#endif //MILODY_MILODY_BINARYREADER_H
