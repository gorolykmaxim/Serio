#include <jni.h>
#include <queue.h>
#include <string>
#include <core.h>
#include <ui.h>
#include <task.h>
#include <nlohmann/json.hpp>

queue<task> task_queue;

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

static render_view create_platform_render_callback(JNIEnv* env, jobject obj) {
    return [env, obj] (const std::string& view_data) {receive_render_task(env, obj, view_data);};
}

JNIEXPORT void JNICALL Java_org_serio_Core_runNative(JNIEnv* env, jobject obj, jstring db_path) {
    try {
        const auto path = to_string(env, db_path);
        core_main(path, task_queue, create_platform_render_callback(env, obj));
    } catch (const std::exception& e) {
        display_fatal_error(e.what(), create_platform_render_callback(env, obj));
    }
}

JNIEXPORT void JNICALL Java_org_serio_Core_sendTask(JNIEnv *env, jobject obj, jstring task) {
    try {
        const auto task_string = to_string(env, task);
        const auto task_json = nlohmann::json::parse(task_string);
        const auto task = parse_task(task_json);
        task_queue.enqueue(task);
    } catch (const std::exception& e) {
        display_fatal_error(e.what(), create_platform_render_callback(env, obj));
    }
}

JNIEXPORT jboolean JNICALL Java_org_serio_Core_sendBackTaskFromRenderTask(JNIEnv* env, jobject obj, jstring render_task) {
    try {
        const auto render_task_string = to_string(env, render_task);
        const auto render_task_json = nlohmann::json::parse(render_task_string);
        const auto back_task = render_task_json.find("backTask");
        if (back_task != render_task_json.end()) {
            const auto task = parse_task(*back_task);
            task_queue.enqueue(task);
            return JNI_TRUE;
        } else {
            return JNI_FALSE;
        }
    } catch (const std::exception& e) {
        display_fatal_error(e.what(), create_platform_render_callback(env, obj));
        return JNI_TRUE;
    }
}
}