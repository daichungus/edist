#ifndef EDIT_DISTANCE_H
#define EDIT_DISTANCE_H

#include <stddef.h>

int edit_distance_base(const char *str1, const char *str2, size_t len1, size_t len2, size_t tile_size, size_t num_threads);
int edit_distance(const char *str1, const char *str2, size_t len1, size_t len2);

#endif