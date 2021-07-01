#include <jni.h>
#include <queue.h>
#include <string>
#include <core.h>

queue<std::string> task_queue;

extern "C" {
static std::string to_string(JNIEnv* env, jstring value) {
    const auto raw = env->GetStringUTFChars(value, nullptr);
    std::string result(raw);
    env->ReleaseStringUTFChars(value, raw);
    return result;
}

static void receive_event(JNIEnv *env, jobject obj, const std::string& event) {
    jstring outgoing = env->NewStringUTF(event.c_str());
    jclass cls = env->GetObjectClass(obj);
    jmethodID mid = env->GetMethodID(cls, "receiveEvent", "(Ljava/lang/String;)V");
    env->CallVoidMethod(obj, mid, outgoing);
}
JNIEXPORT void JNICALL Java_org_serio_Core_runNative(JNIEnv* env, jobject obj, jstring db_path) {
    const auto path = to_string(env, db_path);
    core_main(path, task_queue, [env, obj] (const std::string& view_data) {receive_event(env, obj, view_data);});
}

JNIEXPORT void JNICALL Java_org_serio_Core_sendEvent(JNIEnv *env, jobject obj, jstring event) {
    task_queue.enqueue(to_string(env, event));
}
}