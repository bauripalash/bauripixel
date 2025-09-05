#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "external/raylib.h"
#ifndef BPM_OS_WEB
#include "external/tfd/tinyfiledialogs.h"
#endif

#include "include/nativedlg.h"

bool BpOpenFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
) {
#ifndef BPM_OS_WEB
    bool result = false;
    int nFilter = 0;
    char **filterList = TextSplit(filters, ';', &nFilter);

    return result;
#else
    return false;
#endif
}
bool BpSaveFileDialog(
    const char *title, char *filename, const char *filters, const char *msg
) {
#ifndef BPM_OS_WEB
    bool result = false;
    int nFilter = 0;
    const char **filterList = (const char **)TextSplit(filters, ';', &nFilter);

    char *temp =
        tinyfd_saveFileDialog(title, filename, nFilter, filterList, msg);

    if (temp != NULL) {
        strcpy(filename, temp);
        result = true;
    }

    return result;
#else
    return false;
#endif
}
