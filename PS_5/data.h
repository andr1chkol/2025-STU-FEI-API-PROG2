/*
 *  Makra a struktury
 */

#ifndef DATA_H
#define DATA_H

#define ITEMS_FILE "items.json"   // fixne dana cesta k JSON suboru s predmetmi ... NEMENIT !!!
#define MAX_LINE 1000       // maximalna dlzka riadku v TXT/JSON subore
#define MAX_FILENAME 500    // maximalna dlzka cesty k suboru
#define MAX_NAME 100        // maximalna dlzka nazvu/mena predmetu/jednotky
#define MIN_ARMY 1          // minimalny pocet bojovych jednotiek v armade
#define MAX_ARMY 5          // maximalny pocet bojovych jednotiek v armade

// chyby vznikajuce pocas nacitania predmetov z JSON suboru
#define ERR_MISSING_ATTRIBUTE "ERR_MISSING_ATTRIBUTE"   // chybajuci atribut predmetu
#define ERR_MISSING_VALUE "ERR_MISSING_VALUE"           // chybajuca hodnota kluca
#define ERR_BAD_VALUE "ERR_BAD_VALUE"                   // zla hodnota kluca

// chyby vznikajuce pocas nacitania armady z TXT suboru
#define ERR_UNIT_COUNT "ERR_UNIT_COUNT"                 // nespravny pocet jednotiek
#define ERR_ITEM_COUNT "ERR_ITEM_COUNT"                 // nepovoleny pocet predmetov v inventari
#define ERR_WRONG_ITEM "ERR_WRONG_ITEM"                 // neznamy predmet v inventari
#define ERR_SLOTS "ERR_SLOTS"                           // pocet slotov prekroceny

// dalsie chyby
#define ERR_FILE "ERR_FILE"                             // subor sa neda otvorit (plati pre JSON aj TXT subor)
#define ERR_CMD "ERR_CMD"                               // zly pocet CMD argumentov

// Struktura pre predmet v inventari
typedef struct item {
    char name[MAX_NAME + 1];        // nazov predmetu
    int att;                        // utok
    int def;                        // obrana
    int slots;                      // pocet slotov, ktore zabera predmet v inventari
    int range;                      // utocny dosah/dostrel predmetu
    int radius;                     // sirka zasahu (alebo damage radius)
} ITEM;

// Struktura pre bojovu jednotku
typedef struct unit {
    char name[MAX_NAME + 1];    // meno jednotky
    const ITEM *item1;          // prvy predmet v inventari (ukazuje do pola 'items')
    const ITEM *item2;          // druhy predmet v inventari (ukazuje do pola 'items')
    int hp;                     // zdravie (z angl. hit points)
} UNIT;

#endif //DATA_H


