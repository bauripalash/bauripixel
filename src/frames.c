#include "include/frames.h"
#include "external/raylib.h"
#include "external/stb/stb_ds.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

FrameObj *NewFrameObj(int w, int h) {
    FrameObj *f = (FrameObj *)malloc(sizeof(FrameObj));
    if (f == NULL) {
        return NULL;
    }

    f->index = 0;
    f->speed = 1;
    f->img = GenImageColor(w, h, BLANK);

    if (f->img.data == NULL) {
        free(f);
        return NULL;
    }

    f->txt = LoadTextureFromImage(f->img);

    return f;
}
void FreeFrameObj(FrameObj *frame) {
    if (frame == NULL) {
        return;
    }

    if (frame->img.data == NULL) {
        free(frame);
        return;
    }

    UnloadTexture(frame->txt);
    UnloadImage(frame->img);
    free(frame);
}
FrameObj *DuplicateFrameObj(const FrameObj *frame) {
    FrameObj *dup = (FrameObj *)malloc(sizeof(FrameObj));
    if (dup == NULL) {
        return NULL;
    }

    dup->index = frame->index;
    dup->speed = frame->speed;

    dup->img = ImageCopy(frame->img);
    if (dup->img.data == NULL) {
        free(dup);
        return NULL;
    }

    dup->txt = LoadTextureFromImage(dup->img);

    return dup;
}

FrameList *NewFrameList(int w, int h) {
    FrameList *list = (FrameList *)malloc(sizeof(FrameList));
    if (list == NULL) {
        return NULL;
    }

    list->frames = NULL;
    FrameObj *initFrame = NewFrameObj(w, h);
    if (initFrame == NULL) {
        free(list);
        return NULL;
    }
    initFrame->index = 0;
    AddToFrameList(list, initFrame);
    list->count = 1;
    FrameObj *twoFrame = NewFrameObj(w, h);
    if (twoFrame != NULL) {
        twoFrame->index = 1;
        AddToFrameList(list, twoFrame);
        list->count = 2;
    }

    return list;
}
void FreeFrameList(FrameList *list) {
    if (list == NULL) {
        return;
    }

    if (list->frames == NULL) {
        free(list);
        return;
    }

    int count = arrlen(list->frames);
    while (count > 0) {
        FrameObj *f = arrpop(list->frames);
        FreeFrameObj(f);
        count--;
    }
    arrfree(list->frames);
    free(list);
    list = NULL;
}

static void syncFrameIndexes(FrameList *list) {
    if (list == NULL) {
        return;
    }

    if (list->frames == NULL) {
        return;
    }

    int len = arrlen(list->frames);
    for (int i = 0; i < len; i++) {
        FrameObj *f = list->frames[i];
        if (f != NULL) {
            f->index = i;
        }
    }

    list->count = len;
}
void AddToFrameList(FrameList *list, FrameObj *frame) {
    if (list == NULL || frame == NULL) {
        return;
    }

    arrput(list->frames, frame);
    frame->index = list->count;
    list->count++;
    syncFrameIndexes(list);
}
FrameObj *RemoveIdxFrameList(FrameList *list, int index) {
    if (list == NULL) {
        return NULL;
    }

    if (list->frames == NULL) {
        return NULL;
    }

    if (index < 0 || index >= list->count) {
        return NULL;
    }
    FrameObj *f = list->frames[index];
    arrdel(list->frames, index);
    list->count--;
    syncFrameIndexes(list);

    return f;
}
bool DuplicateIdxFrameList(FrameList *list, int index) {
    if (list == NULL) {
        return false;
    }

    if (list->frames == NULL) {
        return false;
    }

    if (index < 0 || index >= list->count) {
        return false;
    }

    FrameObj *og = list->frames[index];
    if (og == NULL) {
        return false;
    }
    FrameObj *dup = DuplicateFrameObj(og);
    if (dup == NULL) {
        return false;
    }
    dup->index++;

    if (index == (list->count - 1)) {
        arrput(list->frames, dup);
    } else {
        arrins(list->frames, dup->index, dup);
    }

    syncFrameIndexes(list);

    return true;
}
bool MoveIdxFrameList(FrameList *list, int from, int to) {
    if (list == NULL) {
        return false;
    }

    if (list->frames == NULL) {
        return false;
    }

    int len = arrlen(list->frames);
    if (from < 0 || from >= len || to < 0 || to >= len) {
        return false;
    }

    if (from == to) {
        return true;
    }

    FrameObj *f = list->frames[from];
    arrdel(list->frames, from);
    if (to > from) {
        to--; // fill the gap? IDK
    }

    arrins(list->frames, to, f);
    syncFrameIndexes(list);
    return true;
}
FrameObj *GetFrameFromList(const FrameList *list, int index) {
    if (list == NULL || index < 0 || index >= list->count) {
        return NULL;
    }

    return list->frames[index];
}
