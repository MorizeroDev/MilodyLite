#ifndef MILODY_CONTEXT_H
#define MILODY_CONTEXT_H

#include "Milody/game/milody_game_types.h"
#include "atomic"

namespace milody::context {
class Context {
public:
    virtual bool isCanceled() {
        return false;
    };
};

class ManagedCancellationTokenContext: public Context {
private:
    MilodyManagedCancellationTokenContextCancelledCallback cancelledCallback;
    void* cancelledCtx;

public:
    ManagedCancellationTokenContext(MilodyManagedCancellationTokenContextCancelledCallback cancelled, void* cancelledCtx) {
        this->cancelledCallback = cancelled;
        this->cancelledCtx = cancelledCtx;
    }

    bool isCanceled() override {
        if (cancelledCallback != nullptr) {
            return cancelledCallback(cancelledCtx) != 0;
        }
        return false;
    }
};

class NativeCancellationTokenContext: public Context {
private:
    std::atomic<bool> canceled = false;

public:
    void cancel() {
        canceled = true;
    }

    bool isCanceled() override {
        return canceled;
    }
};

}

#endif