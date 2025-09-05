#ifndef NATIVE_DLG_H
#define NATIVE_DLG_H

#include <stdbool.h>
bool BpOpenFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
);
bool BpSaveFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
);

#endif
