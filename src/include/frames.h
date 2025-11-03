#ifndef FRAMES_H
#define FRAMES_H

#include "../external/raylib.h"
#include <stdbool.h>

// Frame Object
typedef struct FrameObj {
	//Frame index
    int index;
	// Actual Image for the frame
    Image img;
	//Texture of the Image
    Texture txt;
	// Animation speed this frame
    int speed;
} FrameObj;

// Create New Heap Allocated Frame Object
// `w` = Width of the Frame Image
// `h` = Height of the Frame Image
FrameObj *NewFrameObj(int w, int h);
// Free Frame Object
void FreeFrameObj(FrameObj *frame);
// Duplicate the Frame
FrameObj *DuplicateFrameObj(const FrameObj *frame);
// Sync Image and Textures of the Frame
bool SyncFrameObj(FrameObj *frame);

// Frame List (uses stb_ds arrays)
typedef struct FrameList {
	// Frame array pointer
    FrameObj **frames;
	// Count of frames
    int count;
} FrameList;

// Create New Frame List (The frames will have same dimensions)
FrameList *NewFrameList(int w, int h);
// Free the frame list
void FreeFrameList(FrameList *list);
// Duplicate the whole Frame list (Not yet implemented)
FrameList *DuplicateFrameList(const FrameList *list);
// Add a new Frame at the end of the list
void AddToFrameList(FrameList *list, FrameObj *frame);
// Remove the frame at the index and return it
// `list` = Frame List
// `index` = index of the frame to remove
// Returns => The just removed frame
FrameObj *RemoveIdxFrameList(FrameList *list, int index);
// Duplicate the frame which is at `index` and put the duplicate frame after it
// `list` = Frame list
// `index` = Index of the frame to duplicate
// Returns => `true` if task completed otherwise returns `false` the task failed
bool DuplicateIdxFrameList(FrameList *list, int index);
// Move the frame at index `from` to index `to`
// `list` = Frame list
// `from` = index of the frame which we intend to move
// `to` = Where to move the frame (If out of range, returns `false`)
// Returns => `true` if task completed successfully otherwise `false`
bool MoveIdxFrameList(FrameList *list, int from, int to);
// Get the Frame which is at `index`
// `list` = Frame list
// `index` = Index of the frame which intend to fetch
// Returns => The pointer to the frame is found, otherwise returns NULL if index
// is invalid or out of range
FrameObj *GetFrameFromList(const FrameList *list, int index);

#endif
