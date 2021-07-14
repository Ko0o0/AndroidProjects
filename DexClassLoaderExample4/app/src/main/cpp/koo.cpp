#include <jni.h>
#include <string>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <unistd.h>

jboolean loadAssetManager(JNIEnv *env, jobject thiz, jobject context);
void loadDex(JNIEnv *env, jobject thiz);
jobject getClassLoader(JNIEnv *env);
jobject getContext(JNIEnv *env);
jstring getAbsolutePath(JNIEnv *env, jobject thiz, jobject file);
jobject getCacheDir(JNIEnv *env);
FILE * createDexFile(JNIEnv *env, jobject thiz, char *filename);
jstring getMessage(JNIEnv *env, jobject thiz);

AAssetManager *aAssetManager;

jboolean loadAssetManager(JNIEnv *env, jobject thiz, jobject context) {
    jclass contextClass = env->FindClass("android/content/Context");
    jmethodID getAssets = env->GetMethodID(contextClass, "getAssets", "()Landroid/content/res/AssetManager;");
    jobject assetManager = env->CallObjectMethod(context, getAssets);
    aAssetManager = AAssetManager_fromJava(env, assetManager);

    if(aAssetManager != NULL) {
        return true;
    } else {
        return false;
    }
}

void loadDex(JNIEnv *env, jobject thiz) {
    if(aAssetManager == NULL) {
        return;
    }

    AAssetDir *aAssetDir = AAssetManager_openDir(aAssetManager, "");
    char *filename = NULL;
    int size = 0;
    /*
     * 원래 while문으로 반복해야 하는데 이상한 파일 하나가 딸려 나옴.. 일단 주석 처리
    while((filename = (char *) AAssetDir_getNextFileName(aAssetDir)) != NULL) {
        AAsset *aAsset = AAssetManager_open(aAssetManager, filename, AASSET_MODE_UNKNOWN);
        if(aAsset == NULL) {
            return;
        }

        FILE *dexFile = createDexFile(env, thiz, filename);
        int count = AAsset_getLength(aAsset);
        char *buf = (char *) malloc(sizeof(char) *count);
        if((size = AAsset_read(aAsset, buf, count)) > 0) {
            fwrite(buf, size, 1, dexFile);
        }
        fflush(dexFile);
        AAsset_close(aAsset);
        free(buf);
    }
    */

    filename = (char *) AAssetDir_getNextFileName(aAssetDir);
    AAsset *aAsset = AAssetManager_open(aAssetManager, filename, AASSET_MODE_UNKNOWN);
    if(aAsset == NULL) {
        return;
    }

    FILE *dexFile = createDexFile(env, thiz, filename);
    int count = AAsset_getLength(aAsset);
    char *buf = (char *) malloc(sizeof(char) *count);
    if((size = AAsset_read(aAsset, buf, count)) > 0) {
        fwrite(buf, size, 1, dexFile);
    }
    fflush(dexFile);
    AAsset_close(aAsset);
    free(buf);

    const char *cacheDir = env->GetStringUTFChars(getAbsolutePath(env, thiz, getCacheDir((env))), 0);
    char *dexPath = (char *) malloc(strlen(cacheDir));
    sprintf(dexPath, "%s/%s", cacheDir, filename);

    jclass DexFileClass = env->FindClass("dalvik/system/DexFile");
    jmethodID loadDexMethod = env->GetStaticMethodID(DexFileClass, "loadDex", "(Ljava/lang/String;Ljava/lang/String;I)Ldalvik/system/DexFile;");
    jobject loadDexFile = env->CallStaticObjectMethod(DexFileClass, loadDexMethod, env->NewStringUTF((const char *) dexPath), getAbsolutePath(env, thiz, getCacheDir(env)), 0);

    jmethodID loadClass = env->GetMethodID(DexFileClass, "loadClass", "(Ljava/lang/String;Ljava/lang/ClassLoader;)Ljava/lang/Class;");
    jobject classLoader = getClassLoader(env);
    env->CallObjectMethod(loadDexFile, loadClass, env->NewStringUTF("com/example/dexclassloaderexample4/Flag"), classLoader);

    // __android_log_print(ANDROID_LOG_DEBUG, "dexPath", "unlink %s", dexPath);
    unlink(dexPath);
}

jobject getClassLoader(JNIEnv *env) {
    jobject context = getContext(env);
    jclass contextClass = env->FindClass("android/content/Context");
    jmethodID getClassLoader = env->GetMethodID(contextClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject classLoader = env->CallObjectMethod(context, getClassLoader);
    return classLoader;
}

jobject getContext(JNIEnv *env) {
    jclass activityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);

    jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
    jobject context = env->CallObjectMethod(at, getApplication);
    return context;
}

jstring getAbsolutePath(JNIEnv *env, jobject thiz, jobject file) {
    jclass fileClass =env->FindClass("java/io/File");
    jmethodID getAbsolutePath = env->GetMethodID(fileClass, "getAbsolutePath", "()Ljava/lang/String;");
    jstring path = (jstring) env->CallObjectMethod(file, getAbsolutePath);
    return path;
}

jobject getCacheDir(JNIEnv *env) {
    jobject context = getContext(env);

    jclass contextClass = env->FindClass("android/content/Context");
    jmethodID getCacheDir = env->GetMethodID(contextClass, "getCacheDir", "()Ljava/io/File;");
    jobject cacheDir = env->CallObjectMethod(context, getCacheDir);

    return cacheDir;
}

FILE * createDexFile(JNIEnv *env, jobject thiz, char *filename) {
    jobject cacheDir = getCacheDir(env);
    const char *dexFormat = ".dex";
    strncat(filename, dexFormat, strlen(dexFormat));
    jclass fileClass =env->FindClass("java/io/File");
    jmethodID fileInit = env->GetMethodID(fileClass, "<init>", "(Ljava/io/File;Ljava/lang/String;)V");
    jobject dexPath = env->NewObject(fileClass, fileInit, cacheDir, env->NewStringUTF((char *)filename));

    const char *dexAbsolutePath = env->GetStringUTFChars(getAbsolutePath(env, thiz, dexPath), 0);
    // __android_log_print(ANDROID_LOG_DEBUG, "dexPath", "path is %s", dexAbsolutePath);
    FILE *dexFile = fopen(dexAbsolutePath, "wb");
    return dexFile;
}

jstring getMessage(JNIEnv *env, jobject thiz) {
    jclass mainActivityClass = env->FindClass("com/example/dexclassloaderexample4/MainActivity");
    jmethodID getMsg = env->GetMethodID(mainActivityClass, "getMsg", "()Ljava/lang/String;");
    jstring message = (jstring) env->CallObjectMethod(thiz, getMsg);
    if(message != NULL) {
        return message;
    }
    return env->NewStringUTF("Error occurred !");
}

jint JNI_OnLoad(JavaVM *vm, void* reserved) {
    JNIEnv *env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    // Find your class. JNI_OnLoad is called from the correct class loader context for this to work.
    jclass c = env->FindClass("com/example/dexclassloaderexample4/MainActivity");
    if (c == nullptr) return JNI_ERR;

    // Register your class' native methods.
    static const JNINativeMethod methods[] = {
            {"loadAssetManager", "(Landroid/content/Context;)Z", reinterpret_cast<void*>(loadAssetManager)},
            {"loadDex", "()V", reinterpret_cast<void*>(loadDex)},
            {"getMessage", "()Ljava/lang/String;", reinterpret_cast<void*>(getMessage)}
    };
    int rc = env->RegisterNatives(c, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}