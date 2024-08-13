#ifndef MILODY_JUCE_TIME_SLICE_THREAD_H
#define MILODY_JUCE_TIME_SLICE_THREAD_H
#include <juce_core/juce_core.h>
#include <Milody/util/milody_lazy.h>

namespace milody::audio {

juce::TimeSliceThread* GetTimeSliceThread();

}

#endif //MILODY_JUCE_TIME_SLICE_THREAD_H
