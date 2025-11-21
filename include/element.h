#ifndef ELEMENT_H
#define ELEMENT_H

#include <stddef.h>

/* Bond types */
typedef enum {
    BOND_NONE = 0,
    BOND_SINGLE = 1,
    BOND_DOUBLE = 2,
    BOND_TRIPLE = 3
} BondType;

/* Element state at room temperature */
typedef enum {
    STATE_SOLID,
    STATE_LIQUID,
    STATE_GAS,
    STATE_UNKNOWN
} ElementState;

/* Element category */
typedef enum {
    CAT_NONMETAL,
    CAT_NOBLE_GAS,
    CAT_ALKALI_METAL,
    CAT_ALKALINE_EARTH,
    CAT_METALLOID,
    CAT_HALOGEN,
    CAT_TRANSITION_METAL,
    CAT_POST_TRANSITION,
    CAT_LANTHANIDE,
    CAT_ACTINIDE,
    CAT_UNKNOWN
} ElementCategory;

/* Element structure containing key properties */
typedef struct {
    int atomic_number;          /* Z - number of protons */
    char symbol[4];             /* Element symbol (e.g., "H", "He", "Na") */
    char name[20];              /* Full name */
    double atomic_mass;         /* Atomic mass in amu */
    int valence_electrons;      /* Electrons in outer shell */
    double electronegativity;   /* Pauling scale (0 if unknown) */
    ElementState state;         /* State at room temperature */
    ElementCategory category;   /* Element category */
    int common_charges[4];      /* Common ionic charges (0-terminated) */
} Element;

/* Number of elements in our database */
#define NUM_ELEMENTS 118

/* Global periodic table array */
extern const Element PERIODIC_TABLE[NUM_ELEMENTS];

/* Lookup functions */
const Element* element_by_number(int atomic_number);
const Element* element_by_symbol(const char* symbol);
const Element* element_by_name(const char* name);

/* Utility functions */
const char* element_state_str(ElementState state);
const char* element_category_str(ElementCategory cat);
void element_print(const Element* el);

/* Calculate max bonds an element can typically form */
int element_max_bonds(const Element* el);

#endif /* ELEMENT_H */
