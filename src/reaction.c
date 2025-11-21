#include "reaction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ============ Reaction Database Storage ============ */
static Reaction reaction_database[MAX_REACTIONS];
static int reaction_db_size = 0;
static bool reaction_db_initialized = false;

/* ============ Reaction Initialization ============ */

void reaction_init(Reaction* rxn) {
    if (!rxn) return;
    memset(rxn, 0, sizeof(Reaction));
    rxn->condition = COND_NORMAL;
    rxn->type = RXTYPE_OTHER;
}

bool reaction_add_reactant(Reaction* rxn, const char* formula) {
    if (!rxn || !formula) return false;
    if (rxn->reactant_count >= MAX_REACTANTS) return false;

    if (!formula_parse(formula, &rxn->reactants[rxn->reactant_count])) {
        return false;
    }
    rxn->reactant_count++;
    return true;
}

bool reaction_add_product(Reaction* rxn, const char* formula) {
    if (!rxn || !formula) return false;
    if (rxn->product_count >= MAX_PRODUCTS) return false;

    if (!formula_parse(formula, &rxn->products[rxn->product_count])) {
        return false;
    }
    rxn->product_count++;
    return true;
}

void reaction_set_condition(Reaction* rxn, ReactionCondition cond) {
    if (rxn) rxn->condition = cond;
}

void reaction_set_type(Reaction* rxn, ReactionType type) {
    if (rxn) rxn->type = type;
}

void reaction_set_description(Reaction* rxn, const char* desc) {
    if (rxn && desc) {
        strncpy(rxn->description, desc, sizeof(rxn->description) - 1);
    }
}

/* ============ String Conversions ============ */

const char* reaction_condition_str(ReactionCondition cond) {
    switch (cond) {
        case COND_NORMAL:       return "Normal (STP)";
        case COND_HEATED:       return "Heated";
        case COND_HIGH_PRESSURE:return "High Pressure";
        case COND_CATALYST:     return "Catalyst Required";
        case COND_LIGHT:        return "Light Required";
        case COND_ELECTROLYSIS: return "Electrolysis";
        default:                return "Unknown";
    }
}

const char* reaction_type_str(ReactionType type) {
    switch (type) {
        case RXTYPE_SYNTHESIS:      return "Synthesis";
        case RXTYPE_DECOMPOSITION:  return "Decomposition";
        case RXTYPE_SINGLE_REPLACE: return "Single Replacement";
        case RXTYPE_DOUBLE_REPLACE: return "Double Replacement";
        case RXTYPE_COMBUSTION:     return "Combustion";
        case RXTYPE_ACID_BASE:      return "Acid-Base";
        case RXTYPE_REDOX:          return "Redox";
        default:                    return "Other";
    }
}

/* ============ Reaction Balancing Check ============ */

/* Count total atoms of each element on one side */
static void count_atoms(const Formula* formulas, int count, int* atom_counts) {
    /* atom_counts should be zeroed and have NUM_ELEMENTS entries */
    for (int i = 0; i < count; i++) {
        int coef = formulas[i].coefficient > 0 ? formulas[i].coefficient : 1;
        for (int j = 0; j < formulas[i].element_count; j++) {
            int atomic_num = formulas[i].elements[j].element->atomic_number;
            atom_counts[atomic_num - 1] += formulas[i].elements[j].count * coef;
        }
    }
}

bool reaction_check_balanced(Reaction* rxn) {
    if (!rxn) return false;

    int reactant_atoms[NUM_ELEMENTS] = {0};
    int product_atoms[NUM_ELEMENTS] = {0};

    count_atoms(rxn->reactants, rxn->reactant_count, reactant_atoms);
    count_atoms(rxn->products, rxn->product_count, product_atoms);

    /* Compare counts */
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        if (reactant_atoms[i] != product_atoms[i]) {
            rxn->is_balanced = false;
            return false;
        }
    }

    rxn->is_balanced = true;
    return true;
}

/* ============ Reaction Printing ============ */

void reaction_print(const Reaction* rxn) {
    if (!rxn) {
        printf("(null reaction)\n");
        return;
    }

    /* Print reactants */
    for (int i = 0; i < rxn->reactant_count; i++) {
        if (i > 0) printf(" + ");
        formula_print(&rxn->reactants[i]);
    }

    /* Arrow */
    printf(" -> ");

    /* Print products */
    for (int i = 0; i < rxn->product_count; i++) {
        if (i > 0) printf(" + ");
        formula_print(&rxn->products[i]);
    }

    printf("\n");
}

void reaction_print_detailed(const Reaction* rxn) {
    if (!rxn) return;

    printf("=== Chemical Reaction ===\n");
    printf("Equation: ");
    reaction_print(rxn);

    if (rxn->description[0]) {
        printf("Description: %s\n", rxn->description);
    }
    printf("Type: %s\n", reaction_type_str(rxn->type));
    printf("Condition: %s\n", reaction_condition_str(rxn->condition));
    printf("Balanced: %s\n", rxn->is_balanced ? "Yes" : "No");
    printf("Reversible: %s\n", rxn->is_reversible ? "Yes" : "No");

    /* Calculate mass */
    double reactant_mass = 0, product_mass = 0;
    for (int i = 0; i < rxn->reactant_count; i++) {
        reactant_mass += formula_mass(&rxn->reactants[i]);
    }
    for (int i = 0; i < rxn->product_count; i++) {
        product_mass += formula_mass(&rxn->products[i]);
    }
    printf("Reactant mass: %.3f g/mol\n", reactant_mass);
    printf("Product mass: %.3f g/mol\n", product_mass);
}

/* ============ Reaction Database ============ */

/* Helper to add a reaction to the database */
static void db_add_reaction(const char* reactants[], int r_count,
                           const char* products[], int p_count,
                           ReactionType type, ReactionCondition cond,
                           const char* description) {
    if (reaction_db_size >= MAX_REACTIONS) return;

    Reaction* rxn = &reaction_database[reaction_db_size];
    reaction_init(rxn);

    for (int i = 0; i < r_count; i++) {
        reaction_add_reactant(rxn, reactants[i]);
    }
    for (int i = 0; i < p_count; i++) {
        reaction_add_product(rxn, products[i]);
    }

    rxn->type = type;
    rxn->condition = cond;
    if (description) {
        strncpy(rxn->description, description, sizeof(rxn->description) - 1);
    }

    reaction_check_balanced(rxn);
    reaction_db_size++;
}

void reaction_db_init(void) {
    if (reaction_db_initialized) return;

    /* ===== Combustion Reactions ===== */

    /* Carbon + Oxygen -> Carbon Dioxide */
    db_add_reaction(
        (const char*[]){"C", "O2"}, 2,
        (const char*[]){"CO2"}, 1,
        RXTYPE_COMBUSTION, COND_HEATED,
        "Combustion of carbon"
    );

    /* Hydrogen + Oxygen -> Water */
    db_add_reaction(
        (const char*[]){"2H2", "O2"}, 2,
        (const char*[]){"2H2O"}, 1,
        RXTYPE_COMBUSTION, COND_HEATED,
        "Combustion of hydrogen"
    );

    /* Methane + Oxygen -> Carbon Dioxide + Water */
    db_add_reaction(
        (const char*[]){"CH4", "2O2"}, 2,
        (const char*[]){"CO2", "2H2O"}, 2,
        RXTYPE_COMBUSTION, COND_HEATED,
        "Combustion of methane"
    );

    /* ===== Synthesis Reactions ===== */

    /* Sodium + Chlorine -> Sodium Chloride */
    db_add_reaction(
        (const char*[]){"2Na", "Cl2"}, 2,
        (const char*[]){"2NaCl"}, 1,
        RXTYPE_SYNTHESIS, COND_NORMAL,
        "Formation of table salt"
    );

    /* Iron + Oxygen -> Iron(III) Oxide (rust) */
    db_add_reaction(
        (const char*[]){"4Fe", "3O2"}, 2,
        (const char*[]){"2Fe2O3"}, 1,
        RXTYPE_SYNTHESIS, COND_NORMAL,
        "Rusting of iron"
    );

    /* Nitrogen + Hydrogen -> Ammonia (Haber process) */
    db_add_reaction(
        (const char*[]){"N2", "3H2"}, 2,
        (const char*[]){"2NH3"}, 1,
        RXTYPE_SYNTHESIS, COND_CATALYST,
        "Haber process for ammonia synthesis"
    );

    /* Sulfur + Oxygen -> Sulfur Dioxide */
    db_add_reaction(
        (const char*[]){"S", "O2"}, 2,
        (const char*[]){"SO2"}, 1,
        RXTYPE_COMBUSTION, COND_HEATED,
        "Combustion of sulfur"
    );

    /* ===== Decomposition Reactions ===== */

    /* Water -> Hydrogen + Oxygen (electrolysis) */
    db_add_reaction(
        (const char*[]){"2H2O"}, 1,
        (const char*[]){"2H2", "O2"}, 2,
        RXTYPE_DECOMPOSITION, COND_ELECTROLYSIS,
        "Electrolysis of water"
    );

    /* Hydrogen Peroxide -> Water + Oxygen */
    db_add_reaction(
        (const char*[]){"2H2O2"}, 1,
        (const char*[]){"2H2O", "O2"}, 2,
        RXTYPE_DECOMPOSITION, COND_CATALYST,
        "Decomposition of hydrogen peroxide"
    );

    /* Calcium Carbonate -> Calcium Oxide + Carbon Dioxide */
    db_add_reaction(
        (const char*[]){"CaCO3"}, 1,
        (const char*[]){"CaO", "CO2"}, 2,
        RXTYPE_DECOMPOSITION, COND_HEATED,
        "Thermal decomposition of limestone"
    );

    /* ===== Acid-Base Reactions ===== */

    /* Hydrochloric Acid + Sodium Hydroxide -> Sodium Chloride + Water */
    db_add_reaction(
        (const char*[]){"HCl", "NaOH"}, 2,
        (const char*[]){"NaCl", "H2O"}, 2,
        RXTYPE_ACID_BASE, COND_NORMAL,
        "Neutralization reaction"
    );

    /* Sulfuric Acid + Sodium Hydroxide -> Sodium Sulfate + Water */
    db_add_reaction(
        (const char*[]){"H2SO4", "2NaOH"}, 2,
        (const char*[]){"Na2SO4", "2H2O"}, 2,
        RXTYPE_ACID_BASE, COND_NORMAL,
        "Neutralization with sulfuric acid"
    );

    /* ===== Single Replacement Reactions ===== */

    /* Zinc + Hydrochloric Acid -> Zinc Chloride + Hydrogen */
    db_add_reaction(
        (const char*[]){"Zn", "2HCl"}, 2,
        (const char*[]){"ZnCl2", "H2"}, 2,
        RXTYPE_SINGLE_REPLACE, COND_NORMAL,
        "Zinc displaces hydrogen from acid"
    );

    /* Iron + Copper Sulfate -> Iron Sulfate + Copper */
    db_add_reaction(
        (const char*[]){"Fe", "CuSO4"}, 2,
        (const char*[]){"FeSO4", "Cu"}, 2,
        RXTYPE_SINGLE_REPLACE, COND_NORMAL,
        "Iron displaces copper"
    );

    /* ===== Double Replacement Reactions ===== */

    /* Silver Nitrate + Sodium Chloride -> Silver Chloride + Sodium Nitrate */
    db_add_reaction(
        (const char*[]){"AgNO3", "NaCl"}, 2,
        (const char*[]){"AgCl", "NaNO3"}, 2,
        RXTYPE_DOUBLE_REPLACE, COND_NORMAL,
        "Precipitation of silver chloride"
    );

    /* Barium Chloride + Sodium Sulfate -> Barium Sulfate + Sodium Chloride */
    db_add_reaction(
        (const char*[]){"BaCl2", "Na2SO4"}, 2,
        (const char*[]){"BaSO4", "2NaCl"}, 2,
        RXTYPE_DOUBLE_REPLACE, COND_NORMAL,
        "Precipitation of barium sulfate"
    );

    /* ===== More Common Reactions ===== */

    /* Photosynthesis (simplified) */
    db_add_reaction(
        (const char*[]){"6CO2", "6H2O"}, 2,
        (const char*[]){"C6H12O6", "6O2"}, 2,
        RXTYPE_OTHER, COND_LIGHT,
        "Photosynthesis (simplified)"
    );

    /* Cellular Respiration (simplified) */
    db_add_reaction(
        (const char*[]){"C6H12O6", "6O2"}, 2,
        (const char*[]){"6CO2", "6H2O"}, 2,
        RXTYPE_COMBUSTION, COND_NORMAL,
        "Cellular respiration (simplified)"
    );

    /* Magnesium + Oxygen -> Magnesium Oxide */
    db_add_reaction(
        (const char*[]){"2Mg", "O2"}, 2,
        (const char*[]){"2MgO"}, 1,
        RXTYPE_SYNTHESIS, COND_HEATED,
        "Burning magnesium"
    );

    reaction_db_initialized = true;
}

/* Compare two formulas for matching (ignoring coefficients) */
static bool formulas_match_set(const Formula* set1, int count1,
                               const Formula* set2, int count2) {
    if (count1 != count2) return false;

    bool used[MAX_REACTANTS] = {false};

    for (int i = 0; i < count1; i++) {
        bool found = false;
        for (int j = 0; j < count2; j++) {
            if (used[j]) continue;

            /* Compare element composition (ignoring coefficient) */
            if (set1[i].element_count == set2[j].element_count) {
                bool match = true;
                for (int k = 0; k < set1[i].element_count; k++) {
                    bool el_found = false;
                    for (int l = 0; l < set2[j].element_count; l++) {
                        if (set1[i].elements[k].element == set2[j].elements[l].element) {
                            el_found = true;
                            break;
                        }
                    }
                    if (!el_found) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    used[j] = true;
                    found = true;
                    break;
                }
            }
        }
        if (!found) return false;
    }
    return true;
}

const Reaction* reaction_db_find(const Formula* reactants, int reactant_count) {
    if (!reaction_db_initialized) reaction_db_init();

    for (int i = 0; i < reaction_db_size; i++) {
        if (formulas_match_set(reactants, reactant_count,
                              reaction_database[i].reactants,
                              reaction_database[i].reactant_count)) {
            return &reaction_database[i];
        }
    }
    return NULL;
}

const Reaction* reaction_db_find_by_string(const char* reactants_str) {
    if (!reactants_str) return NULL;
    if (!reaction_db_initialized) reaction_db_init();

    /* Parse the reactants string (e.g., "C + O2") */
    Formula formulas[MAX_REACTANTS];
    int formula_count = 0;

    char buffer[MAX_FORMULA_LENGTH];
    strncpy(buffer, reactants_str, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    char* token = strtok(buffer, "+");
    while (token && formula_count < MAX_REACTANTS) {
        /* Trim whitespace */
        while (*token && isspace((unsigned char)*token)) token++;
        char* end = token + strlen(token) - 1;
        while (end > token && isspace((unsigned char)*end)) *end-- = '\0';

        if (formula_parse(token, &formulas[formula_count])) {
            formula_count++;
        }
        token = strtok(NULL, "+");
    }

    return reaction_db_find(formulas, formula_count);
}

int reaction_db_find_by_element(const Element* el, const Reaction** results, int max_results) {
    if (!el || !results || max_results <= 0) return 0;
    if (!reaction_db_initialized) reaction_db_init();

    int count = 0;
    for (int i = 0; i < reaction_db_size && count < max_results; i++) {
        bool found = false;

        /* Check reactants */
        for (int j = 0; j < reaction_database[i].reactant_count && !found; j++) {
            for (int k = 0; k < reaction_database[i].reactants[j].element_count; k++) {
                if (reaction_database[i].reactants[j].elements[k].element == el) {
                    found = true;
                    break;
                }
            }
        }

        /* Check products */
        for (int j = 0; j < reaction_database[i].product_count && !found; j++) {
            for (int k = 0; k < reaction_database[i].products[j].element_count; k++) {
                if (reaction_database[i].products[j].elements[k].element == el) {
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            results[count++] = &reaction_database[i];
        }
    }

    return count;
}

int reaction_db_count(void) {
    if (!reaction_db_initialized) reaction_db_init();
    return reaction_db_size;
}

const Reaction* reaction_db_get(int index) {
    if (!reaction_db_initialized) reaction_db_init();
    if (index < 0 || index >= reaction_db_size) return NULL;
    return &reaction_database[index];
}

/* ============ Simple Equation Balancing ============ */
/* Note: Full balancing is complex. This is a simplified version. */

bool reaction_balance(Reaction* rxn) {
    /* For now, just check if it's balanced */
    /* A full implementation would use linear algebra */
    return reaction_check_balanced(rxn);
}

/* ============ Reaction Prediction ============ */
/* Simplified prediction based on reaction types */

bool reaction_predict(const Formula* reactants, int reactant_count,
                     Formula* products, int* product_count) {
    if (!reactants || !products || !product_count) return false;

    /* First, check if we have this reaction in our database */
    const Reaction* known = reaction_db_find(reactants, reactant_count);
    if (known) {
        *product_count = known->product_count;
        for (int i = 0; i < known->product_count; i++) {
            products[i] = known->products[i];
        }
        return true;
    }

    /* Basic prediction rules could go here */
    /* For now, return false if not in database */
    *product_count = 0;
    return false;
}
