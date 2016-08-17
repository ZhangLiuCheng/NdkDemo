#include <jni.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>

#define LOG_TAG "AesUtils"
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args)

extern "C" {

    static bool checkSignature = false;

    // 获取签名信息
    JNIEXPORT jstring JNICALL Java_com_innotechx_jni_NativeUtils_getSignature
            (JNIEnv *env, jclass clazz, jobject context_object) {
        jclass context_class = env->GetObjectClass(context_object);

        //context.getPackageManager()
        jmethodID methodId = env->GetMethodID(context_class, "getPackageManager", "()Landroid/content/pm/PackageManager;");
        jobject package_manager_object = env->CallObjectMethod(context_object, methodId);
        if (package_manager_object == NULL) {
            __android_log_print(ANDROID_LOG_INFO, "JNITag","getPackageManager() Failed!");
            return NULL;
        }

        //context.getPackageName()
        methodId = env->GetMethodID(context_class, "getPackageName", "()Ljava/lang/String;");
        jstring package_name_string = (jstring)env->CallObjectMethod(context_object, methodId);
        if (package_name_string == NULL) {
            __android_log_print(ANDROID_LOG_INFO, "JNITag","getPackageName() Failed!");
            return NULL;
        }

        env->DeleteLocalRef(context_class);

        //PackageManager.getPackageInfo(Sting, int)
        jclass pack_manager_class = env->GetObjectClass(package_manager_object);
        methodId = env->GetMethodID(pack_manager_class, "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
        env->DeleteLocalRef(pack_manager_class);
        jobject package_info_object = env->CallObjectMethod(package_manager_object, methodId, package_name_string, 64);
        if (package_info_object == NULL) {
            __android_log_print(ANDROID_LOG_INFO, "JNITag","getPackageInfo() Failed!");
            return NULL;
        }

        env->DeleteLocalRef(package_manager_object);

        //PackageInfo.signatures[0]
        jclass package_info_class = env->GetObjectClass(package_info_object);
        jfieldID fieldId = env->GetFieldID(package_info_class, "signatures", "[Landroid/content/pm/Signature;");
        env->DeleteLocalRef(package_info_class);
        jobjectArray signature_object_array = (jobjectArray)env->GetObjectField(package_info_object, fieldId);
        if (signature_object_array == NULL) {
            __android_log_print(ANDROID_LOG_INFO, "JNITag","PackageInfo.signatures[] is null");
            return NULL;
        }
        jobject signature_object = env->GetObjectArrayElement(signature_object_array, 0);

        env->DeleteLocalRef(package_info_object);

        //Signature.toCharsString()
        jclass signature_class = env->GetObjectClass(signature_object);
//        methodId = env->GetMethodID(signature_class, "toCharsString", "()Ljava/lang/String;");
//        env->DeleteLocalRef(signature_class);
//        jstring signature_string = (jstring) env->CallObjectMethod(signature_object, methodId);
//        return signature_string;

        //Signature.hashCode()
        jmethodID methodID_hc = env->GetMethodID(signature_class, "hashCode", "()I");
        int hash_code = env->CallIntMethod(signature_object, methodID_hc);
        char str[32];
        sprintf(str, "%u", hash_code);
        return env->NewStringUTF(str);
    }

    // jstring转char＊
    char* jstringTostring(JNIEnv* env, jstring jstr) {
        char* rtn = NULL;
        jclass clsstring = env->FindClass("java/lang/String");
        jstring strencode = env->NewStringUTF("utf-8");
        jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
        jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
        jsize alen = env->GetArrayLength(barr);
        jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
        if (alen > 0) {
            rtn = (char*)malloc(alen + 1);
            memcpy(rtn, ba, alen);
            rtn[alen] = 0;
        }
        env->ReleaseByteArrayElements(barr, ba, 0);
        return rtn;
    }

    // 检查签名是否被更改过
    JNIEXPORT jboolean JNICALL Java_com_innotechx_jni_NativeUtils_checkSignature
            (JNIEnv *env, jclass clazz, jobject context_object) {
        jstring signature = Java_com_innotechx_jni_NativeUtils_getSignature(env, clazz, context_object);
        const char *orginalSignature = "244699197";
        const char *currentSignature = jstringTostring(env, signature);
        int result = strcmp(orginalSignature, currentSignature);
        return result == 0 ? true : false;
    }

    // 获取私有的key(用与加解密或者和服务器约定好的key)
    JNIEXPORT jstring JNICALL Java_com_innotechx_jni_NativeUtils_getPrivateKey
            (JNIEnv *env, jclass clazz, jobject context_object) {
        LOGI("-----> : checkSignature  %d", checkSignature);

        if (false == checkSignature) {
            checkSignature = Java_com_innotechx_jni_NativeUtils_checkSignature(env, clazz, context_object);
        }
        if (checkSignature) {
            return env->NewStringUTF("获取privateKey成功");
        }
        return env->NewStringUTF("获取失败");
    }
}