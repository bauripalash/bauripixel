#ifndef FRAMES_H
#define FRAMES_H

#include "../external/raylib.h"
#include <stdbool.h>

typedef struct FrameObj {
    int index;
    Image img;
    Texture txt;
    int speed;
} FrameObj;

FrameObj *NewFrameObj(int w, int h);
void FreeFrameObj(FrameObj *frame);
FrameObj *DuplicateFrameObj(const FrameObj *frame);

typedef struct FrameList {
    FrameObj **frames;
    int count;
} FrameList;

FrameList *NewFrameList(int w, int h);
void FreeFrameList(FrameList *list);
void AddToFrameList(FrameList *list, FrameObj *frame);
FrameObj *RemoveIdxFrameList(FrameList *list, int index);
bool DuplicateIdxFrameList(FrameList *list, int index);
bool MoveIdxFrameList(FrameList *list, int from, int to);
FrameObj *GetFrameFromList(const FrameList *list, int index);

#endif
