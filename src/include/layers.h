#ifndef LAYERS_H
#define LAYERS_H
#include "../external/raylib.h"
#include "frames.h"
#include <stdbool.h>

typedef struct LayerObj {
    int index;
    Image img;
    Texture2D txt;
    int mode;
    char *name;
    bool visible;
    float opacity;

    FrameList *flist;
} LayerObj;

LayerObj *NewLayerObj(int w, int h, int framecount);
void FreeLayerObj(LayerObj *layer);
void SyncImgLayerObj(LayerObj *layer);
LayerObj *DuplicateLayerObj(const LayerObj *layer);

typedef struct LayerList {
    LayerObj **layers;
    int count;
} LayerList;

LayerList *NewLayerList(int w, int h);
void FreeLayerList(LayerList *list);
void AddToLayerList(LayerList *list, LayerObj *layer);
LayerObj *RemoveIdxLayerList(LayerList *list, int index);
bool DuplicateIdxLayerList(LayerList *list, int index);
bool MoveIdxLayerList(LayerList *list, int from, int to);
bool MoveIdxBottomLayerList(LayerList *list, int idx);
LayerObj *GetLayerFromList(const LayerList *list, int index);
#endif // LAYERS_H
