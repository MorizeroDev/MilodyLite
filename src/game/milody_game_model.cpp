#include <Milody/game/milody_game_interface.h>
#include <Milody/game/milody_game_model.h>
#include <Milody/game/milody_game_retcode.h>

extern "C" {

MILODY_API int64_t MilodyGameModelBytesWrapperDestroy(milody::game::model::BytesWrapper*& ret) {
    if (ret == nullptr) {
        return MILODY_API_RET_OK;
    }
    delete ret;
    ret = nullptr;
    return MILODY_API_RET_OK;
}

MILODY_API int64_t MilodyGameModelBytesWrapperCStr(milody::game::model::BytesWrapper* ret,
                                                   uint8_t*& ptr,
                                                   uint64_t& size) {
    ptr = reinterpret_cast<uint8_t*>(ret->GetPtr());
    size = ret->GetSize();
    return MILODY_API_RET_OK;
}
}
