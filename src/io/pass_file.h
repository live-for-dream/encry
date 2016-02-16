#ifndef PASS_FILE_INCLUDED
#define PASS_FILE_INCLUDED
#include <fcntl.h>
#include <unistd.h>
#include "types.h"
#include "hash.h"
#include "objects.h"
#include <sys/types.h>
#include <dirent.h>

/*file layout nextoff is for the same username;
total_len username_len username passwd_len passworld other_len other nextoff
...
*/
#define md5_len         33;

typedef struct record_s record_t;

struct record_s {
    object_t    obj;
    uint        total_len;
    uint        next_off;
    string_t    passwd_ciper;
    string_t    other;
};

typedef struct user_s user_t;
struct user_s {
    object_t    obj;
    string_t    user_name;
    string_t    file_name;
};

typedef struct class_s class_t;

struct class_s {
    object_t    obj;
    string_t    name;
    string_t    path;
    DIR        *dir;
};


#define init_record(record) \
    init_string((record)->passwd_ciper);\
    init_string((record)->other)\
    (record)->total_len = (record)->next_off = 0;\
    init_obj(&record->obj)
    
#define init_class(cla) \
    init_obj(&(cla)->obj);\
    init_string(&(cla)->name);\
    init_string(&(cla)->path);\
    (cla)->dir = NULL

int insert_record();
int read_record(string_t filename, string_t user_name);
int start_record_tree(string_t path);

#endif
