#ifndef _GESTURE_H
#define _GESTURE_H

#include <vec3.hpp>
#include <tpl.h>


char* gesture_format_string = "S(fff)";

struct gesture {

    struct vec3 movement;

};

size_t gesture_size() {
    size_t size;
    gesture g;
    tpl_node *tn;
    tn = tpl_map(gesture_format_string, &g);
    tpl_pack(tn, 0);
    tpl_dump(tn, TPL_GETSIZE, &size);
    tpl_free(tn);
    return size;
}

void serialize_gesture(const gesture& g, void* buffer, size_t bufsize) {
    tpl_node *tn;
    tn = tpl_map(gesture_format_string, g);
    tpl_pack(tn, 0);
    tpl_dump(tn, TPL_MEM|TPL_PREALLOCD, buffer, bufsize);
    tpl_free(tn);
}

gesture deserialize_gesture(void* buffer, size_t bufsize) {
    gesture g;
    tpl_node *tn;
    tn = tpl_map(gesture_format_string, &g);
    tpl_load(tn, TPL_MEM, buffer, bufsize);
    tpl_unpack(tn, 0);
    tpl_free(tn);
    return g;
}

#endif