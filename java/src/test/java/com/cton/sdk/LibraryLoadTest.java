package com.cton.sdk;

public class LibraryLoadTest {
    static {
        try {
            // Try to load the native library
            System.loadLibrary("cton-sdk");
            System.out.println("Successfully loaded cton-sdk library");
        } catch (UnsatisfiedLinkError e) {
            System.err.println("Failed to load cton-sdk library: " + e.getMessage());
            e.printStackTrace();
        }
    }
    
    public static void main(String[] args) {
        System.out.println("Library load test completed");
    }
}