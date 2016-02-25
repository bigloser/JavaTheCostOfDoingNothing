// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.

package com.yahoo.jmh.jmhtest;

import com.yahoo.jmh.jmhtest.JavaWrapper;

import org.openjdk.jmh.annotations.Benchmark;
import org.openjdk.jmh.annotations.CompilerControl;
import org.openjdk.jmh.annotations.Scope;
import org.openjdk.jmh.annotations.Setup;
import org.openjdk.jmh.annotations.State;
import org.openjdk.jmh.annotations.TearDown;

@State(Scope.Benchmark)
public class JavaTest {

    String bar = "foo";
    String[] foo = {bar};
    JavaWrapper wrapper = JavaWrapper.getInstance();

    @Setup
    public void setup() {}

    @TearDown
    public void tearDown() {}

    @Benchmark
    @CompilerControl(CompilerControl.Mode.EXCLUDE)
    public boolean testParamNone() {
        return wrapper.paramNone();
    }

    @Benchmark
    @CompilerControl(CompilerControl.Mode.EXCLUDE)
    public boolean testParamString() {
        return wrapper.paramString(bar);
    }

    @Benchmark
    @CompilerControl(CompilerControl.Mode.EXCLUDE)
    public boolean testParamArray() {
        return wrapper.paramArray(foo);
    }

    @Benchmark
    @CompilerControl(CompilerControl.Mode.EXCLUDE)
    public String testParamNoneReturnString() {
        return wrapper.paramNoneReturnString();
    }

}
