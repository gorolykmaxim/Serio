#include <jni.h>
#include <queue.h>
#include <string>

queue<std::string> data_queue;

extern "C" {
static void receiveEvent(JNIEnv *env, jobject obj, const std::string& event) {
    jstring outgoing = env->NewStringUTF(event.c_str());
    jclass cls = env->GetObjectClass(obj);
    jmethodID mid = env->GetMethodID(cls, "receiveEvent", "(Ljava/lang/String;)V");
    env->CallVoidMethod(obj, mid, outgoing);
}
JNIEXPORT void JNICALL Java_org_serio_Core_runNative(JNIEnv* env, jobject obj) {
    while (true) {
        const auto data = data_queue.dequeue();
        receiveEvent(env, obj, "Updated by C++ in LOOP: " + data);
    }
}

JNIEXPORT void JNICALL Java_org_serio_Core_sendEvent(JNIEnv *env, jobject obj, jstring event) {
    const char* raw_event = env->GetStringUTFChars(event, nullptr);
    data_queue.enqueue(raw_event);
    env->ReleaseStringUTFChars(event, raw_event);
}
}