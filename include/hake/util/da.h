#ifndef _HAKE_DA_H_
#define _HAKE_DA_H_

#define HAKE_DA_INIT_CAPACITY 8
#define HAKE_TEMPLATE_DYNAMIC_ARRAY_DEFINE(type,name) \
    typedef struct name ## _t { int capacity; int size; type *data; } * name; \
    name name ## _init(); \
    void name ## _free(name da); \
    type name ## _remove(name da, int index); \
    int  name ## _find(name da, type *ref, int (*compare)(type *, type *)); \
    void name ## _extend2(name da); \
    void name ## _shrink2(name da); \
    void name ## _append(name da, type x);

#define HAKE_TEMPLATE_DYNAMIC_ARRAY(type, name) \
    name name ## _init() { \
        name da = malloc(sizeof(struct name ## _t)); \
        da->size = 0; \
        da->capacity = HAKE_DA_INIT_CAPACITY; \
        da->data = malloc(sizeof(type) * HAKE_DA_INIT_CAPACITY); \
        return da; \
    } \
    void name ## _free(name da) { \
        free(da->data); \
        free(da); \
    } \
    type name ## _remove(name da, int index) { \
        type item = da->data[index]; \
        for (int i = index; i < da->size -1; i++) { \
            da->data[i] = da->data[i+1]; \
        } \
        return item; \
    } \
    int  name ## _find(name da, type *ref, int (*compare)(type *, type *)) { \
        for (int i = 0; i < da->size; i++) \
            if (compare(ref, &da->data[i])) \
                return i; \
        return -1; \
    } \
    void name ## _extend2(name da) { \
        da->data = realloc(da->data, sizeof(type)*da->capacity*2); \
        da->capacity *= 2; \
    } \
    void name ## _shrink2(name da) { \
        da->data = realloc(da->data, sizeof(type)*da->capacity/2); \
        da->capacity /= 2; \
    } \
    void name ## _append(name da, type x) { \
        if ( da->size >= da->capacity ) \
            name ## _extend2(da); \
        da->data[da->size++] = x; \
    }

#endif /* _HAKE_DA_H_ */
