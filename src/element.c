#include "element.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * Periodic Table Database
 * Contains all 118 elements with key properties
 * Electronegativity: Pauling scale (0.0 = unknown/not applicable)
 * Valence electrons: Outer shell electrons for main group, varies for transition metals
 */
const Element PERIODIC_TABLE[NUM_ELEMENTS] = {
    /* Period 1 */
    {1,  "H",  "Hydrogen",     1.008,   1, 2.20, STATE_GAS,    CAT_NONMETAL,       {1, -1, 0}},
    {2,  "He", "Helium",       4.003,   2, 0.00, STATE_GAS,    CAT_NOBLE_GAS,      {0}},

    /* Period 2 */
    {3,  "Li", "Lithium",      6.941,   1, 0.98, STATE_SOLID,  CAT_ALKALI_METAL,   {1, 0}},
    {4,  "Be", "Beryllium",    9.012,   2, 1.57, STATE_SOLID,  CAT_ALKALINE_EARTH, {2, 0}},
    {5,  "B",  "Boron",       10.81,    3, 2.04, STATE_SOLID,  CAT_METALLOID,      {3, 0}},
    {6,  "C",  "Carbon",      12.011,   4, 2.55, STATE_SOLID,  CAT_NONMETAL,       {4, -4, 2, 0}},
    {7,  "N",  "Nitrogen",    14.007,   5, 3.04, STATE_GAS,    CAT_NONMETAL,       {-3, 3, 5, 0}},
    {8,  "O",  "Oxygen",      15.999,   6, 3.44, STATE_GAS,    CAT_NONMETAL,       {-2, 0}},
    {9,  "F",  "Fluorine",    18.998,   7, 3.98, STATE_GAS,    CAT_HALOGEN,        {-1, 0}},
    {10, "Ne", "Neon",        20.180,   8, 0.00, STATE_GAS,    CAT_NOBLE_GAS,      {0}},

    /* Period 3 */
    {11, "Na", "Sodium",      22.990,   1, 0.93, STATE_SOLID,  CAT_ALKALI_METAL,   {1, 0}},
    {12, "Mg", "Magnesium",   24.305,   2, 1.31, STATE_SOLID,  CAT_ALKALINE_EARTH, {2, 0}},
    {13, "Al", "Aluminum",    26.982,   3, 1.61, STATE_SOLID,  CAT_POST_TRANSITION,{3, 0}},
    {14, "Si", "Silicon",     28.086,   4, 1.90, STATE_SOLID,  CAT_METALLOID,      {4, -4, 0}},
    {15, "P",  "Phosphorus",  30.974,   5, 2.19, STATE_SOLID,  CAT_NONMETAL,       {-3, 3, 5, 0}},
    {16, "S",  "Sulfur",      32.065,   6, 2.58, STATE_SOLID,  CAT_NONMETAL,       {-2, 2, 4, 6}},
    {17, "Cl", "Chlorine",    35.453,   7, 3.16, STATE_GAS,    CAT_HALOGEN,        {-1, 1, 3, 5}},
    {18, "Ar", "Argon",       39.948,   8, 0.00, STATE_GAS,    CAT_NOBLE_GAS,      {0}},

    /* Period 4 */
    {19, "K",  "Potassium",   39.098,   1, 0.82, STATE_SOLID,  CAT_ALKALI_METAL,   {1, 0}},
    {20, "Ca", "Calcium",     40.078,   2, 1.00, STATE_SOLID,  CAT_ALKALINE_EARTH, {2, 0}},
    {21, "Sc", "Scandium",    44.956,   2, 1.36, STATE_SOLID,  CAT_TRANSITION_METAL,{3, 0}},
    {22, "Ti", "Titanium",    47.867,   2, 1.54, STATE_SOLID,  CAT_TRANSITION_METAL,{4, 3, 2, 0}},
    {23, "V",  "Vanadium",    50.942,   2, 1.63, STATE_SOLID,  CAT_TRANSITION_METAL,{5, 4, 3, 2}},
    {24, "Cr", "Chromium",    51.996,   1, 1.66, STATE_SOLID,  CAT_TRANSITION_METAL,{3, 6, 2, 0}},
    {25, "Mn", "Manganese",   54.938,   2, 1.55, STATE_SOLID,  CAT_TRANSITION_METAL,{2, 4, 7, 0}},
    {26, "Fe", "Iron",        55.845,   2, 1.83, STATE_SOLID,  CAT_TRANSITION_METAL,{2, 3, 0}},
    {27, "Co", "Cobalt",      58.933,   2, 1.88, STATE_SOLID,  CAT_TRANSITION_METAL,{2, 3, 0}},
    {28, "Ni", "Nickel",      58.693,   2, 1.91, STATE_SOLID,  CAT_TRANSITION_METAL,{2, 3, 0}},
    {29, "Cu", "Copper",      63.546,   1, 1.90, STATE_SOLID,  CAT_TRANSITION_METAL,{2, 1, 0}},
    {30, "Zn", "Zinc",        65.38,    2, 1.65, STATE_SOLID,  CAT_TRANSITION_METAL,{2, 0}},
    {31, "Ga", "Gallium",     69.723,   3, 1.81, STATE_SOLID,  CAT_POST_TRANSITION,{3, 0}},
    {32, "Ge", "Germanium",   72.64,    4, 2.01, STATE_SOLID,  CAT_METALLOID,      {4, 2, 0}},
    {33, "As", "Arsenic",     74.922,   5, 2.18, STATE_SOLID,  CAT_METALLOID,      {-3, 3, 5, 0}},
    {34, "Se", "Selenium",    78.96,    6, 2.55, STATE_SOLID,  CAT_NONMETAL,       {-2, 4, 6, 0}},
    {35, "Br", "Bromine",     79.904,   7, 2.96, STATE_LIQUID, CAT_HALOGEN,        {-1, 1, 5, 0}},
    {36, "Kr", "Krypton",     83.798,   8, 3.00, STATE_GAS,    CAT_NOBLE_GAS,      {0}},

    /* Period 5 */
    {37, "Rb", "Rubidium",    85.468,   1, 0.82, STATE_SOLID,  CAT_ALKALI_METAL,   {1, 0}},
    {38, "Sr", "Strontium",   87.62,    2, 0.95, STATE_SOLID,  CAT_ALKALINE_EARTH, {2, 0}},
    {39, "Y",  "Yttrium",     88.906,   2, 1.22, STATE_SOLID,  CAT_TRANSITION_METAL,{3, 0}},
    {40, "Zr", "Zirconium",   91.224,   2, 1.33, STATE_SOLID,  CAT_TRANSITION_METAL,{4, 0}},
    {41, "Nb", "Niobium",     92.906,   1, 1.60, STATE_SOLID,  CAT_TRANSITION_METAL,{5, 3, 0}},
    {42, "Mo", "Molybdenum",  95.96,    1, 2.16, STATE_SOLID,  CAT_TRANSITION_METAL,{6, 4, 0}},
    {43, "Tc", "Technetium",  98.0,     2, 1.90, STATE_SOLID,  CAT_TRANSITION_METAL,{7, 4, 0}},
    {44, "Ru", "Ruthenium",  101.07,    1, 2.20, STATE_SOLID,  CAT_TRANSITION_METAL,{3, 4, 0}},
    {45, "Rh", "Rhodium",    102.906,   1, 2.28, STATE_SOLID,  CAT_TRANSITION_METAL,{3, 0}},
    {46, "Pd", "Palladium",  106.42,    0, 2.20, STATE_SOLID,  CAT_TRANSITION_METAL,{2, 4, 0}},
    {47, "Ag", "Silver",     107.868,   1, 1.93, STATE_SOLID,  CAT_TRANSITION_METAL,{1, 0}},
    {48, "Cd", "Cadmium",    112.411,   2, 1.69, STATE_SOLID,  CAT_TRANSITION_METAL,{2, 0}},
    {49, "In", "Indium",     114.818,   3, 1.78, STATE_SOLID,  CAT_POST_TRANSITION,{3, 0}},
    {50, "Sn", "Tin",        118.710,   4, 1.96, STATE_SOLID,  CAT_POST_TRANSITION,{4, 2, 0}},
    {51, "Sb", "Antimony",   121.760,   5, 2.05, STATE_SOLID,  CAT_METALLOID,      {-3, 3, 5, 0}},
    {52, "Te", "Tellurium",  127.60,    6, 2.10, STATE_SOLID,  CAT_METALLOID,      {-2, 4, 6, 0}},
    {53, "I",  "Iodine",     126.904,   7, 2.66, STATE_SOLID,  CAT_HALOGEN,        {-1, 1, 5, 7}},
    {54, "Xe", "Xenon",      131.293,   8, 2.60, STATE_GAS,    CAT_NOBLE_GAS,      {0}},

    /* Period 6 */
    {55, "Cs", "Cesium",     132.905,   1, 0.79, STATE_SOLID,  CAT_ALKALI_METAL,   {1, 0}},
    {56, "Ba", "Barium",     137.327,   2, 0.89, STATE_SOLID,  CAT_ALKALINE_EARTH, {2, 0}},
    {57, "La", "Lanthanum",  138.905,   2, 1.10, STATE_SOLID,  CAT_LANTHANIDE,     {3, 0}},
    {58, "Ce", "Cerium",     140.116,   2, 1.12, STATE_SOLID,  CAT_LANTHANIDE,     {3, 4, 0}},
    {59, "Pr", "Praseodymium",140.908,  2, 1.13, STATE_SOLID,  CAT_LANTHANIDE,     {3, 0}},
    {60, "Nd", "Neodymium",  144.242,   2, 1.14, STATE_SOLID,  CAT_LANTHANIDE,     {3, 0}},
    {61, "Pm", "Promethium", 145.0,     2, 1.13, STATE_SOLID,  CAT_LANTHANIDE,     {3, 0}},
    {62, "Sm", "Samarium",   150.36,    2, 1.17, STATE_SOLID,  CAT_LANTHANIDE,     {3, 2, 0}},
    {63, "Eu", "Europium",   151.964,   2, 1.20, STATE_SOLID,  CAT_LANTHANIDE,     {3, 2, 0}},
    {64, "Gd", "Gadolinium", 157.25,    2, 1.20, STATE_SOLID,  CAT_LANTHANIDE,     {3, 0}},
    {65, "Tb", "Terbium",    158.925,   2, 1.20, STATE_SOLID,  CAT_LANTHANIDE,     {3, 0}},
    {66, "Dy", "Dysprosium", 162.500,   2, 1.22, STATE_SOLID,  CAT_LANTHANIDE,     {3, 0}},
    {67, "Ho", "Holmium",    164.930,   2, 1.23, STATE_SOLID,  CAT_LANTHANIDE,     {3, 0}},
    {68, "Er", "Erbium",     167.259,   2, 1.24, STATE_SOLID,  CAT_LANTHANIDE,     {3, 0}},
    {69, "Tm", "Thulium",    168.934,   2, 1.25, STATE_SOLID,  CAT_LANTHANIDE,     {3, 2, 0}},
    {70, "Yb", "Ytterbium",  173.054,   2, 1.10, STATE_SOLID,  CAT_LANTHANIDE,     {3, 2, 0}},
    {71, "Lu", "Lutetium",   174.967,   2, 1.27, STATE_SOLID,  CAT_LANTHANIDE,     {3, 0}},
    {72, "Hf", "Hafnium",    178.49,    2, 1.30, STATE_SOLID,  CAT_TRANSITION_METAL,{4, 0}},
    {73, "Ta", "Tantalum",   180.948,   2, 1.50, STATE_SOLID,  CAT_TRANSITION_METAL,{5, 0}},
    {74, "W",  "Tungsten",   183.84,    2, 2.36, STATE_SOLID,  CAT_TRANSITION_METAL,{6, 4, 0}},
    {75, "Re", "Rhenium",    186.207,   2, 1.90, STATE_SOLID,  CAT_TRANSITION_METAL,{7, 4, 0}},
    {76, "Os", "Osmium",     190.23,    2, 2.20, STATE_SOLID,  CAT_TRANSITION_METAL,{4, 3, 0}},
    {77, "Ir", "Iridium",    192.217,   2, 2.20, STATE_SOLID,  CAT_TRANSITION_METAL,{4, 3, 0}},
    {78, "Pt", "Platinum",   195.084,   1, 2.28, STATE_SOLID,  CAT_TRANSITION_METAL,{2, 4, 0}},
    {79, "Au", "Gold",       196.967,   1, 2.54, STATE_SOLID,  CAT_TRANSITION_METAL,{3, 1, 0}},
    {80, "Hg", "Mercury",    200.59,    2, 2.00, STATE_LIQUID, CAT_TRANSITION_METAL,{2, 1, 0}},
    {81, "Tl", "Thallium",   204.383,   3, 1.62, STATE_SOLID,  CAT_POST_TRANSITION,{1, 3, 0}},
    {82, "Pb", "Lead",       207.2,     4, 2.33, STATE_SOLID,  CAT_POST_TRANSITION,{2, 4, 0}},
    {83, "Bi", "Bismuth",    208.980,   5, 2.02, STATE_SOLID,  CAT_POST_TRANSITION,{3, 5, 0}},
    {84, "Po", "Polonium",   209.0,     6, 2.00, STATE_SOLID,  CAT_METALLOID,      {4, 2, 0}},
    {85, "At", "Astatine",   210.0,     7, 2.20, STATE_SOLID,  CAT_HALOGEN,        {-1, 1, 0}},
    {86, "Rn", "Radon",      222.0,     8, 0.00, STATE_GAS,    CAT_NOBLE_GAS,      {0}},

    /* Period 7 */
    {87, "Fr", "Francium",   223.0,     1, 0.70, STATE_SOLID,  CAT_ALKALI_METAL,   {1, 0}},
    {88, "Ra", "Radium",     226.0,     2, 0.90, STATE_SOLID,  CAT_ALKALINE_EARTH, {2, 0}},
    {89, "Ac", "Actinium",   227.0,     2, 1.10, STATE_SOLID,  CAT_ACTINIDE,       {3, 0}},
    {90, "Th", "Thorium",    232.038,   2, 1.30, STATE_SOLID,  CAT_ACTINIDE,       {4, 0}},
    {91, "Pa", "Protactinium",231.036,  2, 1.50, STATE_SOLID,  CAT_ACTINIDE,       {5, 4, 0}},
    {92, "U",  "Uranium",    238.029,   2, 1.38, STATE_SOLID,  CAT_ACTINIDE,       {6, 4, 3, 0}},
    {93, "Np", "Neptunium",  237.0,     2, 1.36, STATE_SOLID,  CAT_ACTINIDE,       {5, 4, 3, 0}},
    {94, "Pu", "Plutonium",  244.0,     2, 1.28, STATE_SOLID,  CAT_ACTINIDE,       {4, 3, 5, 6}},
    {95, "Am", "Americium",  243.0,     2, 1.30, STATE_SOLID,  CAT_ACTINIDE,       {3, 4, 5, 6}},
    {96, "Cm", "Curium",     247.0,     2, 1.30, STATE_SOLID,  CAT_ACTINIDE,       {3, 0}},
    {97, "Bk", "Berkelium",  247.0,     2, 1.30, STATE_SOLID,  CAT_ACTINIDE,       {3, 4, 0}},
    {98, "Cf", "Californium",251.0,     2, 1.30, STATE_SOLID,  CAT_ACTINIDE,       {3, 0}},
    {99, "Es", "Einsteinium",252.0,     2, 1.30, STATE_SOLID,  CAT_ACTINIDE,       {3, 0}},
    {100,"Fm", "Fermium",    257.0,     2, 1.30, STATE_SOLID,  CAT_ACTINIDE,       {3, 0}},
    {101,"Md", "Mendelevium",258.0,     2, 1.30, STATE_SOLID,  CAT_ACTINIDE,       {3, 2, 0}},
    {102,"No", "Nobelium",   259.0,     2, 1.30, STATE_SOLID,  CAT_ACTINIDE,       {2, 3, 0}},
    {103,"Lr", "Lawrencium", 262.0,     3, 1.30, STATE_SOLID,  CAT_ACTINIDE,       {3, 0}},
    {104,"Rf", "Rutherfordium",267.0,   2, 0.00, STATE_UNKNOWN,CAT_TRANSITION_METAL,{4, 0}},
    {105,"Db", "Dubnium",    268.0,     2, 0.00, STATE_UNKNOWN,CAT_TRANSITION_METAL,{5, 0}},
    {106,"Sg", "Seaborgium", 271.0,     2, 0.00, STATE_UNKNOWN,CAT_TRANSITION_METAL,{6, 0}},
    {107,"Bh", "Bohrium",    270.0,     2, 0.00, STATE_UNKNOWN,CAT_TRANSITION_METAL,{7, 0}},
    {108,"Hs", "Hassium",    277.0,     2, 0.00, STATE_UNKNOWN,CAT_TRANSITION_METAL,{8, 0}},
    {109,"Mt", "Meitnerium", 276.0,     2, 0.00, STATE_UNKNOWN,CAT_TRANSITION_METAL,{0}},
    {110,"Ds", "Darmstadtium",281.0,    2, 0.00, STATE_UNKNOWN,CAT_TRANSITION_METAL,{0}},
    {111,"Rg", "Roentgenium",280.0,     2, 0.00, STATE_UNKNOWN,CAT_TRANSITION_METAL,{0}},
    {112,"Cn", "Copernicium",285.0,     2, 0.00, STATE_UNKNOWN,CAT_TRANSITION_METAL,{2, 0}},
    {113,"Nh", "Nihonium",   284.0,     3, 0.00, STATE_UNKNOWN,CAT_POST_TRANSITION,{0}},
    {114,"Fl", "Flerovium",  289.0,     4, 0.00, STATE_UNKNOWN,CAT_POST_TRANSITION,{0}},
    {115,"Mc", "Moscovium",  288.0,     5, 0.00, STATE_UNKNOWN,CAT_POST_TRANSITION,{0}},
    {116,"Lv", "Livermorium",293.0,     6, 0.00, STATE_UNKNOWN,CAT_POST_TRANSITION,{0}},
    {117,"Ts", "Tennessine", 294.0,     7, 0.00, STATE_UNKNOWN,CAT_HALOGEN,        {0}},
    {118,"Og", "Oganesson",  294.0,     8, 0.00, STATE_UNKNOWN,CAT_NOBLE_GAS,      {0}}
};

/* Lookup element by atomic number */
const Element* element_by_number(int atomic_number) {
    if (atomic_number < 1 || atomic_number > NUM_ELEMENTS) {
        return NULL;
    }
    return &PERIODIC_TABLE[atomic_number - 1];
}

/* Lookup element by symbol (case-insensitive) */
const Element* element_by_symbol(const char* symbol) {
    if (!symbol) return NULL;

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        /* Case-insensitive comparison */
        const char* s1 = PERIODIC_TABLE[i].symbol;
        const char* s2 = symbol;
        int match = 1;

        while (*s1 && *s2) {
            if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
                match = 0;
                break;
            }
            s1++;
            s2++;
        }

        if (match && *s1 == '\0' && *s2 == '\0') {
            return &PERIODIC_TABLE[i];
        }
    }
    return NULL;
}

/* Lookup element by name (case-insensitive) */
const Element* element_by_name(const char* name) {
    if (!name) return NULL;

    for (int i = 0; i < NUM_ELEMENTS; i++) {
        const char* s1 = PERIODIC_TABLE[i].name;
        const char* s2 = name;
        int match = 1;

        while (*s1 && *s2) {
            if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
                match = 0;
                break;
            }
            s1++;
            s2++;
        }

        if (match && *s1 == '\0' && *s2 == '\0') {
            return &PERIODIC_TABLE[i];
        }
    }
    return NULL;
}

/* Convert state enum to string */
const char* element_state_str(ElementState state) {
    switch (state) {
        case STATE_SOLID:   return "Solid";
        case STATE_LIQUID:  return "Liquid";
        case STATE_GAS:     return "Gas";
        default:            return "Unknown";
    }
}

/* Convert category enum to string */
const char* element_category_str(ElementCategory cat) {
    switch (cat) {
        case CAT_NONMETAL:          return "Nonmetal";
        case CAT_NOBLE_GAS:         return "Noble Gas";
        case CAT_ALKALI_METAL:      return "Alkali Metal";
        case CAT_ALKALINE_EARTH:    return "Alkaline Earth Metal";
        case CAT_METALLOID:         return "Metalloid";
        case CAT_HALOGEN:           return "Halogen";
        case CAT_TRANSITION_METAL:  return "Transition Metal";
        case CAT_POST_TRANSITION:   return "Post-Transition Metal";
        case CAT_LANTHANIDE:        return "Lanthanide";
        case CAT_ACTINIDE:          return "Actinide";
        default:                    return "Unknown";
    }
}

/* Print element information */
void element_print(const Element* el) {
    if (!el) {
        printf("(null element)\n");
        return;
    }

    printf("%-2s %-12s (Z=%3d)\n", el->symbol, el->name, el->atomic_number);
    printf("   Mass: %.3f amu\n", el->atomic_mass);
    printf("   Valence electrons: %d\n", el->valence_electrons);
    printf("   Electronegativity: %.2f\n", el->electronegativity);
    printf("   State (room temp): %s\n", element_state_str(el->state));
    printf("   Category: %s\n", element_category_str(el->category));
    printf("   Common charges: ");
    for (int i = 0; i < 4 && el->common_charges[i] != 0; i++) {
        if (i > 0) printf(", ");
        printf("%+d", el->common_charges[i]);
    }
    if (el->common_charges[0] == 0) printf("0");
    printf("\n");
}

/* Calculate maximum bonds an element can typically form */
int element_max_bonds(const Element* el) {
    if (!el) return 0;

    /* Noble gases generally don't form bonds */
    if (el->category == CAT_NOBLE_GAS) return 0;

    /* For main group elements, use valence electrons */
    int valence = el->valence_electrons;

    /* Elements can form bonds to complete octet (8 electrons) */
    /* Either by sharing valence electrons or by accepting electrons */
    if (valence <= 4) {
        return valence;  /* Can share all valence electrons */
    } else {
        return 8 - valence;  /* Can accept electrons to complete octet */
    }
}
