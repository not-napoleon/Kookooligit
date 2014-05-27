#ifndef CREATURE_H_INCLUDED
#define CREATURE_H_INCLUDED



struct CreatureType;

struct Creature {
  struct CreatureType *type;
};

void init_creature_types();


#endif
