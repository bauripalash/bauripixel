#ifndef LAYERS_H
#define LAYERS_H
#include "../external/raylib.h"

typedef struct LayerObj {
    int index;
    Image img;
    Texture2D txt;
    int mode;
    char *name;
} LayerObj;

LayerObj *NewLayerObj(int w, int h);
void FreeLayerObj(LayerObj *layer);
void SyncImgLayerObj(LayerObj *layer);

typedef struct LayerList {
    LayerObj **layers;
    int count;
} LayerList;

LayerList *NewLayerList(int w, int h);
void FreeLayerList(LayerList *list);
void AddToLayerList(LayerList *list, LayerObj *layer);
void RemoveIdxLayerList(LayerList *list, int index);

#endif // LAYERS_H
