// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.

package com.yahoo.jmh.jmhtest;

public interface Wrapper {

    boolean paramNone();

    boolean paramString(String param);

    boolean paramArray(String[] param);

    String paramNoneReturnString();

}
