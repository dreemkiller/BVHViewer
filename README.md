BVHViewer
=========

BVH File viewer
Android app for viewing Biovision BVH files. It creates a wire-frame for the skeleton of the subject, and then displays
the motion.

Currently, zooming in is done by tapping the top fourth of the screen, zooming out by tapping the bottom fourth.
Pan right is the right fourth, pan left is the left fourth.
It starts with a dialog for choosing among the bvh files included in the apk. In addition, it sets up an intent so its
opened whenever a BVH file is selected from the web or the file system.

Known bugs: 
1. the intent is imprecise - lots of things besides bvh files are caught up in it.
2. Pan left and pan right have some vertical drift to them.
3. Occasionally crashes - still haven't root-caused.

Desired enhancements:
1. change the zoom functionality to pinch to zoom.
2. Automatically scale the field of vision and background grid to match the location of the subject from the BFH

