#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "data.h" // NEMENIT, inak vas kod neprejde testom !!!

#define ERR_UNIT_COUNT "ERR_UNIT_COUNT"
#define ERR_ITEM_COUNT "ERR_ITEM_COUNT"
#define ERR_WRONG_ITEM "ERR_WRONG_ITEM"
#define ERR_SLOTS "ERR_SLOTS"

#define MAX_SLOTS 2

typedef struct {
    int army;
    const char* attacker_name;
    const char* item;
    const char* defender_name;
    int damage;
    int item_index;
} DAMAGE;


UNIT* input_army(int *army_count);
const ITEM* item_find(const char* search_item);
void print_table(const UNIT* army, int army_count);
void print_units(const UNIT* army1, const UNIT* army2,  int army_1_count, int army_2_count);
int is_game_over(int army1_count, int army2_count);
void attack_army(const UNIT* army_attack, int attack_count, const UNIT* army_deffence, int deffence_count, int* damage,
    DAMAGE** events, int *event_count, int *event_size, int army_id );
void print_damage(DAMAGE* damage, int damage_count);
void update_army(UNIT* army, int* army_count, int* damage);

int main(const int argc, char *argv[]) {
    int max_rounds = -1;

    if (argc == 2) {
        max_rounds = atoi(argv[1]);
        if (max_rounds < 0) {
            return 1;
        }
    }

    int event_count = 0;
    int event_size = 100;
    DAMAGE* events = malloc(sizeof(DAMAGE) * event_size);

    int round = 1;

    int army_1_count;
    UNIT* army_1 = input_army(&army_1_count);
    if (army_1 == NULL) return 0;

    int army_2_count;
    UNIT* army_2 = input_army(&army_2_count);
    if (army_2 == NULL) {
        free(army_1);
        return 0;
    }


    printf("Army 1\n");
    print_table(army_1, army_1_count);
    printf("Army 2\n");
    print_table(army_2, army_2_count);

    while(!is_game_over(army_1_count, army_2_count) &&
          (max_rounds == -1 || round <= max_rounds)) {
        if (round != 1) {
            printf("\n");
        }
        printf("Round %d\n", round);
        event_count = 0;
        int* damage_to_1 = calloc(army_1_count, sizeof(int));
        int* damage_to_2 = calloc(army_2_count, sizeof(int));

        print_units(army_1, army_2, army_1_count, army_2_count);
        attack_army(army_1, army_1_count, army_2, army_2_count, damage_to_1, &events, &event_count, &event_size, 1);
        attack_army(army_2, army_2_count, army_1, army_1_count, damage_to_2, &events, &event_count, &event_size, 2);
        print_damage(events, event_count);
        update_army(army_1, &army_1_count, damage_to_2);
        update_army(army_2, &army_2_count, damage_to_1);
        print_units(army_1, army_2,  army_1_count, army_2_count);
        round++;

    free(damage_to_1);
    free(damage_to_2);
    }

    free(events);
    free(army_1);
    free(army_2);
    return 0;
}

UNIT* input_army(int *army_count) {
    scanf("%d", army_count);
    getchar();

    if (*army_count < MIN_ARMY || *army_count > MAX_ARMY) {
        printf(ERR_UNIT_COUNT"\n");
        return NULL;
    }

    UNIT* army = malloc(sizeof(UNIT) * (*army_count));
    if (army == NULL) return NULL;

    for (int i = 0; i < *army_count; i++) {
        char input[256];

        fgets(input,sizeof(input) , stdin);

        while (input[0] == '\n') {
            fgets(input, sizeof(input), stdin);
        }


        char item_1[MAX_NAME+1] = "";
        char item_2[MAX_NAME+1] = "";
        char item_3[MAX_NAME+1] = "";

        int count = sscanf(input, "%s %s %s %s",  army[i].name, item_1, item_2, item_3);

        if (count < 2 || count > 3) {
            printf(ERR_ITEM_COUNT"\n");
            free(army);
            return 0;
        }

        army[i].item1 = item_find(item_1);
        if (army[i].item1 == NULL) {
            free(army);
            return 0;
        }
        army[i].item2 = NULL;

        if (count > 2) {
            army[i].item2 = item_find(item_2);
            if (army[i].item2 == NULL) {
                free(army);
                return 0;
            }
        }

        int slots = army[i].item1 -> slots;
        if (army[i].item2 != NULL) {
            slots += army[i].item2 -> slots;
        }
        if (slots > MAX_SLOTS) {
            printf(ERR_SLOTS"\n");
            free(army);
            return 0;
        }

        army[i].hp = 100;
    }
    return army;
}

const ITEM* item_find(const char* search_item) {
    for (int i = 0; i < NUMBER_OF_ITEMS; i++) {
        if (strcmp(items[i].name, search_item) == 0) {
            return &items[i];
        }
    }
    printf(ERR_WRONG_ITEM"\n");
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

void print_units(const UNIT* army1, const UNIT* army2,  int army_1_count, int army_2_count) {
    printf("1: ");
    for (int i = 0; i < army_1_count; i++) {
        printf("%s,%d ", army1[i].name, army1[i].hp);
    }
    printf("\n");
    printf("2: ");
    for (int i = 0; i < army_2_count; i++) {
        printf("%s,%d ", army2[i].name, army2[i].hp);
    }
    printf("\n");

}

void attack_army(const UNIT* army_attack, int attack_count, const UNIT* army_deffence, int deffence_count, int* damage,
    DAMAGE** events, int* event_count, int* event_size, int army_id ) {

    for (int i = 0; i < attack_count; i++) {
        const ITEM* items[2] = {army_attack[i].item1, army_attack[i].item2};

        for (int IT = 0; IT < 2; IT++) {
            const ITEM* item = items[IT];
            if (!item) continue;

            if (item->range < i) continue;

            for (int j = 0; j <= item->radius; j++) {
                if (j >= deffence_count) break;

                const UNIT* aim = &army_deffence[j];

                int def = aim->item1->def;
                if (aim->item2) def += aim->item2->def;

                int dmg = item->att - def;
                if (dmg < 1) dmg = 1;

                damage[j] += dmg;

                if (*event_count >= *event_size) {
                    *event_size *= 2;
                    DAMAGE* resized = realloc(*events, sizeof(DAMAGE) * (*event_size));
                    if (!resized) {
                        exit(1);
                    }
                    *events = resized;
                }

                (*events)[*event_count].army = army_id;
                (*events)[*event_count].attacker_name = army_attack[i].name;
                (*events)[*event_count].item = item->name;
                (*events)[*event_count].defender_name = army_deffence[j].name;
                (*events)[*event_count].damage = dmg;
                (*events)[*event_count].item_index = IT;
                (*event_count)++;
            }
        }
    }
}

void print_damage(DAMAGE* damage, int damage_count) {
    for (int i = 0; i < damage_count; i++) {

        int already_printed = 0;
        for (int j = 0; j < i; j++) {
            if (damage[i].army == damage[j].army &&
                strcmp(damage[i].attacker_name, damage[j].attacker_name) == 0 &&
                strcmp(damage[i].item, damage[j].item) == 0 &&
                damage[i].item_index == damage[j].item_index) {
                already_printed = 1;
                break;
                }

        }
        if (already_printed) continue;


        printf("%d,%s,%s:\t", damage[i].army, damage[i].attacker_name, damage[i].item);


        for (int k = 0; k < damage_count; k++) {
            if (damage[k].army == damage[i].army &&
                strcmp(damage[k].attacker_name, damage[i].attacker_name) == 0 &&
                strcmp(damage[k].item, damage[i].item) == 0 &&
                damage[k].item_index == damage[i].item_index) {
                printf(" [%s,%d]", damage[k].defender_name, damage[k].damage);
                }
        }
        printf("\n");
    }
}

void update_army(UNIT* army, int* army_count, int* damage) {
    int alive = 0;

    for (int i = 0; i < *army_count; i++) {
        army[i].hp -= damage[i];

        if (army[i].hp > 0) {
            if (i != alive) {
                army[alive] = army[i];
            }
            alive++;
        }
    }

    *army_count = alive;
}

int is_game_over(int army1_count, int army2_count) {
    if (army1_count== 0 && army2_count ==0) {
        printf("\n");
        printf("NO WINNER\n");
        return 1;
    }
    if (army1_count == 0) {
        printf("\n");
        printf("WINNER: 2\n");
        return 1;
    }
    if (army2_count == 0) {
        printf("\n");
        printf("WINNER: 1\n");
        return 1;
    }
    return 0;

}