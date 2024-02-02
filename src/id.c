#include <hake/util/id.h>
#include <uuid/uuid.h>

void hake_Id_gen(hake_Id id) {
    uuid_t uu;
    uuid_generate(uu);
    uuid_unparse_lower(uu, id);
}
