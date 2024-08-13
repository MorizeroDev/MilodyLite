#include <Milody/audio/milody_juce_time_slice_thread.h>

milody::util::Lazy<juce::TimeSliceThread*> TimeSliceThreadFactory([]() {
    auto thread = new juce::TimeSliceThread("audio file read");
    thread->startThread(juce::Thread::Priority::highest);
    return thread;
});

juce::TimeSliceThread* milody::audio::GetTimeSliceThread() {
    return TimeSliceThreadFactory.Get();
}
