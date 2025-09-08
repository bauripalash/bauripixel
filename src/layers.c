#include "include/layers.h"
#include "external/raylib.h"
#include "external/stb/stb_ds.h"
#include "include/utils.h"
#include <stdbool.h>
#include <stdlib.h>

LayerObj *NewLayerObj(int w, int h) {
    LayerObj *l = malloc(sizeof(LayerObj));
    if (l == NULL) {
        return NULL;
    }

    l->index = 0;
    l->mode = 0;
    l->name = NULL;
    l->visible = true;
    l->opacity = 1.0f;

    l->img = GenImageColor(w, h, BLANK);
    l->txt = LoadTextureFromImage(l->img);

    return l;
}

void FreeLayerObj(LayerObj *layer) {
    if (layer == NULL) {
        return;
    }

    if (layer->name != NULL) {
        free(layer->name);
    }

    if (layer->img.data == NULL) {
        return;
    }

    UnloadTexture(layer->txt);
    UnloadImage(layer->img);
    free(layer);
}

void SyncImgLayerObj(LayerObj *layer) {
    if (layer == NULL || layer->img.data == NULL) {
        return;
    }

    UpdateTexture(layer->txt, layer->img.data);
}

LayerObj *DuplicateLayerObj(const LayerObj *layer) {
    LayerObj *l = malloc(sizeof(LayerObj));
    if (l == NULL) {
        return NULL;
    }
    l->index = layer->index;
    l->mode = layer->mode;
    if (layer->name == NULL) {
        l->name = MakeString("Duplicate Layer");
    } else {
        l->name = MakeString(TextFormat("%s (Copy)", layer->name));
    }

    if (l->name == NULL) {
        free(l);
        return NULL;
    }

    l->visible = layer->visible;
    l->opacity = layer->opacity;

    l->img = ImageCopy(layer->img);
    l->txt = LoadTextureFromImage(l->img);

    return l;
}

LayerList *NewLayerList(int w, int h) {
    LayerList *ll = malloc(sizeof(LayerList));
    if (ll == NULL) {
        return NULL;
    }

    ll->layers = NULL;

    LayerObj *initLayer = NewLayerObj(w, h);
    if (initLayer == NULL) {
        free(ll);
        return NULL;
    }
    initLayer->index = 0;
    initLayer->name = MakeString("Layer 0");

    AddToLayerList(ll, initLayer);

    ll->count = 1;

    return ll;
}

void FreeLayerList(LayerList *list) {
    if (list == NULL) {
        return;
    }

    if (list->layers == NULL) {
        free(list);
        return;
    }

    int count = arrlen(list->layers);
    while (count > 0) {
        FreeLayerObj(list->layers[count - 1]);
        arrpop(list->layers);
        count--;
    }

    arrfree(list->layers);

    free(list);
}

void AddToLayerList(LayerList *list, LayerObj *layer) {
    if (list == NULL || layer == NULL) {
        return;
    }

    arrput(list->layers, layer);

    list->count++;
}

static void syncLayerIndex(LayerList *list) {
    int len = arrlen(list->layers);
    for (int i = 0; i < len; i++) {
        LayerObj *lr = list->layers[i];
        if (lr != NULL) {
            lr->index = i;
        }
    }

    list->count = len;
}

LayerObj *RemoveIdxLayerList(LayerList *list, int index) {
    if (index < 0 || index >= list->count) {
        return NULL;
    }

    LayerObj *lr = list->layers[index];
    arrdel(list->layers, index);
    syncLayerIndex(list);
    return lr;
}

bool DuplicateIdxLayerList(LayerList *list, int index) {
    if (index < 0 || index >= list->count) {
        return false;
    }

    LayerObj *lr = list->layers[index];
    LayerObj *dupLr = DuplicateLayerObj(lr);
    if (dupLr == NULL) {
        return false;
    }
    dupLr->index += 1;

    if (index == (list->count - 1)) {
        arrput(list->layers, dupLr);
    } else {
        arrins(list->layers, dupLr->index, dupLr);
    }

    syncLayerIndex(list);

    return true;
}

bool MoveIdxLayerList(LayerList *list, int from, int to) {
    if (list == NULL) {
        return false;
    }
    if (list->layers == NULL) {
        return false;
    }
    int len = arrlen(list->layers);
    if (from < 0 || from >= len || to < 0 || to >= len) {
        return false;
    }

    if (from == to) {
        return true;
    }

    LayerObj *lr = list->layers[from];
    arrdel(list->layers, from);

    if (to > from) {
        to--; // fill the gap
    }

    arrins(list->layers, to, lr);

    syncLayerIndex(list);

    return true;
}

bool MoveIdxBottomLayerList(LayerList *list, int idx) {
    int len = arrlen(list->layers);

    if (len == 1) {
        return true;
    }

    if (list == NULL || idx < 0 || idx >= len) {
        return false;
    }
    LayerObj *lr = list->layers[idx];
    arrdel(list->layers, idx);
    arrput(list->layers, lr);
    syncLayerIndex(list);

    return true;
}

LayerObj *GetLayerFromList(const LayerList *list, int index) {
    if (list == NULL) {
        return NULL;
    }

    if (list->layers == NULL) {
        return NULL;
    }

    if (index < 0 || index >= arrlen(list->layers)) {
        return NULL;
    }

    return list->layers[index];
}
