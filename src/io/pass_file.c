#include "pass_file.h"
#include <linux/limits.h>
#include <sys/types.h>
#include <dirent.h>

class_t  root;
static obj_attr_t cla_attr = {
    .build = class_build,
};

static obj_attr_t user_attr = {
    .build = user_build
};


int init_root(string_t *path) {
    char                ab_path[PATH_MAX + 1];
    if (!path) {
        getcwd(ab_path, sizeof(ab_path));
        if (errno) {
            return ERR;
        }
    }

    
}

int start_record_tree(string_t *path) {
    char                ab_path[PATH_MAX + 1];
    int                 p_len;
    DIR                *dir;
    struct dirent      *p_dirent;

    if (!path || !path->len) {
        getcwd(ab_path, sizeof(ab_path));
        if (errno) {
            return ERR;
        }
    } else {
        sprintf(ab_path, "%s", (char *)path->str);
        ab_path[path->len] = '\0';
    }

    p_len = strlen(ab_path);
    init_class(&root);
    root.path.len = p_len;
    root.path.str = malloc(sizeof(uchar) *p_len + 1);
    if (!root.path.str) {
        return ERR_NOMEM;
    }
    sprintf((char *)root.path.str, "%s", ab_path);
    root.path.str[p_len] = '\0';
    
    dir = opendir(ab_path);
    if (!dir) {
        return ERR;
    }

    root.dir = dir;
    while((p_dirent=readdir(dir))) {
        if (p_dirent->d_type & DT_REG) {
            
        } else if (p_dirent->d_type & DT_DIR) {
            
        }
        printf("%s\n",p_dirent->d_name);
    }
    
}

static void get_class_path(object_t *obj, string_t *path) {
    class_t *cla; 
    cla = obj_entry(obj, class_t, obj);
    if (!obj->parent) {
        sprintf((char *)path->str, "%s", (char *)cla->path.str);
        path->len += cla->path.len;
        return;
    }

    get_class_path(obj->parent, path);
    sprintf((char *)path->str + path->len, "/%s", (char *)cla->path.str);
    path->len += 1 + cla->path.len;
    return;
}

int class_build(object_t *obj) {
    class_t            *cla;
    class_t            *new_cla;
    user_t             *new_user;
    string_t            path;
    char                ab_path[PATH_MAX + 1];
    DIR                *dir;
    struct dirent      *p_dirent;
    string_t            hex_name;
    
    
    path.str = (uchar)ab_path;
    path.len = 0;
    cla = obj_entry(obj, class_t, obj);
    get_class_path(obj, &path);

    dir = opendir(ab_path);
    if (!dir) {
        return ERR;
    }

    cla->dir = dir;
    while((p_dirent=readdir(dir))) {
        if (p_dirent->d_type & DT_DIR) {
            if (!strcmp(p_dirent->d_name, ".") || !strcmp(p_dirent->d_name, ".")) {
                continue;
            }

            new_cla = malloc(sizeof(class_t));
            if (!new_cla) {
                return ERR_NOMEM;
            }

            new_cla->path.str = malloc(strlen(p_dirent->d_name) * sizeof(uchar) + 1);
            if (!new_cla->path.str) {
                return ERR_NOMEM;
            }

            sprintf(new_cla->path.str, "%s", p_dirent->d_name);
            init_obj(&new_cla->obj);
            add_obj(obj, &new_cla->obj);
            new_cla->obj.options = &cla_attr;
            new_cla->obj.options->build(&new_cla->obj);
        } else if (p_dirent->d_type & DT_REG) {
            new_user = malloc(sizeof(user_t));
            if (!new_user) {
                return ERR_NOMEM;
            }
            
            hex_name.len = strlen(p_dirent->d_name);
            hex_name.str = (uchar *)p_dirent->d_name;
            hex_to_string(&new_user->user_name, &hex_name);
            new_user->file_name.str = malloc(hex_name.len + 1);
            new_user->file_name.len = hex_name.len;
            sprintf(new_user->file_name.str, "%s", p_dirent->d_name);
            
            init_obj(&new_user->obj);
            add_obj(obj, &new_user->obj);
            new_user->obj.options = &user_attr;
            //new_user->obj.options->build(&new_user->obj);
        } else {
            continue;
        }
    }

    return OK;
}


int user_build(object_t *obj) {
    user_t             *usr;
    char                ab_path[PATH_MAX + 1];
    string_t            path;
    int                 fd;

    path.len = 0;
    path.str = ab_path;
    get_class_path(obj->parent, &path);
    usr = obj_entry(obj, user_t, obj);

    sprintf(path.str + path.len, "/%s", usr->file_name.str);
    path.len += usr->file_name.len + 1;

    fd = open(path.str, O_RDWR);
    if (fd < 0) {
        return ERR;
    }

    
}

