#ifndef MOLECULE_H
#define MOLECULE_H

#include "element.h"
#include <stdbool.h>

#define MAX_ATOMS_PER_MOLECULE 100
#define MAX_BONDS_PER_MOLECULE 150
#define MAX_FORMULA_LENGTH 256

/* Atom within a molecule */
typedef struct {
    const Element* element;     /* Pointer to element in periodic table */
    int charge;                 /* Ionic charge (0 for neutral) */
    int id;                     /* Unique ID within molecule */
} Atom;

/* Bond between two atoms */
typedef struct {
    int atom1_id;               /* First atom ID */
    int atom2_id;               /* Second atom ID */
    BondType type;              /* Single, double, or triple bond */
} Bond;

/* Molecule structure */
typedef struct {
    char name[64];              /* Common name (e.g., "Water") */
    char formula[MAX_FORMULA_LENGTH]; /* Chemical formula (e.g., "H2O") */
    Atom atoms[MAX_ATOMS_PER_MOLECULE];
    int atom_count;
    Bond bonds[MAX_BONDS_PER_MOLECULE];
    int bond_count;
    double molecular_mass;      /* Calculated from atoms */
} Molecule;

/* Element count for formula representation */
typedef struct {
    const Element* element;
    int count;
} ElementCount;

/* Parsed formula representation */
typedef struct {
    ElementCount elements[MAX_ATOMS_PER_MOLECULE];
    int element_count;
    int coefficient;            /* Leading coefficient (e.g., 2 in 2H2O) */
} Formula;

/* Molecule creation and manipulation */
void molecule_init(Molecule* mol, const char* name);
int molecule_add_atom(Molecule* mol, const Element* element, int charge);
bool molecule_add_bond(Molecule* mol, int atom1_id, int atom2_id, BondType type);
void molecule_calculate_mass(Molecule* mol);

/* Formula parsing */
bool formula_parse(const char* formula_str, Formula* result);
void formula_print(const Formula* formula);
double formula_mass(const Formula* formula);
bool formula_to_string(const Formula* formula, char* buffer, size_t buffer_size);

/* Molecule information */
void molecule_print(const Molecule* mol);
void molecule_print_composition(const Molecule* mol);

/* Create common molecules (convenience functions) */
Molecule* molecule_create_water(void);
Molecule* molecule_create_co2(void);
Molecule* molecule_create_methane(void);

/* Utility */
bool formula_equals(const Formula* f1, const Formula* f2);
void formula_simplify(Formula* formula);

#endif /* MOLECULE_H */
