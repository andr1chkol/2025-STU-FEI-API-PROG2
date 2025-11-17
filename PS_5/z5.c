#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// NEMENIT, inak vas kod neprejde testom !!!
#include "data.h"


#define MAX_ITEMS 30
#define MAX_SLOTS 2

typedef struct {
    int army;
    const char* attacker_name;
    const char* item;
    const char* defender_name;
    int damage;
    int item_index;
} DAMAGE;

int input_items(char* start, const char* attribute, const char* format, int* destination);
void print_items(ITEM* items, int num_items);
int game(const char* file_army_1, const char* file_army_2, ITEM* items, int num_items);
UNIT* input_army(FILE* f, int *army_count, ITEM* items, int item_count);
ITEM* item_find(const char* search_item, ITEM* items, int item_count);
void print_table(const UNIT* army, int army_count);


int main(const int argc, char *argv[]) {
    if (argc == 1 || argc == 3) {


        ITEM ITEMS[MAX_ITEMS];
        int item_count = 0;

        FILE* f = fopen(ITEMS_FILE, "r");
        if (f == NULL) {
            fprintf(stderr, "%s\n", ERR_FILE);
            return 0;
        }

        fseek(f, 0, SEEK_END);
        long FILE_SIZE = ftell(f);
        rewind(f);

        char* items_file = malloc(FILE_SIZE+1);
        fread(items_file, 1, FILE_SIZE, f);
        items_file[FILE_SIZE] = '\0';

        char* ptr_start = strstr(items_file, "{");
        while (ptr_start != NULL) {


            char* name_ptr = strstr(ptr_start, "\"name\"");
            if (name_ptr == NULL) {
                fprintf(stderr, "%s\n", ERR_MISSING_ATTRIBUTE);
                free(items_file);
                fclose(f);
                return 0;

            }
            int exist = sscanf(name_ptr, "\"name\" : \"%[^\"]\"", ITEMS[item_count].name);
                if (!exist) {
                    fprintf(stderr, "%s\n", ERR_MISSING_VALUE);
                    free(items_file);
                    fclose(f);
                    return 0;
                }
            if (!input_items(ptr_start, "\"att\"", "\"att\" : %d", &ITEMS[item_count].att)
                || !input_items(ptr_start, "\"def\"", "\"def\" : %d", &ITEMS[item_count].def)
                || !input_items(ptr_start, "\"slots\"", "\"slots\" : %d", &ITEMS[item_count].slots)
                || !input_items(ptr_start, "\"range\"", "\"range\" : %d", &ITEMS[item_count].range)
                || !input_items(ptr_start, "\"radius\"", "\"radius\" : %d", &ITEMS[item_count].radius)
                ) {
                free(items_file);
                fclose(f);
                return 0;
                }
            item_count ++;
            ptr_start = strstr(ptr_start + 1, "{");
        }

        if (argc == 1) {
            print_items(ITEMS, item_count);
        }
        if (argc == 3) {
            game(argv[1], argv[2], ITEMS, item_count);
        }
        free(items_file);
        fclose(f);
        return 0;
    }

        fprintf(stderr, "%s\n", ERR_CMD);
        return 0;

}

int input_items(char* start, const char* attribute, const char* format, int* destination) {
    char* pointer = strstr(start, attribute);
    if (pointer == NULL) {
        fprintf(stderr, "%s\n", ERR_MISSING_ATTRIBUTE);
        return 0;
    }

    char* colon = strchr(pointer, ':');
    if (colon == NULL) {
        fprintf(stderr, "%s\n", ERR_MISSING_VALUE);
        return 0;
    }

    colon++;
    while (isspace(*colon)) colon++;

    if (*colon == ',' || *colon == '}' || *colon == '\0') {
        fprintf(stderr, "%s\n", ERR_MISSING_VALUE);
        return 0;
    }

    if (!isdigit(*colon) ) {
        fprintf(stderr, "%s\n", ERR_BAD_VALUE);
        return 0;
    }

    if (sscanf(pointer, format, destination) != 1) {
        fprintf(stderr, "%s\n", ERR_BAD_VALUE);
        return 0;
    }

    return 1;
}

void print_items(ITEM* items, int num_items) {
	for (int i = 0; i < num_items; i++) {
		printf("Name:  %s\n", items[i].name);
		printf("Attack:  %d\n", items[i].att);
		printf("Defense: %d\n", items[i].def);
		printf("Slots:  %d\n", items[i].slots);
		printf("Range:  %d\n", items[i].range);
		printf("Radius: %d\n\n", items[i].radius);
	}
}


int game(const char* file_army_1, const char* file_army_2, ITEM* items, int num_items) {
    FILE* input_army_1 = fopen(file_army_1, "r");
    FILE* input_army_2 = fopen(file_army_2, "r");

    if (input_army_1 == NULL || input_army_2 == NULL) {
        fprintf(stderr, "%s\n", ERR_FILE);
        return 0;
    }

    int army_1_count;
    UNIT* army_1 = input_army(input_army_1, &army_1_count, items, num_items);
    if (army_1 == NULL) return 0;

    int army_2_count;
    UNIT* army_2 = input_army(input_army_2, &army_2_count, items, num_items);
    if (army_2 == NULL) {
        free(army_1);
        return 0;
    }

    printf("Army 1\n");
    print_table(army_1, army_1_count);
    printf("Army 2\n");
    print_table(army_2, army_2_count);

    fclose(input_army_1);
    fclose(input_army_2);
    free(army_1);
    free(army_2);
    return 0;
}

UNIT* input_army(FILE* f, int *army_count, ITEM* items, const int item_count) {
    fscanf(f, "%d", army_count);
    fgetc(f);

    if (*army_count < MIN_ARMY || *army_count > MAX_ARMY) {
        fprintf(stderr, "%s\n", ERR_UNIT_COUNT);
        return 0;
    }

    UNIT* army = malloc(sizeof(UNIT) * (*army_count));
    if (army == NULL){
        return NULL;
    }

    for (int i = 0; i < *army_count; i++) {
        char input[256];

        if (fgets(input, sizeof(input), f) == NULL) {
            free(army);
            return NULL;
        }
        while (input[0] == '\n') {
            if (fgets(input, sizeof(input), f) == NULL) {
                free(army);
                return NULL;
            }
        }

        char item_1[MAX_NAME+1] = "";
        char item_2[MAX_NAME+1] = "";
        char item_3[MAX_NAME+1] = "";

        int count = sscanf(input, "%s %s %s %s",  army[i].name, item_1, item_2, item_3);

        if (count < 2 || count > 3) {
            fprintf(stderr, "%s\n", ERR_ITEM_COUNT);
            free(army);
            return 0;
        }

        army[i].item1 = item_find(item_1, items, item_count);
        if (army[i].item1 == NULL) {
            free(army);
            return NULL;
        }
        army[i].item2 = NULL;

        if (count > 2) {
            army[i].item2 = item_find(item_2, items, item_count);
            if (army[i].item2 == NULL) {
                free(army);
                return NULL;
            }
        }

        int slots = army[i].item1 -> slots;
        if (army[i].item2 != NULL) {
            slots += army[i].item2 -> slots;
        }
        if (slots > MAX_SLOTS) {
            fprintf(stderr, "%s\n", ERR_SLOTS);
            free(army);
            return 0;
        }

        army[i].hp = 100;
    }
    return army;
}

ITEM* item_find(const char* search_item, ITEM* items, const int item_count) {
    for (int i = 0; i < item_count; i++) {
        if (strcmp(items[i].name, search_item) == 0) {
            return &items[i];
        }
    }
    fprintf(stderr, "%s\n", ERR_WRONG_ITEM);
    return NULL;
}

void print_table(const UNIT* army, int army_count){
    for (int i = 0; i < army_count; i++) {
        printf("    Unit: %d\n", i );
        printf("    Name: %s\n", army[i].name);
        printf("    HP: %d\n", army[i].hp);
        printf("    Item 1: %s,%d,%d,%d,%d,%d\n", army[i].item1->name, army[i].item1->att, army[i].item1->def,
            army[i].item1->slots,army[i].item1->range, army[i].item1->radius );

        if (army[i].item2 != NULL) {
            printf("    Item 2: %s,%d,%d,%d,%d,%d\n", army[i].item2->name, army[i].item2->att, army[i].item2->def,
            army[i].item2->slots,army[i].item2->range, army[i].item2->radius );
        }

        printf("\n");
    }
}

