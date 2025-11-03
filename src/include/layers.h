#ifndef LAYERS_H
#define LAYERS_H
#include "../external/raylib.h"
#include "frames.h"
#include <stdbool.h>

// Layer Object
typedef struct LayerObj {
	// Index of the layer
    int index;
	// Mode ? (Does nothing)
    int mode;
	// Name of the layer
    char *name;
	// Is the layer visible
    bool visible;
	// Visibility of the layer 0.0 to 1.0
    float opacity;
	// Width of the layer (same as sprite)
    int width;
	// Height of the layer (same as sprite)
    int height;

    FrameList *flist;
} LayerObj;

// Create New Layer Object
// `w` = Width of the layer (for creating underlying frame image, textures)
// `h` = Height of the layer (for creating underlying frame image, textures)
// (Height and Width is same as Sprite)
// `framecount` = How many blank frames to create
// Returns => Newly Heap Allocated Layer or `NULL` on allocation failure
LayerObj *NewLayerObj(int w, int h, int framecount);
// Free the layer object
void FreeLayerObj(LayerObj *layer);
// Sync the specified frame's image and texture
// `layer` = Layer which to sync 
// `frame` = Index of the frame which to sync
void SyncImgLayerObj(LayerObj *layer, int frame);
// Duplicate The Layer
// Returns => Newly created Duplicate of `layer` or `NULL` on allocation failure
LayerObj *DuplicateLayerObj(const LayerObj *layer);

// Layer List
typedef struct LayerList {
	// Array of Layer Objects
    LayerObj **layers;
	// Count of layers
    int count;
} LayerList;

// Create new layer list 
// `w` = Sprite Width
// `h` = Sprite Height
LayerList *NewLayerList(int w, int h);
// Free the layer list and all underlying layers
void FreeLayerList(LayerList *list);
// Add new layer to list
// `list` = Layer list
// `layer` = Layer to add
void AddToLayerList(LayerList *list, LayerObj *layer);
// Remove the layer at index `index` from the list
// `list` = Layer list
// `index` = Index of the layer to remove
// Returns => The just removed layer
LayerObj *RemoveIdxLayerList(LayerList *list, int index);
// Duplicate the layer at index `index`
// `list` = Layer list
// `index` = Index of the layer which we intend to duplicate
// Returns => `true` on success otherwise `false`
bool DuplicateIdxLayerList(LayerList *list, int index);
// Move the layer at index `from` to index `to`
// `list` = Layer list
// `from` = index of the layer which we intend to move
// `to` = Where to move the layer (If out of range, returns `false`)
// Returns => `true` if task completed successfully otherwise `false`
bool MoveIdxLayerList(LayerList *list, int from, int to);
// Move the layer at index `idx` to the bottom of the list
// `list` = Layer list
// `idx` = Index of the layer to move
// Returns => `true` if moved successfully otherwise `false`
// Also returns `false` if index is invalid or out of range
bool MoveIdxBottomLayerList(LayerList *list, int idx);
// Get the Layer at index `index` from the list
// Returns => Layer object from the List from index `index`
// returns null if index is invalid, out of range or list is empty
LayerObj *GetLayerFromList(const LayerList *list, int index);
#endif // LAYERS_H
