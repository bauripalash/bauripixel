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
    initLayer->index = 0;
    initLayer->name = MakeString("Layer 0");
    if (initLayer == NULL) {
        free(ll);
        return NULL;
    }

    AddToLayerList(ll, initLayer);

    ll->count = 1;

    return ll;
}

void FreeLayerList(LayerList *list) {
    if (list == NULL) {
        return;
    }

    if (list->layers == NULL) {
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
void RemoveIdxLayerList(LayerList *list, int index) { return; }
