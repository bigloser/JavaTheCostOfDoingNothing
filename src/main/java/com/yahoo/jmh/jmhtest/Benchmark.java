// Copyright 2016 Yahoo Inc.
// Licensed under the terms of the New-BSD license. Please see LICENSE file in the project root for terms.

package com.yahoo.jmh.jmhtest;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.Collection;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;
import org.openjdk.jmh.results.RunResult;
import org.openjdk.jmh.results.format.ResultFormatType;

import org.openjdk.jmh.runner.Runner;
import org.openjdk.jmh.runner.RunnerException;
import org.openjdk.jmh.runner.options.Options;
import org.openjdk.jmh.runner.options.OptionsBuilder;
import org.openjdk.jmh.runner.options.VerboseMode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class Benchmark {
    
    private static final Logger log = LoggerFactory.getLogger(Benchmark.class);
    
    public static void main(String[] args) throws RunnerException {
        String outputFn = "jmhoutput.txt";
        
        OptionsBuilder builder = new OptionsBuilder();        
        
        builder.include(".*");
        builder.warmupIterations(10);
        builder.measurementIterations(10);
        builder.verbosity(VerboseMode.EXTRA);
        builder.jvmArgs("-server");
        builder.forks(1);
        builder.resultFormat(ResultFormatType.TEXT);
        builder.timeUnit(TimeUnit.NANOSECONDS);
        builder.output(outputFn);
        Options o = builder.build();
        
        Runner r = new Runner(o);
        
        Collection<RunResult> records = r.run();
        
        if (null == records) {
            System.err.println("Benchmark did not run. No records were returned.");
        }
        
        log.debug("Detailed results saved to: {}", outputFn);
        log.debug("");

        try (Scanner scanner = new Scanner(new File("jmh-result.text")).useDelimiter("\\Z")) {
            while (scanner.hasNext()) {
                String line = scanner.next();
                log.debug(line);
            }
        } catch (FileNotFoundException e) {
            log.error("Unable to display results for file: {}" , outputFn, e);            
        }
    }
    
}
