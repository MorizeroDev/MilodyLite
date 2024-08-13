#include <Milody/context/context.h>
#include <Milody/game/milody_game_interface.h>
#include <Milody/game/milody_game_retcode.h>

using namespace milody::context;

int64_t MilodyContextNativeCancellationTokenContextCreate(Context*& ctx) {
    ctx = dynamic_cast<Context*>(new NativeCancellationTokenContext());
    return MILODY_API_RET_OK;
}

int64_t MilodyContextNativeCancellationTokenContextDestroy(Context*& ctx) {
    delete ctx;
    ctx = nullptr;
    return MILODY_API_RET_OK;
}

int64_t MilodyContextNativeCancellationTokenContextCancel(Context* ctx) {
    dynamic_cast<NativeCancellationTokenContext*>(ctx)->cancel();
    return MILODY_API_RET_OK;
}

int64_t MilodyContextManagedCancellationTokenContextCreate(Context*& ctx,
                                                   MilodyManagedCancellationTokenContextCancelledCallback cancelled, void* cancelledCtx) {
    ctx = dynamic_cast<Context*>(new ManagedCancellationTokenContext(cancelled, cancelledCtx));
    return MILODY_API_RET_OK;
}

int64_t MilodyContextManagedCancellationTokenContextDestroy(Context*& ctx) {
    delete ctx;
    ctx = nullptr;
    return MILODY_API_RET_OK;
}
