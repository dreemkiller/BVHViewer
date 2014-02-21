package com.DreemKiller.BVHViewer;

import java.nio.FloatBuffer;
import java.util.Vector;

public class BVHNode {
    BVHNodeType type;
    String name;
    double[] offset;
    BVHChannel[] channels;
    Vector<BVHNode> children;
    FloatBuffer lineBuffer;
    public BVHNode() {
        children = new Vector<BVHNode>();
        lineBuffer = null;
    }
}
