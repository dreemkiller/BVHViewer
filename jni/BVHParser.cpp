#include <jni.h>
#include <string.h>
#include <android/log.h>
#include <vector>

#define ERROR_CHECK 0
#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG_TAG "BVHParser_Native"

char delimiters[] = " \n";

typedef enum _BVHNodeType_ {
    BVHNTRoot, BVHNTJoint, BVHNTEnd
} BVHNodeType;

typedef enum _BVHChannel_ {
    Xposition, Yposition, Zposition, Zrotation, Yrotation, Xrotation
} BVHChannel;


class BVHNode {
	public:
    BVHNodeType type;
    char *name;
    double *offset;
    int numChannels;
    BVHChannel *channels;
    std::vector<BVHNode> children;
    BVHNode() {
        //children = new std::vector<BVHNode>();
    }
};

BVHNode *current_node;

void ParseRoot();
void ParseMotion();
double* ParseOffsets();
BVHChannel* ParseChannels(int *numChannels);
void ParseJoint(BVHNode jointNode);
void ParseEnd(BVHNode endNode);
BVHChannel EnumerateChannel(char *channelStr);
BVHNode root;
double **channelValues;
int totalNumChannels;
int numFrames;

void Java_com_DreemKiller_BVHViewer_BVHLoader_BVHParse(JNIEnv *env, jobject jthis, jbyte *data) {
	char *token = strtok((char*)data, delimiters);
	while(token != NULL) {
	 	if (0 == strcmp(token, "HIERARCHY")) {
                // do nothing. This keyword is sort of useless
        }else if (0 == strcmp(token, "ROOT")) {
        	ParseRoot();
		}else if (0 == strcmp(token, "MOTION")) {
        	ParseMotion();
		}else {
        	// we've got a problem
		}
		token = strtok(NULL, delimiters);
	}
	current_node = &root;
}

BVHNodeType Java_com_DreemKiller_BVHViewer_BVHLoader_GetCurrentNodeType() {
	return current_node->type;
}

char* Java_com_DreemKiller_BVHViewer_BVHLoader_GetCurrentName() {
	return current_node->name;
}

double Java_com_DreemKiller_BVHViewer_BVHLoader_GetOffset(int index) {
	return current_node->offset[index];
}

int Java_com_DreemKiller_BVHViewer_BVHLoader_GetNumChannels() {
	return current_node->numChannels;
}

BVHChannel Java_com_DreemKiller_BVHViewer_BVHLoader_GetChannel(int index) {
	return current_node->channels[index];
}

void ParseRoot() {
	char *token = strtok(NULL, delimiters);
	if(ERROR_CHECK && 0 != strcmp(token, "{")) {
	    // problem here
	}
	token = strtok(NULL, delimiters);
	if (ERROR_CHECK && 0 != strcmp(token, "OFFSET")) {
	    // problem here
	}
	root.offset = ParseOffsets();
	
	token = strtok(NULL, delimiters);
	if (ERROR_CHECK && 0 != strcmp(token, "CHANNELS")) {
	    // problem here
	}
	root.channels = ParseChannels(&root.numChannels);
	
	token = strtok(NULL, delimiters);
	while(0 != strcmp(token, "}")) {
	    if (0 == strcmp(token, "JOINT")) {
	        BVHNode newNode;
	        newNode.type = BVHNTJoint;
	        root.children.push_back(newNode);
	        ParseJoint(newNode);
	    }else if (0 == strcmp(token, "End")) {
	        BVHNode newNode;
	        newNode.type = BVHNTEnd;
	        root.children.push_back(newNode);
	        ParseEnd(newNode);
	    }else
	    {
	        // problem here
	    }
	    token = strtok(NULL, delimiters);
	}
}

void ParseEnd(BVHNode endNode) {
    char *token = strtok(NULL, delimiters);
    if (ERROR_CHECK && 0 != strcmp(token, "Site")) {
        // problem here
    }
    token = strtok(NULL, delimiters);
    if (ERROR_CHECK && 0 != strcmp(token, "{")) {
        // problem here
    }
    token = strtok(NULL, delimiters);
    if (ERROR_CHECK && 0 != strcmp(token, "OFFSET")) {
        // problem here
    }
    endNode.offset = ParseOffsets();
    token = strtok(NULL, delimiters);
    if (ERROR_CHECK && 0 != strcmp(token, "}")) {
        // problem here
    }
}

void ParseMotion() {
	char *token = strtok(NULL, delimiters);
	if (ERROR_CHECK && 0 != strcmp(token, "Frames:")) {
	    // problem here
	}
	token = strtok(NULL, delimiters);
	numFrames = (int)strtol(token, NULL, 10);
	token = strtok(NULL, delimiters);
	if (ERROR_CHECK && 0 != strcmp(token, "Frame")) {
	    // problem here
	}
	token = strtok(NULL, delimiters);
	if (ERROR_CHECK && 0 != strcmp(token, "Time:")) {
	    // problem here
	}
	token = strtok(NULL, delimiters);
	channelValues = new double  *[numFrames];
	for (int a = 0; a < numFrames; a++) {
		channelValues[a] = new double[totalNumChannels];
	}
	double frameTime = strtod(token, NULL);
	for (int a = 0; a < numFrames; a++) {
	    for (int b = 0; b < totalNumChannels; b++) {
	        token = strtok(NULL, delimiters);
	        channelValues[a][b] = strtod(token, NULL);
	    }
	}
}

double* ParseOffsets() {
	double *offsets = new double[3];
	for (int i = 0; i < 3; i++) {
	    char *token = strtok(NULL, delimiters);
	    offsets[i] = strtod(token, NULL);
	}
	return offsets;
}

BVHChannel* ParseChannels(int *numChannels) {
	char *token = strtok(NULL, delimiters);
	*numChannels = (int) strtol(token, NULL, 10);
	totalNumChannels += *numChannels;
	BVHChannel *channels = new BVHChannel[*numChannels];
	for (int i = 0; i < *numChannels; i++) {
	    token = strtok(NULL, delimiters);
	    channels[i] = EnumerateChannel(token);
	}
	return channels;
}

void ParseJoint(BVHNode jointNode) {
	char *token = strtok(NULL, delimiters);
    jointNode.name = token;
    token = strtok(NULL, delimiters);
    if (ERROR_CHECK && 0 != strcmp(token, "{")) {
        // problem here
    }
    token = strtok(NULL, delimiters);
    if (ERROR_CHECK && 0 != strcmp(token, "OFFSET")) {
        // problem here
    }
    jointNode.offset = ParseOffsets();
    token = strtok(NULL, delimiters);
    if (ERROR_CHECK && 0 != strcmp(token, "CHANNELS")) {
         // problem here
    }
    jointNode.channels = ParseChannels(&jointNode.numChannels);
    token = strtok(NULL, delimiters);
    while (0 != strcmp(token, "}")) {
        if (0 == strcmp(token, "JOINT")) {
            BVHNode newNode;
            newNode.type = BVHNTJoint;
            jointNode.children.push_back(newNode);
            ParseJoint(newNode);
        }else if (0 == strcmp(token, "End")) {
            BVHNode newNode;
            newNode.type = BVHNTEnd;
            jointNode.children.push_back(newNode);
            ParseEnd(newNode);
        }
        token = strtok(NULL, delimiters);
    }
}

BVHChannel EnumerateChannel(char *channelStr) {
    if (0 == strcmp(channelStr, "Xposition")) {
        return Xposition;
    }else if (0 == strcmp(channelStr, "Yposition")) {
        return Yposition;
    }else if (0 == strcmp(channelStr, "Zposition")) {
        return Zposition;
    }else if (0 == strcmp(channelStr, "Zrotation")) {
        return Zrotation;
    }else if (0 == strcmp(channelStr, "Yrotation")) {
        return Yrotation;
    }else if (0 == strcmp(channelStr, "Xrotation")) {
        return Xrotation;
    }else {
        // problem here
    }
}

int CGetNumFrames() {
	return numFrames;
}

#ifdef __cplusplus
}
#endif