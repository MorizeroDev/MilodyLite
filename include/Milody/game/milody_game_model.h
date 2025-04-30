#ifndef MILODY_GAME_MODEL_H
#define MILODY_GAME_MODEL_H

#include <string>
#include <utility>
#include <vector>

namespace milody::game::model {
class BytesWrapper {
private:
    std::vector<uint8_t> store;

public:
    explicit BytesWrapper(std::vector<uint8_t> value)
        : store(std::move(value)) {

          };

    explicit BytesWrapper(std::string value)
        : store(value.data(), value.data() + value.size()) {

          };

    [[nodiscard]] size_t GetSize() {
        return store.size();
    }

    [[nodiscard]] uint8_t* GetPtr() {
        return store.data();
    }
};

} // namespace milody::game::model

#endif
