#ifndef MILODY_BINARYWRITER_H
#define MILODY_BINARYWRITER_H

#include "milody_bitflip.h"
#include <Milody/common/milody_result.h>
#include <bit>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

namespace milody::util {

template <std::endian E>
class BinaryWriter {
public:
    explicit BinaryWriter(std::vector<uint8_t>* target) {
        this->target = target->data();
        this->size = target->size();
        this->cntIdx = 0;
    }

    [[nodiscard]] inline Result<void, std::string> writeString(const std::string& data) {
        return writeBytes(reinterpret_cast<const uint8_t*>(data.c_str()), data.size());
    }

    template <milody::util::bitflip::UInt T>
    [[nodiscard]] inline Result<void, std::string> writeUint(T data) {
        auto g = remainsGuard<T>();
        if (g.isErr()) {
            return Err(g.unwrapErr());
        }
        if constexpr (std::endian::native != E) {
            data = milody::util::bitflip::bswap(data);
        }
        memcpy(target + cntIdx, &data, sizeof(T));
        cntIdx += sizeof(T);
        return Ok();
    }

    [[nodiscard]] inline Result<void, std::string> writeUint16(const uint16_t data) {
        return writeUint<uint16_t>(data);
    }

    [[nodiscard]] inline Result<void, std::string> writeUint32(const uint32_t data) {
        return writeUint<uint32_t>(data);
    }

    [[nodiscard]] inline Result<void, std::string> writeUint64(const uint64_t data) {
        return writeUint<uint64_t>(data);
    }

    [[nodiscard]] inline Result<void, std::string> writeInt16(const int16_t data) {
        return writeUint<uint16_t>(data);
    }

    [[nodiscard]] inline Result<void, std::string> writeInt32(const int32_t data) {
        return writeUint<uint32_t>(data);
    }

    [[nodiscard]] inline Result<void, std::string> writeInt64(const int64_t data) {
        return writeUint<uint64_t>(data);
    }

    [[nodiscard]] inline Result<void, std::string> writeBytes(const uint8_t* data, const uint64_t length) {
        auto g = remainsGuard(length);
        if (g.isErr()) {
            return Err(g.unwrapErr());
        }
        memcpy(target + cntIdx, data, length * sizeof(uint8_t));
        cntIdx += length;
        return Ok();
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
        return size - cntIdx;
    }

    [[nodiscard]] inline uint64_t used() const {
        return cntIdx;
    }


protected:
    uint8_t* target;
    uint64_t size;
    uint64_t cntIdx;
};

template <std::endian E>
class SeekableBinaryWriter : public BinaryWriter<E> {
public:
    explicit SeekableBinaryWriter(std::vector<uint8_t>* target) : BinaryWriter<E>(target) {
    }

    [[nodiscard]] Result<void, std::string> seek(uint64_t position) {
        if (position >= this->size) {
            return Err(std::string("position greater than size"));
        }
        this->cntIdx = position;
        return Ok();
    }
};

} // namespace milody::util

#endif //MILODY_MILODY_UTIL_BINARYREADER_H
