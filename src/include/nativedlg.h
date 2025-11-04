#ifndef NATIVE_DLG_H
#define NATIVE_DLG_H

#include <stdbool.h>
// Wrapper for Native Open File Dialog
bool BpOpenFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
);

// Wrapper for Native Save File Dialog
bool BpSaveFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
);

#endif
