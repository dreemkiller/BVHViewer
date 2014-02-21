package com.DreemKiller.BVHViewer;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLU;
import android.view.MotionEvent;

public class BVHViewerRenderer implements Renderer {

    private BVHNode root;
    double[][] channelValues;
    int currentFrame;
    int currentChannel;
    float offsetScaling = 0.025f;
    float[] cameraPosition = {-.05f, 0.5f, 1.5f};
    float[] cameraLookAt = {0.0f, 0.0f, 0.0f};
    
    public BVHViewerRenderer(BVHNode root, double[][] channelValues) {
        this.root = root;
        currentFrame = -1;
        this.channelValues = channelValues;
    }
    public void onDrawFrame(GL10 gl) {
        // Redraw the background color
        gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);

        gl.glMatrixMode(GL10.GL_MODELVIEW);
        gl.glLoadIdentity();
        GLU.gluLookAt(gl, cameraPosition[0], cameraPosition[1], cameraPosition[2], 
                          cameraLookAt[0], cameraLookAt[1], cameraLookAt[2],
                           0.0f, 1.0f, 0.0f);

        drawGrid(gl);

        currentChannel = 0;
        gl.glLineWidth(5.0f);
        drawNode(gl, root);
        gl.glLineWidth(1.0f);
        gl.glFlush();
        currentFrame++;
        if (currentFrame >= channelValues.length) {
            currentFrame = 0;
        }
    }
    
    private void drawNode(GL10 gl, BVHNode node) {
        gl.glTranslatef(offsetScaling * (float) node.offset[0],
                        offsetScaling * (float) node.offset[1],
                        offsetScaling * (float) node.offset[2]);
        if ((-1 != currentFrame) && (BVHNodeType.BVHNTEnd != node.type )) {
            for (int a = 0; a < node.channels.length; a++) {
                BVHChannel thisChannel = node.channels[a];
                switch (thisChannel) {
                case Xposition:
                    gl.glTranslatef(offsetScaling * (float) channelValues[currentFrame][currentChannel], 0.0f, 0.0f);
                    break;
                case Yposition:
                    gl.glTranslatef(0.0f, offsetScaling * (float) channelValues[currentFrame][currentChannel], 0.0f);
                    break;
                case Zposition:
                    gl.glTranslatef(0.0f, 0.0f, offsetScaling * (float) channelValues[currentFrame][currentChannel]);
                    break;
                case Zrotation:
                    gl.glRotatef((float) channelValues[currentFrame][currentChannel], 0.0f, 0.0f, 1.0f);
                    break;
                case Yrotation:
                    gl.glRotatef((float) channelValues[currentFrame][currentChannel], 0.0f, 1.0f, 0.0f);
                    break;
                case Xrotation:
                    gl.glRotatef((float) channelValues[currentFrame][currentChannel], 1.0f, 0.0f, 0.0f);
                    break;
                }
                currentChannel++;
            }
        }
        drawDiamond(gl);
        for (int a = 0; a < node.children.size(); a++)
        {
            BVHNode thisChild = node.children.get(a);
            // draw a line from the parent node(which due to translations, is at the origin) to this node
            drawLine(gl, thisChild.offset, thisChild.lineBuffer);
            gl.glPushMatrix();
            drawNode(gl, thisChild);
            gl.glPopMatrix();
        }
    }

    private void drawDiamond(GL10 gl) {
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, diamondVB);
        gl.glColor4f(0.0f, 1.0f, 0.0f, 0.0f);
        gl.glDrawElements(GL10.GL_TRIANGLES, 24, GL10.GL_UNSIGNED_SHORT, diamondIB);
    }
    private int screenHeight;
    private int screenWidth;
    public void sendMotionEvent(MotionEvent e) {
        if(e.getAction() == MotionEvent.ACTION_DOWN)
        {
            MotionEvent.PointerCoords coords = new MotionEvent.PointerCoords();
            e.getPointerCoords(0, coords);
            // Determine which quadrant the click was in: top center, bottom center, center right, or center left.
            float test1 = ((float) screenWidth) * (coords.y - (float) screenHeight) + ((float) screenHeight) * coords.x;
            float test2 = ((float) screenWidth) * coords.y - ((float) screenHeight) * coords.x;
            boolean topLeftHalf = (test1 < 0.0f);
            boolean topRightHalf = (test2 < 0.0f);
            float zoomDelta = 0.1f;
            float panDelta = (float) (25.0 / 360.0 * 2.0 * Math.PI);
            
            // need a normalized vector from cameraPosition to cameraLookAt
            float[] vector = {cameraLookAt[0] - cameraPosition[0], cameraLookAt[1] - cameraPosition[1], cameraLookAt[2] - cameraPosition[2]};
            float vectorLength = (float) Math.sqrt(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
            vector[0] /= vectorLength;
            vector[1] /= vectorLength;
            vector[2] /= vectorLength;
            if (topLeftHalf && topRightHalf) {
                // top center. Zoom in
                // Need to modify camera position in the direction of cameraLookAt (scaled by zoomDelta)
                vector[0] *= zoomDelta;
                vector[1] *= zoomDelta;
                vector[2] *= zoomDelta;
                cameraPosition[0] += vector[0];
                cameraPosition[1] += vector[1];
                cameraPosition[2] += vector[2];
                
            } else if (!topLeftHalf && !topRightHalf) {
                // bottom center. Zoom out
                // Need to modify camera position in the reverse direction of cameraLookAt (scaled by zoomDelta)
                vector[0] *= zoomDelta;
                vector[1] *= zoomDelta;
                vector[2] *= zoomDelta;
                
                cameraPosition[0] -= vector[0];
                cameraPosition[1] -= vector[1];
                cameraPosition[2] -= vector[2];
            }else if (!topLeftHalf && topRightHalf) {
                // center right. Pan right
                float[] tempVector = new float[2];
                tempVector[0] = (float) (vector[0] * Math.cos(panDelta) - vector[2] * Math.sin(panDelta));
                tempVector[1] = (float) (vector[0] * Math.sin(panDelta) + vector[2] * Math.cos(panDelta));
                cameraLookAt[0] += tempVector[0];
                cameraLookAt[1] = cameraLookAt[1]; // no change in y
                cameraLookAt[2] += tempVector[1]; 
                
            }else if (topLeftHalf && !topRightHalf) {
                // center left. Pan left
                float[] tempVector = new float[2];
                tempVector[0] = (float) (vector[0] * Math.cos(-panDelta) - vector[2] * Math.sin(-panDelta));
                tempVector[1] = (float) (vector[0] * Math.sin(-panDelta) + vector[2] * Math.cos(-panDelta));
                cameraLookAt[0] += tempVector[0];
                cameraLookAt[1] = cameraLookAt[1]; // no change in y
                cameraLookAt[2] += tempVector[1];
            }
        }
    }
    
    private void drawLine(GL10 gl, double[] end, FloatBuffer lineBuffer) {
         if (lineBuffer == null) {
            float lineCoords[] = {
                                                0.0f,                               0.0f,                             0.0f,
                       offsetScaling * (float) end[0],    offsetScaling * (float) end[1],   offsetScaling * (float) end[2],
            };
            FloatBuffer lineVB;
            ByteBuffer vbb = ByteBuffer.allocateDirect(lineCoords.length * 4);
            vbb.order(ByteOrder.nativeOrder());// use the device Hws native byte order
            lineVB = vbb.asFloatBuffer();
            lineVB.put(lineCoords);
            lineVB.position(0);
            lineBuffer = lineVB;
        }
        gl.glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, lineBuffer);
        gl.glDrawArrays(GL10.GL_LINES, 0, 2);
        
    }
    
    private void drawGrid(GL10 gl) {
        gl.glVertexPointer(3, GL10.GL_FLOAT, 0, gridVB);
        gl.glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        gl.glDrawArrays(GL10.GL_LINES, 0, numPointsInGrid);
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
        screenWidth = width;
        screenHeight = height;
        gl.glViewport(0, 0, width, height);
        gl.glMatrixMode(GL10.GL_PROJECTION);
        gl.glLoadIdentity();
        GLU.gluPerspective(gl, 80.0f, (float) width/(float) height, 0.25f, 100.0f);
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        // Set the background frame color
        gl.glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
        initShapes();
        
        // enable use of vertex arrays
        gl.glEnableClientState(GL10.GL_VERTEX_ARRAY);
    }

    private FloatBuffer diamondVB;
    private ShortBuffer diamondIB;
    private FloatBuffer gridVB;
    private int numPointsInGrid;
    //private FloatBuffer axesVB;

    private void initShapes() {
        float diamondSize = 0.012f;
        float diamondCoords[] = {
                         0.0f,  diamondSize,          0.0f,
                         0.0f,            0.0f,  diamondSize,
                 diamondSize,            0.0f,          0.0f,
                         0.0f,            0.0f, -diamondSize,
                -diamondSize,            0.0f,          0.0f,
                         0.0f, -diamondSize,          0.0f
        };
        ByteBuffer vbb = ByteBuffer.allocateDirect(diamondCoords.length * 4);
        vbb.order(ByteOrder.nativeOrder());// use the device Hws native byte order
        diamondVB = vbb.asFloatBuffer();
        diamondVB.put(diamondCoords);
        diamondVB.position(0);
        
        short diamondIndices[] = {
                0, 2, 1,
                0, 3, 2,
                0, 4, 3,
                0, 1, 4,
                5, 2, 1,
                5, 3, 2,
                5, 4, 3,
                5, 1, 2
        };
        ByteBuffer dibb = ByteBuffer.allocateDirect(diamondIndices.length * 2);
        dibb.order(ByteOrder.nativeOrder());
        diamondIB = dibb.asShortBuffer();
        diamondIB.put(diamondIndices);
        diamondIB.position(0);
        
        int numLines = 20;
        int pointsPerLine = 2;
        int numDimensionsOfPlane = 2;
        int numCoordsPerVertex = 3;
        numPointsInGrid = pointsPerLine * numDimensionsOfPlane * (numLines + 1);
        float gridCoords[] = new float[numPointsInGrid * numCoordsPerVertex];
        float startX = -2.0f;
        float endX = 2.0f;
        float startZ = -2.0f;
        float endZ = 2.0f;
        float xStep = (endX - startX) / (float) numLines;
        float zStep = (endZ - startZ) / (float) numLines;
        for (int a = 0; a < numLines + 1; a++) {
            gridCoords[a * 6 + 0] = startX + a * xStep;
            gridCoords[a * 6 + 1] = 0.0f;
            gridCoords[a * 6 + 2] = startZ;
            
            gridCoords[a * 6 + 3] = startX + a * xStep;
            gridCoords[a * 6 + 4] = 0.0f;
            gridCoords[a * 6 + 5] = endZ;
            
            gridCoords[(numLines + 1) * 6 + a * 6 + 0] = startX;
            gridCoords[(numLines + 1) * 6 + a * 6 + 1] = 0.0f;
            gridCoords[(numLines + 1) * 6 + a * 6 + 2] = startZ + a * zStep;
            
            gridCoords[(numLines + 1) * 6 + a * 6 + 3] = endX;
            gridCoords[(numLines + 1) * 6 + a * 6 + 4] = 0.0f;
            gridCoords[(numLines + 1) * 6 + a * 6 + 5] = startZ + a * zStep;
        }
        ByteBuffer gbb = ByteBuffer.allocateDirect(gridCoords.length * 4);
        gbb.order(ByteOrder.nativeOrder());
        gridVB = gbb.asFloatBuffer();
        gridVB.put(gridCoords);
        gridVB.position(0);
    }
}