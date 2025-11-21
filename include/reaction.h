#ifndef REACTION_H
#define REACTION_H

#include "molecule.h"
#include <stdbool.h>

#define MAX_REACTANTS 10
#define MAX_PRODUCTS 10
#define MAX_REACTIONS 100

/* Reaction conditions */
typedef enum {
    COND_NORMAL,            /* Room temperature and pressure (STP) */
    COND_HEATED,            /* Requires heat */
    COND_HIGH_PRESSURE,     /* Requires high pressure */
    COND_CATALYST,          /* Requires a catalyst */
    COND_LIGHT,             /* Requires light (photochemical) */
    COND_ELECTROLYSIS       /* Requires electricity */
} ReactionCondition;

/* Reaction type classification */
typedef enum {
    RXTYPE_SYNTHESIS,       /* A + B -> AB (combination) */
    RXTYPE_DECOMPOSITION,   /* AB -> A + B */
    RXTYPE_SINGLE_REPLACE,  /* A + BC -> AC + B */
    RXTYPE_DOUBLE_REPLACE,  /* AB + CD -> AD + CB */
    RXTYPE_COMBUSTION,      /* Fuel + O2 -> CO2 + H2O */
    RXTYPE_ACID_BASE,       /* Acid + Base -> Salt + Water */
    RXTYPE_REDOX,           /* Oxidation-reduction */
    RXTYPE_OTHER
} ReactionType;

/* A chemical reaction */
typedef struct {
    Formula reactants[MAX_REACTANTS];
    int reactant_count;

    Formula products[MAX_PRODUCTS];
    int product_count;

    ReactionCondition condition;
    ReactionType type;
    char description[128];

    bool is_balanced;
    bool is_reversible;
} Reaction;

/* Initialize a reaction */
void reaction_init(Reaction* rxn);

/* Add reactant/product to reaction */
bool reaction_add_reactant(Reaction* rxn, const char* formula);
bool reaction_add_product(Reaction* rxn, const char* formula);

/* Set reaction properties */
void reaction_set_condition(Reaction* rxn, ReactionCondition cond);
void reaction_set_type(Reaction* rxn, ReactionType type);
void reaction_set_description(Reaction* rxn, const char* desc);

/* Check if reaction is balanced (conservation of mass) */
bool reaction_check_balanced(Reaction* rxn);

/* Print reaction as equation */
void reaction_print(const Reaction* rxn);
void reaction_print_detailed(const Reaction* rxn);

/* String conversions */
const char* reaction_condition_str(ReactionCondition cond);
const char* reaction_type_str(ReactionType type);

/* ============ Reaction Database ============ */

/* Initialize the reaction database with known reactions */
void reaction_db_init(void);

/* Find a reaction given reactants (returns NULL if not found) */
const Reaction* reaction_db_find(const Formula* reactants, int reactant_count);

/* Find reaction by string input (e.g., "C + O2") */
const Reaction* reaction_db_find_by_string(const char* reactants_str);

/* Get all reactions involving an element */
int reaction_db_find_by_element(const Element* el, const Reaction** results, int max_results);

/* Get total number of reactions in database */
int reaction_db_count(void);

/* Get reaction by index */
const Reaction* reaction_db_get(int index);

/* ============ Equation Balancing ============ */

/* Attempt to balance a reaction equation */
bool reaction_balance(Reaction* rxn);

/* ============ Reaction Prediction ============ */

/* Predict products of a reaction based on reactants and rules */
bool reaction_predict(const Formula* reactants, int reactant_count,
                     Formula* products, int* product_count);

#endif /* REACTION_H */
