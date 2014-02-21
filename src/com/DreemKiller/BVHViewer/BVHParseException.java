package com.DreemKiller.BVHViewer;

public class BVHParseException extends Exception {
    /**
     * 
     */
    private static final long serialVersionUID = -8163593905121452393L;
    private String token;
    public BVHParseException(String token) {
        this.token = token;
    }
    
    public String toString() {
        return "BVH Parse error at token:" + token;
    }
}
