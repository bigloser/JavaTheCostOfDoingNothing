#ifndef __YJAVA_JNI_HELPER_DEFINES_H__
#define __YJAVA_JNI_HELPER_DEFINES_H__

//#pragma GCC diagnostic ignored "-Wunused-function"

/* Note that method IDs are cached in static local variables.  This is susceptible to
 race conditions, but that's OK because the operations are idempotent.  Note also that
 global references are created for the classes that created the method IDs.  This keeps the
 classes from getting unloaded which would invalidate the cached method IDs. */

/**
 * Help for using the #defines
 * 		RETURN_NULL_IF_EXCEPTION();
 * 			checks for an exception. returns null to allow it to be thrown.
 *
 * 		RETURN_NULL_IF_EXCEPTION_OR_NULL(p);
 * 			checks for an exception or NULL==p. returns null to allow it to be thrown.
 *
 * 		POP_AND_RETURN_NULL_IF_EXCEPTION();
 * 			checks for an exception. returns null to allow it to be thrown, also does a PopLocalFrame
 *
 * 		POP_AND_RETURN_NULL_IF_EXCEPTION(p);
 * 			checks for an exception or NULL==p. returns null to allow it to be thrown, also does a PopLocalFrame
 *
 * Accessing a class:
 *  	// Get the class and hang onto the classId, you'll need this reference.
 *		GET_CACHED_CLASS(jenv, inet4AddressClass);
 *		RETURN_NULL_IF_EXCEPTION_OR_NULL(inet4Clazz);
 *
 * Accessing a Object Method:
 * 		GET_CACHED_METHOD_ID(jenv, inet4ConstructorID);
 *		RETURN_NULL_IF_EXCEPTION_OR_NULL(inet4ConstructorID);
 *
 *	The Method is now stored in inet4ConstructorID, which is a global static with a GlobalReference.
 *	As such you can reuse it, or recall the same function from 2 threads and result in the same information.
 *
 */

#include <stdlib.h>
#include <string.h>

#ifdef YJAVA_DEBUG_TYPEMAPS
#undef NDEBUG
#include <assert.h>
#endif //YJAVA_DEBUG_TYPEMAPS

#define POINTER_TO_CONTEXT(CTX_TYPE) CTX_TYPE* context= reinterpret_cast<CTX_TYPE*>(pointer);
#define CONTEXT_TO_POINTER(CTX) jlong pointer = reinterpret_cast<jlong>(CTX);

#define RETURN_IF_EXCEPTION() {\
        if (jenv->ExceptionCheck()) {\
            return;\
        }\
    }

#define RETURN_NULL_IF_EXCEPTION() {\
        if (jenv->ExceptionCheck()) {\
            return 0;\
        }\
    }

#define RETURN_NULL_IF_CLASS_IS_NULL(p) {\
    if (jenv->ExceptionCheck()) {\
        return NULL;\
    }\
    if (NULL == p) {\
       ThrowNullPointerException(jenv, "class was not found "#p);\
       return 0;\
    }\
}

#define RETURN_IF_CLASS_IS_NULL(p) {\
    if (jenv->ExceptionCheck()) {\
        return;\
    }\
    if (NULL == p) {\
        ThrowNullPointerException(jenv, "class was not found "#p);\
        return;\
    }\
}

#define RETURN_NULL_IF_EXCEPTION_OR_NULL(p) {\
    if (NULL == p || jenv->ExceptionCheck()) {\
        return 0;\
    }\
}

#define RETURN_NULL_IF_EXCEPTION_OR_NULL_AND_FREE(p, func, context) {\
    if (NULL == p || jenv->ExceptionCheck()) {\
        if (NULL != context) { \
            (*func)(context); \
        } \
        return 0; \
    } \
}

#define POP_AND_RETURN_NULL_IF_EXCEPTION() {\
    if (jenv->ExceptionCheck()) {\
        jenv->PopLocalFrame(NULL);\
        return 0;\
    }\
}

#define POP_AND_RETURN_NULL_IF_EXCEPTION_OR_NULL(p) {\
    if (NULL == p || jenv->ExceptionCheck()) {\
       jenv->PopLocalFrame(NULL);\
       return 0;\
    }\
}

#define RETURN_NULL_IF_NOT_0_OR_EXCEPTION(p) {\
    if (0 != p || jenv->ExceptionCheck()) {\
        return 0;\
    }\
}

#define RETURN_IF_EXCEPTION_OR_NULL(p) {\
    if (0 == p || jenv->ExceptionCheck()) {\
        return;\
    }\
}

#define RETURN_NULL_IF_0_OR_EXCEPTION(p) {\
    if (0 == p || jenv->ExceptionCheck()) {\
        return 0;\
    }\
}

#define RETURN_NULL_AND_THROW_IF_NULL(p,mesg) {\
    if (jenv->ExceptionCheck()) {\
        return 0;\
    }\
    if (0==p) {\
        ThrowNullPointerException(jenv, mesg);\
        return 0;\
    }\
}

#define RETURN_FALSE_AND_THROW_IF_NULL(p,mesg) {\
    if (jenv->ExceptionCheck()) {\
        return JNI_FALSE;\
    }\
    if (0==p) {\
        ThrowNullPointerException(jenv, mesg);\
        return JNI_FALSE;\
    }\
}

#define RETURN_AND_THROW_IF_NULL(p,mesg) {\
    if (jenv->ExceptionCheck()) {\
        return;\
    }\
    if (0==p) {\
        ThrowNullPointerException(jenv, mesg);\
        return;\
    }\
}

#define RETURN_AND_THROW_IF_NOT_ZERO(p,mesg) {\
    if (jenv->ExceptionCheck()) {\
        return;\
    }\
    if (0!=p) {\
        ThrowException(jenv,RUNTIME_EXCEPTION , mesg);\
        return;\
    }\
}

#define RETURN_FALSE_IF_EXCEPTION() {\
    if (jenv->ExceptionCheck()) {\
        return false;\
    }\
}

#define RETURN_FALSE_IF_EXCEPTION_OR_NULL(p) {\
    if (NULL == p || jenv->ExceptionCheck()) {\
        return false;\
    }\
}

#define RETURN_NULL_AND_THROW_IF_NOT_INSTANCEOF(object, clazz, mesg) {\
    if (JNI_FALSE == jenv->IsInstanceOf(object, clazz)) {\
        ThrowException(jenv, CLASS_CAST_EXCEPTION , mesg#clazz);\
        return 0;\
    }\
}

#define RETURN_AND_THROW_IF_NOT_INSTANCEOF(object, clazz, mesg) {\
    if (JNI_FALSE == jenv->IsInstanceOf(object, clazz)) {\
        ThrowException(jenv, CLASS_CAST_EXCEPTION , mesg#clazz);\
        return;\
    }\
}

#define SETUP_OUTPUT_BUFFER() ScopedMemory toBuffer(scopedIn.getLength());
#define SETUP_OUTPUT_BUFFER_FROM_LEN(len) ScopedMemory toBuffer(len);
#define SETUP_OUTPUT_BYTE_ARRAY(len) ScopedByteArray toBuffer(jenv, len);

#define DECLARE_CACHED_CLASS(class_var, class_name) \
static jclass class_var = 0; \
\
static jclass get_ ## class_var(JNIEnv *jenv) { \
    if (class_var) return class_var; \
    jclass clazz = jenv->FindClass(class_name); \
    if (0 == clazz) return 0; \
    clazz = (jclass) jenv->NewGlobalRef(clazz); \
    if (0 == clazz) return 0; \
    class_var = clazz; \
    return class_var; \
}

#define GET_CACHED_CLASS(jenv, class_var) get_##class_var(jenv)

#define DECLARE_CACHED_METHOD_ID(class_var, method_var, method_name, method_sig) \
static jmethodID method_var = 0; \
\
static jmethodID get_ ## method_var(JNIEnv *jenv) { \
    if (method_var) return method_var; \
    jclass clazz = GET_CACHED_CLASS(jenv, class_var); \
    if (0 == clazz) return 0; \
    method_var = jenv->GetMethodID(clazz, method_name, method_sig); \
    if (jenv->ExceptionCheck()) return 0; \
    return method_var; \
}

#define GET_CACHED_METHOD_ID(jenv, method_var) get_##method_var(jenv)

#define DECLARE_CACHED_STATIC_METHOD_ID(class_var, method_var, method_name, method_sig) \
static jmethodID method_var = 0; \
\
static jmethodID get_ ## method_var(JNIEnv *jenv) { \
    if (method_var) return method_var; \
    jclass clazz = GET_CACHED_CLASS(jenv, class_var); \
    if (0 == clazz) return 0; \
    method_var = jenv->GetStaticMethodID(clazz, method_name, method_sig); \
    if (jenv->ExceptionCheck()) return 0; \
    return method_var; \
}

#define GET_CACHED_STATIC_METHOD_ID(jenv, method_var) get_##method_var(jenv)

#define DECLARE_CACHED_FIELD_ID(class_var, fieldid_var, fieldid_name, fieldid_type) \
static jfieldID fieldid_var = 0; \
\
static jfieldID get_ ## fieldid_var(JNIEnv *jenv) { \
    if (fieldid_var) return fieldid_var; \
    jclass clazz = GET_CACHED_CLASS(jenv, class_var); \
    if (0 == clazz) return 0; \
    fieldid_var = jenv->GetFieldID(clazz, fieldid_name, fieldid_type); \
    if (jenv->ExceptionCheck()) return 0; \
    return fieldid_var; \
}

#define GET_CACHED_FIELD_ID(jenv, fieldid_var) get_##fieldid_var(jenv)


#define DECLARE_STATIC_CACHED_FIELD_ID(class_var, fieldid_var, fieldid_name, field_sig) \
static jfieldID fieldid_var = 0; \
\
static jfieldID get_ ## fieldid_var(JNIEnv *jenv) { \
    if (fieldid_var) return fieldid_var; \
    jclass clazz = GET_CACHED_CLASS(jenv, class_var); \
    if (0 == clazz) return 0; \
    fieldid_var = jenv->GetStaticFieldID(clazz, fieldid_name, field_sig); \
    if (jenv->ExceptionCheck()) return 0; \
    return fieldid_var; \
}


DECLARE_CACHED_CLASS(stringBufferClass, "java/lang/StringBuffer")
DECLARE_CACHED_METHOD_ID(stringBufferClass, stringBufferCapacityID, "capacity",
        "()I")
DECLARE_CACHED_METHOD_ID(stringBufferClass, stringBufferSetLengthID,
        "setLength", "(I)V")
DECLARE_CACHED_METHOD_ID(stringBufferClass, stringBufferAppendStringID,
        "append", "(Ljava/lang/String;)Ljava/lang/StringBuffer;")
DECLARE_CACHED_METHOD_ID(stringBufferClass, stringBufferToStringID, "toString",
        "()Ljava/lang/String;")

DECLARE_CACHED_CLASS(stringClass, "java/lang/String")

#define OUT_OF_MEMORY_ERROR "java/lang/OutOfMemoryError"
#define IO_EXCEPTION "java/io/IOException"
#define RUNTIME_EXCEPTION "java/lang/RuntimeException"
#define INDEX_OUT_OF_BOUNDS_EXCEPTION "java/lang/IndexOutOfBoundsException"
#define ARITHMETIC_EXCEPTION "java/lang/ArithmeticException"
#define ILLEGAL_ARGUMENT_EXCEPTION "java/lang/IllegalArgumentException"
#define NULL_POINTER_EXCEPTION "java/lang/NullPointerException"
#define UNKNOWN_HOST_EXCEPTION "java/net/UnknownHostException"
#define UNSUPPORTED_ENCODING_EXCEPTION "java/io/UnsupportedEncodingException"
#define CLASS_CAST_EXCEPTION "java/lang/ClassCastException"

static void ThrowException(JNIEnv *jenv, const char *exceptionClass,
        const char *message) {
    jclass clazz = 0;

    if (!jenv || !exceptionClass) {
        return;
    }

    jenv->ExceptionClear();

    clazz = jenv->FindClass(exceptionClass);

    if (0 == clazz) {
        fprintf(stderr, "Error, cannot find exception class: %s",
                exceptionClass);
        return;
    }

    jenv->ThrowNew(clazz, message);
}

static void ThrowNullPointerException(JNIEnv *jenv, const char *mesg) {
    ThrowException(jenv, NULL_POINTER_EXCEPTION, mesg);
}

class ScopedMemory {
#ifdef YJAVA_DEBUG_TYPEMAPS
    const static size_t beforeBytes = 32;
    const static size_t afterBytes = 8192;
#endif //YJAVA_DEBUG_TYPEMAPS

public:
    ScopedMemory() {
        set((size_t) 0);
    }

    ScopedMemory(size_t length) {
        set(length);
    }

    ScopedMemory(size_t length, size_t size) {
        set(length, size);
    }


    inline void set(size_t length, size_t size) {
        set(length * size);
    }

    inline void set(size_t length) {
        _length = length;
        if (0 == _length) {
            _memory = 0;
#ifdef YJAVA_DEBUG_TYPEMAPS
            _first = 0;
            _last = 0;
            _realPtr = _memory;
#endif //YJAVA_DEBUG_TYPEMAPS
            return;
        }

#ifdef YJAVA_DEBUG_TYPEMAPS
        _realLen = beforeBytes+ length + afterBytes;
        _realPtr = calloc(_realLen, 1);
        _first = (void*) ((char*) _realPtr + beforeBytes);
        _last = (void*) ((char*) _first + length);

        memset(_realPtr, 0xDD, _realLen);
        memset(_realPtr, 0xBB, beforeBytes);
        memset(_first, 0, length);
        memset((void*) ((char*) _last), 0xAA, afterBytes);

        _memory = _first;
#else //YJAVA_DEBUG_TYPEMAPS
        _memory = calloc(_length + 1, 1);
#endif //YJAVA_DEBUG_TYPEMAPS
    }

    inline void set(void *memory, size_t length) {
        _memory = memory;
        _length = length;
#ifdef YJAVA_DEBUG_TYPEMAPS
        _first = _memory;
        _last = 0;
        _realPtr = _memory;
        _skipAssert = true;
#endif //YJAVA_DEBUG_TYPEMAPS
    }

    inline void *get() {
        return _memory;
    }

    inline size_t getLength() {
        return _length;
    }

#ifdef YJAVA_DEBUG_TYPEMAPS
    inline void check() {
        assert(_memory == _first);
        if (!_skipAssert) {
            char expectedBefore[beforeBytes];
            char expectedAfter[afterBytes];

            memset(expectedBefore, 0xBB, beforeBytes);
            memset(expectedAfter, 0xAA, afterBytes);

            assert(memcmp(_realPtr, expectedBefore, beforeBytes) == 0);
            assert(memcmp(_last, expectedAfter, afterBytes) == 0);
        }
    }
#endif //YJAVA_DEBUG_TYPEMAPS

    virtual ~ScopedMemory() {
        if (_memory) {
#ifdef YJAVA_DEBUG_TYPEMAPS
            assert(_memory == _first);
            check();
            free(_realPtr);
            _realPtr = NULL;
#else //YJAVA_DEBUG_TYPEMAPS
            free(_memory);
            _memory = NULL;
#endif //YJAVA_DEBUG_TYPEMAPS
        }
    }

private:
    // Disabled
    ScopedMemory(const ScopedMemory &);
    void operator=(const ScopedMemory &);

    size_t _length;
#ifdef YJAVA_DEBUG_TYPEMAPS
    size_t _realLen;
    void *_realPtr;
    void *_first;
    void *_last;
    bool _skipAssert;
#endif //YJAVA_DEBUG_TYPEMAPS

    void *_memory;
};

class ScopedStringUTFChars {
public:
    ScopedStringUTFChars(JNIEnv *jenv, jstring jstr, char *str) :
            _jenv(jenv), _jstr(jstr), _str(str) {
    }

    ScopedStringUTFChars(JNIEnv *jenv, jstring jstr) {
        set(jenv, jstr);
    }

    ScopedStringUTFChars() :
            _jenv(0), _jstr(0), _str(0) {
    }

    virtual ~ScopedStringUTFChars() {
        if (_jenv && _jstr && _str) {
            _jenv->ReleaseStringUTFChars(_jstr, _str);
        }
    }

    inline void set(JNIEnv *jenv, jstring jstr, char *str) {
        _jenv = jenv;
        _jstr = jstr;
        _str = str;
    }

    inline void set(JNIEnv *jenv, jstring jstr) {
        _str = (NULL == jstr) ? NULL : jenv->GetStringUTFChars(jstr, 0);
        _jenv = jenv;
        _jstr = jstr;
    }

    inline const char * getChars() {
        return _str;
    }

    inline const char * get() {
        return _str;
    }

    inline const char * getJavaStr() {
        return _str;
    }

    inline const jsize getLength() {
        if (NULL == _jstr) {
            return 0;
        }
        return _jenv->GetStringUTFLength(_jstr);
    }

private:
    // Disabled
    ScopedStringUTFChars(const ScopedStringUTFChars &);
    void operator=(const ScopedStringUTFChars &);

    JNIEnv *_jenv;
    jstring _jstr;
    const char *_str;
};

class ScopedStringUTFCharsArray {
public:
    ScopedStringUTFCharsArray(char **nativeStrings,
            ScopedStringUTFChars *scopedStrings) :
            _nativeStrings(nativeStrings), _scopedStrings(scopedStrings) {
    }

    ScopedStringUTFCharsArray() :
            _nativeStrings(0), _scopedStrings(0) {
    }

    virtual ~ScopedStringUTFCharsArray() {
        if (_scopedStrings) {
            // has to be delete because it's an array of c++ objects.
            delete[] _scopedStrings;
        }

        if (_nativeStrings) {
            free(_nativeStrings);
        }
    }

    void set(char **nativeStrings, ScopedStringUTFChars *scopedStrings) {
        _nativeStrings = nativeStrings;
        _scopedStrings = scopedStrings;
    }

    char **get() {
        return _nativeStrings;
    }

private:
    // Disabled
    ScopedStringUTFCharsArray(const ScopedStringUTFCharsArray &);
    void operator=(const ScopedStringUTFCharsArray &);

    char **_nativeStrings;
    ScopedStringUTFChars *_scopedStrings;
};


class ScopedStringArray {
public:
    ScopedStringArray(JNIEnv *jenv, jobjectArray objects) : _scopedStrings(NULL), size(0), _nativeStrings(NULL) {
      if (NULL == objects) {
          return;
      }

      GET_CACHED_CLASS(jenv, stringClass);

      size = jenv->GetArrayLength(objects);
      _scopedStrings = new ScopedStringUTFChars*[size];
      _nativeStrings = new const char*[size+1];

      for (size_t i=0;i < size; i++) {
        jobject object = jenv->GetObjectArrayElement(objects, i);
        jstring str = NULL;
        if (NULL != object) {
            if (JNI_FALSE == jenv->IsInstanceOf(object, stringClass)) {
              continue;
            }
            str = (jstring)object;
        }

        _scopedStrings[i] = new ScopedStringUTFChars(jenv, str);
        _nativeStrings[i] = _scopedStrings[i]->getChars();
      }

      // gotta love the NULL;
      _nativeStrings[size] = NULL;
    }

    virtual ~ScopedStringArray() {
        if (NULL != _scopedStrings) {
            for (size_t i=0;i<size;i++) {
                if (NULL != _scopedStrings[i]) {
                    delete _scopedStrings[i];
                    _scopedStrings[i]=NULL;
                }
            }

            // has to be delete because it's an array of c++ objects.
            delete[] _scopedStrings;
            _scopedStrings = NULL;
        }
    }

    const char **get() {
      return _nativeStrings;
    }

private:
    // Disabled
    ScopedStringArray(const ScopedStringArray &);
    void operator=(const ScopedStringArray &);

    ScopedStringUTFChars **_scopedStrings;
    size_t size;
    const char **_nativeStrings;
};


class ScopedByteArray {
public:
    ScopedByteArray() :
            _jenv(NULL), javaArray(NULL), nativeArray(NULL) {
    }

    ScopedByteArray(JNIEnv* env, int len) :
            _jenv(env), javaArray(NULL), nativeArray(NULL) {

        this->javaArray = _jenv->NewByteArray(len);

        if (NULL == this->javaArray) {
            ThrowException(_jenv, NULL_POINTER_EXCEPTION,
                    "Failed to allocate byte array");
        }

        setNative();
    }

    ScopedByteArray(JNIEnv* env, jbyteArray javaArray) :
            _jenv(env), javaArray(javaArray), nativeArray(NULL) {

        setNative();
    }

    ScopedByteArray(JNIEnv* env, void *copyFromNative, int start, int len) :
            _jenv(env), javaArray(NULL), nativeArray(NULL) {
        this->javaArray = _jenv->NewByteArray(len);

        if (NULL == copyFromNative || len <= 0) {
            return;
        }

        if (NULL != this->javaArray) {
            (this->_jenv)->SetByteArrayRegion(this->javaArray, start, len,
                    (jbyte*) copyFromNative);
        } else {
            ThrowException(_jenv, NULL_POINTER_EXCEPTION,
                    "Failed to allocate byte array");
        }
    }

    ScopedByteArray(JNIEnv* env, void *copyFromNative, int start, int copyLen, int totalLen) :
            _jenv(env), javaArray(NULL), nativeArray(NULL) {
        this->javaArray = _jenv->NewByteArray(totalLen);

        if (NULL == copyFromNative || copyLen <= 0 || totalLen <= 0 || copyLen > totalLen) {
            return;
        }

        if (NULL != this->javaArray) {
            (this->_jenv)->SetByteArrayRegion(this->javaArray, start, copyLen,
                    (jbyte*) copyFromNative);
        } else {
            ThrowException(_jenv, NULL_POINTER_EXCEPTION,
                    "Failed to allocate byte array");
        }
    }

    ScopedByteArray(JNIEnv* env, jbyteArray javaArray, jbyte* nativeArray) :
            _jenv(env), javaArray(javaArray), nativeArray(nativeArray) {
    }

    ~ScopedByteArray() {
        if (NULL == this->_jenv) {
            return;
        }

        if (NULL == this->javaArray) {
            return;
        }

        if (NULL == this->nativeArray) {
            return;
        }

        (this->_jenv)->ReleaseByteArrayElements(this->javaArray,
                this->nativeArray, 0);
    }

//    void set(JNIEnv* env, jbyteArray javaArray, void* nativeArray) {
//        this->_jenv = env;
//        this->javaArray = javaArray;
//        this->nativeArray = nativeArray;
//    }

    void set(JNIEnv* env, jbyteArray javaArray) {
        this->_jenv = env;
        this->javaArray = javaArray;

        setNative();
    }

    inline jbyte* get() {
        return nativeArray;
    }

    inline jbyteArray getByteArray() {
        return javaArray;
    }

    inline const jsize getLength() {
        if (NULL == javaArray) {
            return 0;
        }

        return _jenv->GetArrayLength(javaArray);
    }

    inline void setNative() {
        if (NULL == this->javaArray) {
            return;
        }

        this->nativeArray = (this->_jenv)->GetByteArrayElements(this->javaArray,
                NULL);
    }

private:
    JNIEnv* _jenv;
    jbyteArray javaArray;
    jbyte* nativeArray;
};


class ScopedLongArray {
public:
    ScopedLongArray() :
            _jenv(NULL), javaArray(NULL), nativeArray(NULL) {
    }

    ScopedLongArray(JNIEnv* env, int len) :
            _jenv(env), javaArray(NULL), nativeArray(NULL) {

        this->javaArray = _jenv->NewLongArray(len);

        if (NULL == this->javaArray) {
            ThrowException(_jenv, NULL_POINTER_EXCEPTION,
                    "Failed to allocate long array");
        }

        setNative();
    }

    ScopedLongArray(JNIEnv* env, jlongArray javaArray) :
            _jenv(env), javaArray(javaArray), nativeArray(NULL) {

        setNative();
    }

    ScopedLongArray(JNIEnv* env, void *copyFromNative, int start, int len) :
            _jenv(env), javaArray(NULL), nativeArray(NULL) {
        this->javaArray = _jenv->NewLongArray(len);

        if (NULL == copyFromNative || len <= 0) {
            return;
        }

        if (NULL != this->javaArray) {
            (this->_jenv)->SetLongArrayRegion(this->javaArray, start, len,
                    (jlong*) copyFromNative);
        } else {
            ThrowException(_jenv, NULL_POINTER_EXCEPTION,
                    "Failed to allocate long array");
        }
    }

    ScopedLongArray(JNIEnv* env, void *copyFromNative, int start, int copyLen, int totalLen) :
            _jenv(env), javaArray(NULL), nativeArray(NULL) {
        this->javaArray = _jenv->NewLongArray(totalLen);

        if (NULL == copyFromNative || copyLen <= 0 || totalLen <= 0 || copyLen > totalLen) {
            return;
        }

        if (NULL != this->javaArray) {
            (this->_jenv)->SetLongArrayRegion(this->javaArray, start, copyLen,
                    (jlong*) copyFromNative);
        } else {
            ThrowException(_jenv, NULL_POINTER_EXCEPTION,
                    "Failed to allocate long array");
        }
    }

    ScopedLongArray(JNIEnv* env, jlongArray javaArray, jlong* nativeArray) :
            _jenv(env), javaArray(javaArray), nativeArray(nativeArray) {
    }

    ~ScopedLongArray() {
        if (NULL == this->_jenv) {
            return;
        }

        if (NULL == this->javaArray) {
            return;
        }

        if (NULL == this->nativeArray) {
            return;
        }

        (this->_jenv)->ReleaseLongArrayElements(this->javaArray,
                this->nativeArray, 0);
    }

//    void set(JNIEnv* env, jlongArray javaArray, void* nativeArray) {
//        this->_jenv = env;
//        this->javaArray = javaArray;
//        this->nativeArray = nativeArray;
//    }

    void set(JNIEnv* env, jlongArray javaArray) {
        this->_jenv = env;
        this->javaArray = javaArray;

        setNative();
    }

    inline jlong* get() {
        return nativeArray;
    }

    inline jlongArray getLongArray() {
        return javaArray;
    }

    inline const jsize getLength() {
        if (NULL == javaArray) {
            return 0;
        }

        return _jenv->GetArrayLength(javaArray);
    }

    inline void setNative() {
        if (NULL == this->javaArray) {
            return;
        }

        this->nativeArray = (this->_jenv)->GetLongArrayElements(this->javaArray,
                NULL);
    }

private:
    JNIEnv* _jenv;
    jlongArray javaArray;
    jlong* nativeArray;
};


#endif /* __YJAVA_JNI_HELPER_DEFINES_H__ */
