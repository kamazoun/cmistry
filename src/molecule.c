#include "molecule.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Initialize a molecule */
void molecule_init(Molecule* mol, const char* name) {
    if (!mol) return;

    memset(mol, 0, sizeof(Molecule));
    if (name) {
        strncpy(mol->name, name, sizeof(mol->name) - 1);
    }
}

/* Add an atom to a molecule, returns atom ID or -1 on failure */
int molecule_add_atom(Molecule* mol, const Element* element, int charge) {
    if (!mol || !element) return -1;
    if (mol->atom_count >= MAX_ATOMS_PER_MOLECULE) return -1;

    int id = mol->atom_count;
    mol->atoms[id].element = element;
    mol->atoms[id].charge = charge;
    mol->atoms[id].id = id;
    mol->atom_count++;

    return id;
}

/* Add a bond between two atoms */
bool molecule_add_bond(Molecule* mol, int atom1_id, int atom2_id, BondType type) {
    if (!mol) return false;
    if (atom1_id < 0 || atom1_id >= mol->atom_count) return false;
    if (atom2_id < 0 || atom2_id >= mol->atom_count) return false;
    if (mol->bond_count >= MAX_BONDS_PER_MOLECULE) return false;
    if (atom1_id == atom2_id) return false;

    int idx = mol->bond_count;
    mol->bonds[idx].atom1_id = atom1_id;
    mol->bonds[idx].atom2_id = atom2_id;
    mol->bonds[idx].type = type;
    mol->bond_count++;

    return true;
}

/* Calculate molecular mass from atoms */
void molecule_calculate_mass(Molecule* mol) {
    if (!mol) return;

    mol->molecular_mass = 0.0;
    for (int i = 0; i < mol->atom_count; i++) {
        if (mol->atoms[i].element) {
            mol->molecular_mass += mol->atoms[i].element->atomic_mass;
        }
    }
}

/* Parse a chemical formula string (e.g., "H2O", "2CO2", "Ca(OH)2") */
bool formula_parse(const char* formula_str, Formula* result) {
    if (!formula_str || !result) return false;

    memset(result, 0, sizeof(Formula));
    result->coefficient = 1;

    const char* p = formula_str;

    /* Skip whitespace */
    while (*p && isspace((unsigned char)*p)) p++;

    /* Parse leading coefficient (e.g., "2" in "2H2O") */
    if (isdigit((unsigned char)*p)) {
        result->coefficient = 0;
        while (isdigit((unsigned char)*p)) {
            result->coefficient = result->coefficient * 10 + (*p - '0');
            p++;
        }
    }

    /* Parse elements and their counts */
    while (*p) {
        /* Skip whitespace */
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;

        /* Element symbol starts with uppercase letter */
        if (!isupper((unsigned char)*p)) {
            /* Handle parentheses - simplified, just skip for now */
            if (*p == '(' || *p == ')') {
                p++;
                continue;
            }
            return false;
        }

        /* Parse element symbol (1-2 characters) */
        char symbol[4] = {0};
        symbol[0] = *p++;

        if (*p && islower((unsigned char)*p)) {
            symbol[1] = *p++;
        }

        /* Look up the element */
        const Element* el = element_by_symbol(symbol);
        if (!el) {
            fprintf(stderr, "Unknown element: %s\n", symbol);
            return false;
        }

        /* Parse count (default 1) */
        int count = 0;
        while (*p && isdigit((unsigned char)*p)) {
            count = count * 10 + (*p - '0');
            p++;
        }
        if (count == 0) count = 1;

        /* Add to result or update existing */
        bool found = false;
        for (int i = 0; i < result->element_count; i++) {
            if (result->elements[i].element == el) {
                result->elements[i].count += count;
                found = true;
                break;
            }
        }

        if (!found) {
            if (result->element_count >= MAX_ATOMS_PER_MOLECULE) {
                return false;
            }
            result->elements[result->element_count].element = el;
            result->elements[result->element_count].count = count;
            result->element_count++;
        }
    }

    return result->element_count > 0;
}

/* Print formula information */
void formula_print(const Formula* formula) {
    if (!formula) {
        printf("(null formula)\n");
        return;
    }

    if (formula->coefficient > 1) {
        printf("%d", formula->coefficient);
    }

    for (int i = 0; i < formula->element_count; i++) {
        printf("%s", formula->elements[i].element->symbol);
        if (formula->elements[i].count > 1) {
            printf("%d", formula->elements[i].count);
        }
    }
}

/* Calculate molecular mass from formula */
double formula_mass(const Formula* formula) {
    if (!formula) return 0.0;

    double mass = 0.0;
    for (int i = 0; i < formula->element_count; i++) {
        mass += formula->elements[i].element->atomic_mass * formula->elements[i].count;
    }

    return mass * formula->coefficient;
}

/* Convert formula to string */
bool formula_to_string(const Formula* formula, char* buffer, size_t buffer_size) {
    if (!formula || !buffer || buffer_size == 0) return false;

    char* p = buffer;
    size_t remaining = buffer_size;

    if (formula->coefficient > 1) {
        int written = snprintf(p, remaining, "%d", formula->coefficient);
        if (written < 0 || (size_t)written >= remaining) return false;
        p += written;
        remaining -= written;
    }

    for (int i = 0; i < formula->element_count; i++) {
        int written;
        if (formula->elements[i].count > 1) {
            written = snprintf(p, remaining, "%s%d",
                             formula->elements[i].element->symbol,
                             formula->elements[i].count);
        } else {
            written = snprintf(p, remaining, "%s",
                             formula->elements[i].element->symbol);
        }
        if (written < 0 || (size_t)written >= remaining) return false;
        p += written;
        remaining -= written;
    }

    return true;
}

/* Print molecule information */
void molecule_print(const Molecule* mol) {
    if (!mol) {
        printf("(null molecule)\n");
        return;
    }

    printf("Molecule: %s\n", mol->name[0] ? mol->name : "(unnamed)");
    printf("Formula: %s\n", mol->formula[0] ? mol->formula : "(none)");
    printf("Atoms: %d\n", mol->atom_count);
    printf("Bonds: %d\n", mol->bond_count);
    printf("Molecular mass: %.3f g/mol\n", mol->molecular_mass);
}

/* Print molecule composition */
void molecule_print_composition(const Molecule* mol) {
    if (!mol) return;

    printf("Composition of %s:\n", mol->name[0] ? mol->name : mol->formula);

    /* Count each element */
    typedef struct { const Element* el; int count; } ECount;
    ECount counts[MAX_ATOMS_PER_MOLECULE];
    int num_elements = 0;

    for (int i = 0; i < mol->atom_count; i++) {
        const Element* el = mol->atoms[i].element;
        bool found = false;

        for (int j = 0; j < num_elements; j++) {
            if (counts[j].el == el) {
                counts[j].count++;
                found = true;
                break;
            }
        }

        if (!found && num_elements < MAX_ATOMS_PER_MOLECULE) {
            counts[num_elements].el = el;
            counts[num_elements].count = 1;
            num_elements++;
        }
    }

    for (int i = 0; i < num_elements; i++) {
        double percent = (counts[i].el->atomic_mass * counts[i].count / mol->molecular_mass) * 100;
        printf("  %s: %d atom(s), %.2f%% by mass\n",
               counts[i].el->name, counts[i].count, percent);
    }
}

/* Check if two formulas are equal (same elements and counts) */
bool formula_equals(const Formula* f1, const Formula* f2) {
    if (!f1 || !f2) return false;
    if (f1->element_count != f2->element_count) return false;

    /* Check each element in f1 exists in f2 with same count */
    for (int i = 0; i < f1->element_count; i++) {
        bool found = false;
        for (int j = 0; j < f2->element_count; j++) {
            if (f1->elements[i].element == f2->elements[j].element) {
                if (f1->elements[i].count != f2->elements[j].count) {
                    return false;
                }
                found = true;
                break;
            }
        }
        if (!found) return false;
    }

    return true;
}

/* Create water molecule (H2O) */
Molecule* molecule_create_water(void) {
    static Molecule water;
    molecule_init(&water, "Water");
    strcpy(water.formula, "H2O");

    const Element* H = element_by_symbol("H");
    const Element* O = element_by_symbol("O");

    int o = molecule_add_atom(&water, O, 0);
    int h1 = molecule_add_atom(&water, H, 0);
    int h2 = molecule_add_atom(&water, H, 0);

    molecule_add_bond(&water, o, h1, BOND_SINGLE);
    molecule_add_bond(&water, o, h2, BOND_SINGLE);

    molecule_calculate_mass(&water);
    return &water;
}

/* Create carbon dioxide molecule (CO2) */
Molecule* molecule_create_co2(void) {
    static Molecule co2;
    molecule_init(&co2, "Carbon Dioxide");
    strcpy(co2.formula, "CO2");

    const Element* C = element_by_symbol("C");
    const Element* O = element_by_symbol("O");

    int c = molecule_add_atom(&co2, C, 0);
    int o1 = molecule_add_atom(&co2, O, 0);
    int o2 = molecule_add_atom(&co2, O, 0);

    molecule_add_bond(&co2, c, o1, BOND_DOUBLE);
    molecule_add_bond(&co2, c, o2, BOND_DOUBLE);

    molecule_calculate_mass(&co2);
    return &co2;
}

/* Create methane molecule (CH4) */
Molecule* molecule_create_methane(void) {
    static Molecule methane;
    molecule_init(&methane, "Methane");
    strcpy(methane.formula, "CH4");

    const Element* C = element_by_symbol("C");
    const Element* H = element_by_symbol("H");

    int c = molecule_add_atom(&methane, C, 0);
    int h1 = molecule_add_atom(&methane, H, 0);
    int h2 = molecule_add_atom(&methane, H, 0);
    int h3 = molecule_add_atom(&methane, H, 0);
    int h4 = molecule_add_atom(&methane, H, 0);

    molecule_add_bond(&methane, c, h1, BOND_SINGLE);
    molecule_add_bond(&methane, c, h2, BOND_SINGLE);
    molecule_add_bond(&methane, c, h3, BOND_SINGLE);
    molecule_add_bond(&methane, c, h4, BOND_SINGLE);

    molecule_calculate_mass(&methane);
    return &methane;
}
