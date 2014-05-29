#ifndef CREATURE_TYPE_H_INCLUDED
#define CREATURE_TYPE_H_INCLUDED

struct CreatureType;

void init_creature_types();
struct CreatureType *get_creature_type_by_id(const char *id);

#endif
