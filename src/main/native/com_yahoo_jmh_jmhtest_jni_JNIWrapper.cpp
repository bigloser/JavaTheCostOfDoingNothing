#include "com_yahoo_jmh_jmhtest_jni_JNIWrapper.h"
#include "jni_helper_defines.h"

/*
 * Class:     com_yahoo_jmh_jmhtest_jni_JNIWrapper
 * Method:    nativeParamNone
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_yahoo_jmh_jmhtest_jni_JNIWrapper_nativeParamNone
(JNIEnv *jenv, jclass)
{
    // we're doing nothing.  yay.
}

/*
 * Class:     com_yahoo_jmh_jmhtest_jni_JNIWrapper
 * Method:    nativeParamString
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_yahoo_jmh_jmhtest_jni_JNIWrapper_nativeParamString
(JNIEnv *jenv, jclass, jstring stringArg)
{
    // we want to use this string.
    ScopedStringUTFChars scopedString(jenv, stringArg);
}

/*
 * Class:     com_yahoo_jmh_jmhtest_jni_JNIWrapper
 * Method:    nativeParamArray
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_yahoo_jmh_jmhtest_jni_JNIWrapper_nativeParamArray
(JNIEnv *jenv, jclass, jobjectArray stringArrayArg)
{

}

/*
 * Class:     com_yahoo_jmh_jmhtest_jni_JNIWrapper
 * Method:    nativeParamNoneReturnString
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_yahoo_jmh_jmhtest_jni_JNIWrapper_nativeParamNoneReturnString(
        JNIEnv *jenv, jclass) {
    return jenv->NewStringUTF("I'm doing nothing");

}
