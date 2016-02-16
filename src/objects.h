#ifndef OBJECTS_INCLUDED
#define OBJECTS_INCLUDED

#include "lists.h"

typedef struct object_s object_t;
typedef struct obj_attr_s obj_attr_t;
typedef int (*obj_op)(object_t *obj);

struct obj_attr_s {
    obj_op  create;
    obj_op  show;
    obj_op  del;
    obj_op  build;
};


struct object_s {
    object_t            *parent;
    list_head_t          sibling;
    list_head_t          childs;
    obj_attr_t          *options;
};
#define init_obj_attr(attr)\
    (attr)->create = NULL;\
    (attr)->show = NULL;\
    (attr)->del = NULL;\
    (attr)->build = NULL;


#define init_obj(obj) \
    (obj)->parent = NULL;\
    INIT_LIST_HEAD(&(obj)->sibling);\
    INIT_LIST_HEAD(&(obj)->childs);\
    (obj)->options = NULL

#define add_obj(parent, child) \
    (child)->parent = (parent);\
    list_add(&(child)->sibling, &(parent)->childs)

#define del_obj(child)\
    list_del(&(child)->sibling);\
    (child)->parent = NULL

#define obj_entry(obj, type, member) \
    container_of(obj, type, member)

#endif
