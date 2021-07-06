#include <jni.h>
#include <queue.h>
#include <string>
#include <core.h>
#include <task.h>
#include <nlohmann/json.hpp>

core core;

extern "C" {
static std::string to_string(JNIEnv* env, jstring value) {
    const auto raw = env->GetStringUTFChars(value, nullptr);
    std::string result(raw);
    env->ReleaseStringUTFChars(value, raw);
    return result;
}

static void receive_render_task(JNIEnv *env, jobject obj, const std::string& render_task) {
    jstring outgoing = env->NewStringUTF(render_task.c_str());
    jclass cls = env->GetObjectClass(obj);
    jmethodID mid = env->GetMethodID(cls, "receiveRenderTask", "(Ljava/lang/String;)V");
    env->CallVoidMethod(obj, mid, outgoing);
}

JNIEXPORT void JNICALL Java_org_serio_Core_runNative(JNIEnv* env, jobject obj, jstring db_path) {
    const auto path = to_string(env, db_path);
    init_core(core, path);
    while (true) {
        execute_core_task(core);
        const auto render_task = core.render_task_queue.try_dequeue();
        if (render_task) {
            receive_render_task(env, obj, *render_task);
        }
    }
}

JNIEXPORT void JNICALL Java_org_serio_Core_sendTask(JNIEnv *env, jobject obj, jstring jtask) {
    const auto task_string = to_string(env, jtask);
    enqueue_core_task(task_string, core.task_queue);
}

JNIEXPORT jboolean JNICALL Java_org_serio_Core_sendBackTaskFromRenderTask(JNIEnv* env, jobject obj, jstring render_task) {
    const auto render_task_string = to_string(env, render_task);
    return enqueue_back_task_from_render_task(render_task_string, core.task_queue) ? JNI_TRUE : JNI_FALSE;
}
}