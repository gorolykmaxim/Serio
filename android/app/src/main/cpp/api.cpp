#include <jni.h>
#include <stdio.h>
#include <string>

extern "C" {
static void receiveEvent(JNIEnv *env, jobject obj, const std::string& event) {
    jstring outgoing = env->NewStringUTF(event.c_str());
    jclass cls = env->GetObjectClass(obj);
    jmethodID mid = env->GetMethodID(cls, "receiveEvent", "(Ljava/lang/String;)V");
    env->CallVoidMethod(obj, mid, outgoing);
}

JNIEXPORT void JNICALL Java_org_serio_Core_sendEvent(JNIEnv *env, jobject obj, jstring event) {
    const char* raw_event = env->GetStringUTFChars(event, nullptr);
    std::string data = std::string("Updated by C++: ") + std::string(raw_event);
    receiveEvent(env, obj, data);
    env->ReleaseStringUTFChars(event, raw_event);
}
}