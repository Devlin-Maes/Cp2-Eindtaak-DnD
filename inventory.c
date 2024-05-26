#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 100
#define BUFFER_SIZE 4096

typedef struct {
    char name[50];
    double weight;
    double cost;
    int count;
} Item;

typedef struct {
    Item items[MAX_ITEMS];
    int item_count;
    double max_weight;
    int cp, sp, ep, gp, pp;
    char camp_file[50];
} Inventory;

void parse_money(const char *money_str, Inventory *inventory) {
    char *money = strdup(money_str);
    char *token = strtok(money, " ");
    while (token != NULL) {
        int value = atoi(token);
        char *type = token + strlen(token) - 2;
        if (strcmp(type, "cp") == 0) inventory->cp = value;
        if (strcmp(type, "sp") == 0) inventory->sp = value;
        if (strcmp(type, "ep") == 0) inventory->ep = value;
        if (strcmp(type, "gp") == 0) inventory->gp = value;
        if (strcmp(type, "pp") == 0) inventory->pp = value;
        token = strtok(NULL, " ");
    }
    free(money);
}

void parse_json(const char *buffer, Item *item) {
    char *name_start = strstr(buffer, "\"name\": \"");
    if (name_start) {
        name_start += 9;
        char *name_end = strchr(name_start, '\"');
        if (name_end) {
            strncpy(item->name, name_start, name_end - name_start);
            item->name[name_end - name_start] = '\0';
        }
    }

    char *weight_start = strstr(buffer, "\"weight\": ");
    if (weight_start) {
        weight_start += 10;
        item->weight = atof(weight_start);
    }

    char *cost_start = strstr(buffer, "\"quantity\": ");
    if (cost_start) {
        cost_start += 12;
        item->cost = atof(cost_start);
    }
}

void load_equipment(const char *filename, int count, Inventory *inventory) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    fread(buffer, sizeof(char), BUFFER_SIZE, file);
    fclose(file);

    buffer[BUFFER_SIZE - 1] = '\0';  // Ensure null-termination
    Item item = {0};
    parse_json(buffer, &item);
    item.count = count;

    inventory->items[inventory->item_count++] = item;
}

void print_inventory(const Inventory *inventory) {
    double total_weight = 0;
    double total_cost = 0;

    for (int i = 0; i < inventory->item_count; i++) {
        total_weight += inventory->items[i].weight * inventory->items[i].count;
        total_cost += inventory->items[i].cost * inventory->items[i].count;
    }

    total_cost += inventory->cp * 0.01 + inventory->sp * 0.1 + inventory->ep * 0.5 + inventory->gp * 1 + inventory->pp * 10;

    printf("Total weight: %.2f %s\n", total_weight, total_weight > inventory->max_weight ? "(encumbered)" : "");
    printf("Total cost: %.2f gp\n", total_cost);
    printf("Coins: %d cp, %d sp, %d ep, %d gp, %d pp\n", inventory->cp, inventory->sp, inventory->ep, inventory->gp, inventory->pp);
}

void usage(const char *progname) {
    printf("Usage: %s [options] equipment-files\n", progname);
    printf("Options:\n");
    printf("  -w max-weight        Maximum weight before becoming encumbered\n");
    printf("  -m money             List of coins and types (cp, sp, ep, gp, pp)\n");
    printf("  -c camp-file         Optional camp file for all discovered items during play that stay in camp\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    Inventory inventory = {0};
    inventory.max_weight = 9999.9;

    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-w") == 0) {
            if (++i < argc) {
                inventory.max_weight = atof(argv[i]);
            } else {
                fprintf(stderr, "Missing argument for -w\n");
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "-m") == 0) {
            if (++i < argc) {
                parse_money(argv[i], &inventory);
            } else {
                fprintf(stderr, "Missing argument for -m\n");
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "-c") == 0) {
            if (++i < argc) {
                strncpy(inventory.camp_file, argv[i], sizeof(inventory.camp_file) - 1);
            } else {
                fprintf(stderr, "Missing argument for -c\n");
                return EXIT_FAILURE;
            }
        } else {
            int count = 1;
            char *filename = argv[i];
            if (i + 1 < argc && argv[i + 1][0] >= '0' && argv[i + 1][0] <= '9') {
                count = atoi(argv[++i]);
            }
            load_equipment(filename, count, &inventory);
        }
        i++;
    }

    print_inventory(&inventory);

    return EXIT_SUCCESS;
}
