// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.

package com.yahoo.jmh.jmhtest.jni;

import com.yahoo.jmh.jmhtest.Wrapper;

public class JNIWrapper implements Wrapper {

    static {
        JniLibraryLoader.load();
    }

    private static final JNIWrapper INSTANCE = new JNIWrapper();

    public static final JNIWrapper getInstance() {
        return INSTANCE;
    }

    static final native void nativeParamNone();

    static final native void nativeParamString(String param);

    static final native void nativeParamArray(String[] param);

    static final native String nativeParamNoneReturnString();

    private boolean returnValue = true;


    @Override
    public final boolean paramNone() {
        nativeParamNone();
        return returnValue;
    }

    @Override
    public final boolean paramString(String param) {
        nativeParamString(param);
        return returnValue;
    }

    @Override
    public final boolean paramArray(String[] param) {
        nativeParamArray(param);
        return returnValue;
    }

    @Override
    public final String paramNoneReturnString() {
        return nativeParamNoneReturnString();
    }

}
