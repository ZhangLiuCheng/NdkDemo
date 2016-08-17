package com.innotechx.jni;

import android.content.Context;

public class NativeUtils {

    static {
        System.loadLibrary("nativeUtils");
    }

    /**
     * 获取签名信息.
     * @param context
     * @return
     */
    public static native String getSignature(Context context);

    /**
     * 检查签名是否被更改过。
     * @param context
     * @return
     */
    public static native boolean checkSignature(Context context);

    /**
     * 获取私有的key(用与加解密或者和服务器约定好的key).
     * @param context
     * @return
     */
    public static native String getPrivateKey(Context context);

}
