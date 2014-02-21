package com.DreemKiller.BVHViewer;

import java.io.BufferedInputStream;
import java.io.ByteArrayInputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.StreamTokenizer;
import java.util.StringTokenizer;

import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Resources;

// there is intentionally unreachable code in this class, due to optimizations
@SuppressWarnings("unused")
public class BVHLoader {
    private StringTokenizer tokenizer;
    
    // change to true to enable parser error checking. It is quite slow, however.
    private static final boolean ERROR_CHECK = false;
    
    public BVHNode root;
    public double [][] channelValues;
    private int totalNumChannels;
    //private native void BVHParse(InputStream is);
    //static {
    //    System.loadLibrary("ndk1");
    //}
    public void BVHStringReader(String data) throws IOException, BVHParseException{
        tokenizer = new StringTokenizer(data);

        int ttype = 0;
        //Debug.startMethodTracing("bvhviewer");
        while(tokenizer.hasMoreTokens()) {
            
            String currentString = tokenizer.nextToken();
            if (currentString.equals("HIERARCHY")) {
                // do nothing. This keyword is sort of useless
            }else if (currentString.equals("ROOT")) {
                ParseRoot();
            }else if (currentString.equals("MOTION")) {
                ParseMotion();
            }else {
                throw new BVHParseException(currentString.toString());
            }
        }
    }
    public BVHLoader (Context context, String data, Integer crap) throws IOException, BVHParseException{
        BVHStringReader(data);
    }
    public BVHLoader (Context context, String filename) throws FileNotFoundException, IOException, BVHParseException{
        totalNumChannels = 0;

        Resources resources = context.getResources();
        AssetManager assetManager = resources.getAssets();
        InputStream is = assetManager.open(filename);
        int length = is.available();
        byte[] data = null; 
        data = new byte[length];
        is.read(data);
        is.close();
        String stringData = new String(data);
        //BVHParse(data);
        //InputStreamReader myReader = new InputStreamReader(is);
        BVHStringReader(stringData);
        
        //Debug.stopMethodTracing();
    }
    
    private void ParseRoot() throws IOException, BVHParseException
    {
        int ttype = 0;
        root = new BVHNode();
        root.type = BVHNodeType.BVHNTRoot;
        root.name = tokenizer.nextToken();
        String currentString = tokenizer.nextToken();
        if(ERROR_CHECK && !currentString.equals("{")) {
            throw new BVHParseException(currentString.toString());
        }
        currentString = tokenizer.nextToken();
        if (ERROR_CHECK && !currentString.equals("OFFSET")) {
            throw new BVHParseException(currentString.toString());
        }
        root.offset = ParseOffsets();

        String keyword = tokenizer.nextToken();
        if (ERROR_CHECK && !keyword.equals("CHANNELS")) {
            throw new BVHParseException(keyword.toString());
        }
        root.channels = ParseChannels();

        keyword = tokenizer.nextToken();
        while(!keyword.equals("}")){
            if (keyword.equals("JOINT")) {
                BVHNode newNode = new BVHNode();
                newNode.type = BVHNodeType.BVHNTJoint;
                root.children.add(newNode);
                ParseJoint(newNode);
            }else if (keyword.equals("End")) {
                BVHNode newNode = new BVHNode();
                newNode.type = BVHNodeType.BVHNTEnd;
                root.children.add(newNode);
                ParseEnd(newNode);
            }else
            {
                throw new BVHParseException(keyword.toString());
            }
            keyword = tokenizer.nextToken();
        }
    }
    
    private void ParseJoint(BVHNode jointNode) throws IOException, BVHParseException {
        jointNode.name = tokenizer.nextToken();
        String token = tokenizer.nextToken();
        if (ERROR_CHECK && !token.equals("{")) {
            throw new BVHParseException(token.toString());
        }
        token = tokenizer.nextToken();
        if (ERROR_CHECK && !token.equals("OFFSET")) {
            throw new BVHParseException(token.toString());
        }
        jointNode.offset = ParseOffsets();
        token = tokenizer.nextToken();
        if (ERROR_CHECK && !token.equals("CHANNELS")) {
            throw new BVHParseException(token.toString());
        }
        jointNode.channels = ParseChannels();
        token = tokenizer.nextToken();
        while (!token.equals("}")) {
            if (token.equals("JOINT")) {
                BVHNode newNode = new BVHNode();
                newNode.type = BVHNodeType.BVHNTJoint;
                jointNode.children.add(newNode);
                ParseJoint(newNode);
            }else if (token.equals("End")) {
                BVHNode newNode = new BVHNode();
                newNode.type = BVHNodeType.BVHNTEnd;
                jointNode.children.add(newNode);
                ParseEnd(newNode);
            }
            token = tokenizer.nextToken();
        }
    }
    
    private double[] ParseOffsets() throws IOException, BVHParseException{
        double [] offsets = new double[3];
        for (int i = 0; i < 3; i++) {
            String token = tokenizer.nextToken();
            offsets[i] = Double.parseDouble(token);
        }
        return offsets;
    }

    private BVHChannel[] ParseChannels() throws IOException, BVHParseException {
        String token = tokenizer.nextToken();
        int numChannels = Integer.parseInt(token);
        totalNumChannels += numChannels;
        BVHChannel[] channels = new BVHChannel[numChannels];
        for (int i = 0; i < numChannels; i++) {
            token = tokenizer.nextToken();
            channels[i] = EnumerateChannel(token);
        }
        return channels;
    }

    private void ParseEnd(BVHNode endNode) throws IOException, BVHParseException {
        String token = tokenizer.nextToken();
        if (ERROR_CHECK && !token.equals("Site")) {
            throw new BVHParseException(token.toString());
        }
        token = tokenizer.nextToken();
        if (ERROR_CHECK && !token.equals("{")) {
            throw new BVHParseException(token.toString());
        }
        token = tokenizer.nextToken();
        if (ERROR_CHECK && !token.equals("OFFSET")) {
            throw new BVHParseException(token.toString());
        }
        endNode.offset = ParseOffsets();
        token = tokenizer.nextToken();
        if (ERROR_CHECK && !token.equals("}")) {
            throw new BVHParseException(token.toString());
        }
    }

    private BVHChannel EnumerateChannel(String channelStr) throws BVHParseException {
        if (channelStr.equals("Xposition")) {
            return BVHChannel.Xposition;
        }else if (channelStr.equals("Yposition")) {
            return BVHChannel.Yposition;
        }else if (channelStr.equals("Zposition")) {
            return BVHChannel.Zposition;
        }else if (channelStr.equals("Zrotation")) {
            return BVHChannel.Zrotation;
        }else if (channelStr.equals("Yrotation")) {
            return BVHChannel.Yrotation;
        }else if (channelStr.equals("Xrotation")) {
            return BVHChannel.Xrotation;
        }else {
            throw new BVHParseException(channelStr);
        }
    }

    private void ParseMotion() throws IOException, BVHParseException {
        String token = tokenizer.nextToken();
        if (ERROR_CHECK && !token.equals("Frames:")) {
            throw new BVHParseException(token.toString());
        }
        token = tokenizer.nextToken();
        int numFrames = Integer.parseInt(token);
        token = tokenizer.nextToken();
        if (ERROR_CHECK && !token.equals("Frame")) {
            throw new BVHParseException(token.toString());
        }
        token = tokenizer.nextToken();
        if (ERROR_CHECK && !token.equals("Time:")) {
            throw new BVHParseException(token.toString());
        }
        token = tokenizer.nextToken();
        channelValues = new double[numFrames][totalNumChannels];
        double frameTime = Double.parseDouble(token);
        for (int a = 0; a < numFrames; a++) {
            for (int b = 0; b < totalNumChannels; b++) {
                token = tokenizer.nextToken();
                channelValues[a][b] = Double.parseDouble(token);
            }
        }
    }
}