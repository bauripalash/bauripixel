#ifndef UTILS_H
#define UTILS_H

#define ArrCount(arr) (sizeof(arr) / sizeof(arr[0]))

#define TraceVector(vec, msg)                                                  \
    TraceLog(LOG_WARNING, "%s [%f, %f]", msg, vec.x, vec.y)

#define TraceRect(r, msg)                                                      \
    TraceLog(                                                                  \
        LOG_WARNING, "%s [[%f, %f] [%f, %f]]", msg, r.x, r.y, r.width,         \
        r.height                                                               \
    )

#endif
