#include <fcntl.h>
#include <unistd.h>
#include "types.h"

/*file layout
    

*/
#define md5_len         33;

typedef struct class_s {
    uchar       file_name;
    hash_t      hash;
} class_t;

typedef struct record_s {
    string_t    passwd_ciper;
    string_t    user_md5;
    string_t    user_name_cipher;
    string_t    other_cipher;
    string_t    other_md5;
    int         cipher_type;
} record_t;


#define init_record(record) \
    init_string((record)->passwd_ciper);\
    init_string((record)->user_md5);\
    init_string((record)->user_name_cipher);\
    init_string((record)->other_cipher)
int insert_record()