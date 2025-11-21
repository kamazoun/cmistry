/*
 * CMistry - Chemistry Application in C
 * Main demonstration program
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "element.h"
#include "molecule.h"
#include "reaction.h"

/* ============ Menu Functions ============ */

static void print_separator(void) {
    printf("----------------------------------------\n");
}

static void print_header(const char* title) {
    printf("\n");
    print_separator();
    printf("  %s\n", title);
    print_separator();
}

/* ============ Element Lookup Demo ============ */

static void demo_element_lookup(void) {
    print_header("Element Lookup");

    char input[32];
    printf("Enter element symbol, name, or atomic number: ");
    if (fgets(input, sizeof(input), stdin) == NULL) return;

    /* Remove newline */
    input[strcspn(input, "\n")] = 0;

    const Element* el = NULL;

    /* Try as number first */
    char* endptr;
    long num = strtol(input, &endptr, 10);
    if (*endptr == '\0' && num > 0) {
        el = element_by_number((int)num);
    }

    /* Try as symbol */
    if (!el) {
        el = element_by_symbol(input);
    }

    /* Try as name */
    if (!el) {
        el = element_by_name(input);
    }

    if (el) {
        printf("\n");
        element_print(el);
        printf("Max typical bonds: %d\n", element_max_bonds(el));
    } else {
        printf("Element not found: %s\n", input);
    }
}

/* ============ Formula Parser Demo ============ */

static void demo_formula_parser(void) {
    print_header("Chemical Formula Parser");

    char input[MAX_FORMULA_LENGTH];
    printf("Enter a chemical formula (e.g., H2O, CO2, C6H12O6): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return;
    input[strcspn(input, "\n")] = 0;

    Formula formula;
    if (formula_parse(input, &formula)) {
        printf("\nParsed formula: ");
        formula_print(&formula);
        printf("\n");

        printf("\nComposition:\n");
        for (int i = 0; i < formula.element_count; i++) {
            printf("  %s (%s): %d atom(s)\n",
                   formula.elements[i].element->name,
                   formula.elements[i].element->symbol,
                   formula.elements[i].count);
        }

        double mass = formula_mass(&formula);
        printf("\nMolecular mass: %.3f g/mol\n", mass);
    } else {
        printf("Failed to parse formula: %s\n", input);
    }
}

/* ============ Molecule Demo ============ */

static void demo_molecules(void) {
    print_header("Common Molecules");

    printf("\n--- Water (H2O) ---\n");
    Molecule* water = molecule_create_water();
    molecule_print(water);
    molecule_print_composition(water);

    printf("\n--- Carbon Dioxide (CO2) ---\n");
    Molecule* co2 = molecule_create_co2();
    molecule_print(co2);
    molecule_print_composition(co2);

    printf("\n--- Methane (CH4) ---\n");
    Molecule* methane = molecule_create_methane();
    molecule_print(methane);
    molecule_print_composition(methane);
}

/* ============ Reaction Lookup Demo ============ */

static void demo_reaction_lookup(void) {
    print_header("Reaction Lookup");

    /* Initialize database */
    reaction_db_init();

    char input[256];
    printf("Enter reactants separated by '+' (e.g., C + O2): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return;
    input[strcspn(input, "\n")] = 0;

    const Reaction* rxn = reaction_db_find_by_string(input);
    if (rxn) {
        printf("\nFound reaction:\n");
        reaction_print_detailed(rxn);
    } else {
        printf("\nNo known reaction found for: %s\n", input);
        printf("Try: C + O2, H2 + O2, Na + Cl2, CH4 + O2\n");
    }
}

/* ============ List All Reactions Demo ============ */

static void demo_list_reactions(void) {
    print_header("Known Reactions Database");

    reaction_db_init();
    int count = reaction_db_count();

    printf("\nDatabase contains %d reactions:\n\n", count);

    for (int i = 0; i < count; i++) {
        const Reaction* rxn = reaction_db_get(i);
        printf("%2d. ", i + 1);
        reaction_print(rxn);
        printf("    Type: %s, Condition: %s\n",
               reaction_type_str(rxn->type),
               reaction_condition_str(rxn->condition));
        if (rxn->description[0]) {
            printf("    %s\n", rxn->description);
        }
        printf("\n");
    }
}

/* ============ Periodic Table Overview ============ */

static void demo_periodic_table(void) {
    print_header("Periodic Table Overview");

    printf("\nFirst 36 elements (through Krypton):\n\n");
    printf("%-3s %-2s %-12s %8s %4s %5s\n",
           "Z", "Sy", "Name", "Mass", "Val", "EN");
    print_separator();

    for (int i = 1; i <= 36; i++) {
        const Element* el = element_by_number(i);
        if (el) {
            printf("%-3d %-2s %-12s %8.3f %4d %5.2f\n",
                   el->atomic_number,
                   el->symbol,
                   el->name,
                   el->atomic_mass,
                   el->valence_electrons,
                   el->electronegativity);
        }
    }

    printf("\n(Total elements in database: %d)\n", NUM_ELEMENTS);
}

/* ============ Reactions by Element Demo ============ */

static void demo_reactions_by_element(void) {
    print_header("Find Reactions by Element");

    char input[32];
    printf("Enter element symbol (e.g., O, C, Fe): ");
    if (fgets(input, sizeof(input), stdin) == NULL) return;
    input[strcspn(input, "\n")] = 0;

    const Element* el = element_by_symbol(input);
    if (!el) {
        printf("Unknown element: %s\n", input);
        return;
    }

    reaction_db_init();

    const Reaction* results[20];
    int count = reaction_db_find_by_element(el, results, 20);

    printf("\nReactions involving %s (%s):\n\n", el->name, el->symbol);

    if (count == 0) {
        printf("No reactions found in database.\n");
    } else {
        for (int i = 0; i < count; i++) {
            printf("%d. ", i + 1);
            reaction_print(results[i]);
            if (results[i]->description[0]) {
                printf("   %s\n", results[i]->description);
            }
        }
    }
}

/* ============ Interactive Menu ============ */

static void print_menu(void) {
    print_header("CMistry - Chemistry Application");
    printf("\n");
    printf("  1. Look up an element\n");
    printf("  2. Parse a chemical formula\n");
    printf("  3. Show common molecules\n");
    printf("  4. Look up a reaction\n");
    printf("  5. List all known reactions\n");
    printf("  6. Show periodic table overview\n");
    printf("  7. Find reactions by element\n");
    printf("  0. Exit\n");
    printf("\n");
    printf("Enter choice: ");
}

int main(void) {
    char input[32];
    int choice;

    printf("\n");
    printf("  ____  __  __  _     _              \n");
    printf(" / ___|/  \\/  |(_)___| |_ _ __ _   _ \n");
    printf("| |   | |\\/| || / __| __| '__| | | |\n");
    printf("| |___| |  | || \\__ \\ |_| |  | |_| |\n");
    printf(" \\____|_|  |_||_|___/\\__|_|   \\__, |\n");
    printf("                              |___/ \n");
    printf("        Chemistry in C              \n");

    while (1) {
        print_menu();

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        choice = atoi(input);

        switch (choice) {
            case 0:
                printf("\nGoodbye!\n");
                return 0;
            case 1:
                demo_element_lookup();
                break;
            case 2:
                demo_formula_parser();
                break;
            case 3:
                demo_molecules();
                break;
            case 4:
                demo_reaction_lookup();
                break;
            case 5:
                demo_list_reactions();
                break;
            case 6:
                demo_periodic_table();
                break;
            case 7:
                demo_reactions_by_element();
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

        printf("\nPress Enter to continue...");
        (void)getchar();
    }

    return 0;
}
