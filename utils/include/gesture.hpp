#ifndef _GESTURE_H
#define _GESTURE_H

#include <vec3.hpp>
#include <tpl.h>


char* gesture_format_string = "S($(fff)f)";

struct gesture {
    struct vec3 movement;
    double rotation;
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

void serialize_gesture(const gesture g, void* buffer, size_t bufsize) {
    tpl_node *tn;
    tn = tpl_map(gesture_format_string, &g);
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

void test_serialization() {
    size_t size = gesture_size();
    char buffer[size];
    gesture g0;
    g0.movement = {1.0, 2.0, 3.0};
    gesture g1;
    g1.movement = vec3::zero();

    // This works
    /*tpl_node *tn0;
    tn0 = tpl_map(gesture_format_string, &g0);
    tpl_pack(tn0, 0);
    tpl_dump(tn0, TPL_MEM|TPL_PREALLOCD, buffer, size);
    tpl_free(tn0);

    tpl_node *tn1;
    tn1 = tpl_map(gesture_format_string, &g1);
    tpl_load(tn1, TPL_MEM, buffer, size);
    tpl_unpack(tn1, 0);
    tpl_free(tn1);*/

    serialize_gesture(g0, buffer, size);
    g1 = deserialize_gesture(buffer, size);

    printf("Original gesture: %.3f %.3f %.3f", g0.movement.x, g0.movement.y, g0.movement.z);
    printf("Serialized gesture: %.3f %.3f %.3f", g1.movement.x, g1.movement.y, g1.movement.z);
}
#endif