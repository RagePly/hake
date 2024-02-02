#include <hake/util/paths.h>
#include <string.h>
#include <stdlib.h>

char *hake_path_get_leaf(char *path) {
    char *leaf = path;
    int l = strlen(path);
    for (int i = 0; i < l; i++)
        if (path[i] == '/')
            leaf = &path[i+1];
    return leaf;
}

char *hake_path_get_extension(char *path) {
    int l = strlen(path);
    char *extension = &path[l];
    for (int i = l; i >= 0; i--) {
        if (path[i] == '.') {
            extension = &path[i];
            break;
        }
    }
    return extension;
}

char *hake_path_new_without_extension(char *path) {
    char *extension = hake_path_get_extension(path);
    unsigned int length = (unsigned int) (extension - path);
    char *without = malloc(length + 1);    
    without[length] = 0;
    memcpy(without, path, length);
    return without;
}
