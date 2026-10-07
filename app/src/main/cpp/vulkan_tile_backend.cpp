#include <jni.h>
#include <vulkan/vulkan.h>
#include <android/log.h>
#include <vector>

#define LOG_TAG "VulkanTileBackend"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern "C" JNIEXPORT jboolean JNICALL
Java_com_hybridstudio_app_raster_tiles_VulkanTileBackend_initVulkan(JNIEnv* env, jobject thiz) {
    LOGI("Vulkan Inicializado.");
    return JNI_TRUE;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_hybridstudio_app_raster_tiles_VulkanTileBackend_processTileNative(
    JNIEnv* env, jobject thiz, jintArray pixelData, jint width, jint height, jint opType) {
    
    jint* pixels = env->GetIntArrayElements(pixelData, NULL);
    jsize length = env->GetArrayLength(pixelData);

    for (int i = 0; i < length; ++i) {
        uint32_t color = pixels[i];
        if (opType == 1) {
            uint32_t a = (color >> 24) & 0xFF;
            uint32_t r = 255 - ((color >> 16) & 0xFF);
            uint32_t g = 255 - ((color >> 8) & 0xFF);
            uint32_t b = 255 - (color & 0xFF);
            pixels[i] = (a << 24) | (r << 16) | (g << 8) | b;
        }
    }

    env->ReleaseIntArrayElements(pixelData, pixels, 0);
    return JNI_TRUE;
}

extern "C" JNIEXPORT void JNICALL
Java_com_hybridstudio_app_raster_tiles_VulkanTileBackend_cleanupVulkan(JNIEnv* env, jobject thiz) {
    LOGI("Vulkan Liberado.");
}
