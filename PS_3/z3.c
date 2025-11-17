#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define N 100
#define MMAX 200

const char *readline() {
    static char buffer[MMAX + 1] = {0};
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    return buffer;
}

void update_words(char *op, char *words[], int *count) {
    *count = 0;
    int in_word = 0;
    for (int i = 0; op[i] != '\0'; i++) {
        if (isalnum(op[i])) {
            if (!in_word) {
                words[*count] = &op[i];
                (*count)++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
}

int main(void) {
    char op[N+1];
    char mem[N+1];
    op[0] = '\0';
    mem[0] = '\0';

    char *sp = op;
    char *ep = op;

    char *words[N];

    int count = 0;
    int index = 0;

    while (1) {
        const char *string = readline();
        if (strcmp(string, "quit") == 0) {
            break;
        }

        if (strncmp(string, "in ", 3) == 0) {
            if (strlen(string + 3) > N) {
                printf("ERR_OVERFLOW\n");
                break;
            }
            strcpy(op, string + 3);
            sp = op;
            ep = op + strlen(op);

            update_words(op, words, &count);
            index = 0;
        }

        if (strncmp(string, "out op", 6) == 0) {
            printf("%s\n", op);
        }

        if (strncmp(string, "out range", 9) == 0) {
            if (sp > ep) {
                printf("ERR_POSITION\n");
                break;
            }
            if (sp == ep) {
                printf("\n");
            } else {
                int length = ep - sp;
                char temp[length + 1];
                strncpy(temp, sp, length);
                temp[length] = '\0';
                printf("%s\n", temp);
            }
        }

        if (strncmp(string, "out mem", 8) == 0) {
            printf("%s\n", mem);
        }

        if (strncmp(string, "move sp", 7) == 0) {
            int n = atoi(string + 8);
            sp += n;
            if (sp < op || sp > op + strlen(op)) {
                printf("ERR_OUT_OF_RANGE\n");
                break;
            }
        } else if (strncmp(string, "move ep", 7) == 0) {
            int n = atoi(string + 8);
            ep += n;
            if (ep < op || ep > op + strlen(op)) {
                printf("ERR_OUT_OF_RANGE\n");
                break;
            }
        }

        if (strncmp(string, "start sp", 9) == 0) {
            sp = op;
        } else if (strncmp(string, "start ep", 9) == 0) {
            ep = op;
        }

        if (strncmp(string, "end sp", 9) == 0) {
            sp = op + strlen(op);
        } else if (strncmp(string, "end ep", 9) == 0) {
            ep = op + strlen(op);
        }

        if (strncmp(string, "first", 5) == 0) {
            if (count > 0) {
                sp = words[0];
                ep = sp;
                while (isalnum(*ep)) ep++;
                index = 0;
            } else {
                sp = ep = op;
            }
        }

        if (strncmp(string, "last", 4) == 0) {
            if (count > 0) {
                sp = words[count - 1];
                ep = sp;
                while (isalnum(*ep)) ep++;
                index = count - 1;
            } else {
                sp = ep = op + strlen(op);
            }
        }

        if (strncmp(string, "prev", 4) == 0) {
            index = -1;
            for (int i = count - 1; i >= 0; i--) {
                char *start = words[i];
                char *end = start;
                while (isalnum(*end)) end++;

                if (sp > end) {
                    index = i;
                    break;
                }
                if (sp > start && sp <= end) {
                    index = i - 1;
                    break;
                }
            }

            if (index >= 0) {
                sp = words[index];
                ep = sp;
                while (isalnum(*ep)) ep++;
            } else {
                sp = ep = op;
            }
        }

        if (strncmp(string, "next", 4) == 0) {
            index = -1;
            for (int i = 0; i < count; i++) {
                if (sp < words[i]) {
                    index = i;
                    break;
                }
            }
            if (index != -1 && index < count) {
                sp = words[index];
                ep = sp;
                while (isalnum(*ep)) ep++;
            } else {
                sp = ep = op + strlen(op);
            }
        }

        if (strncmp(string, "del", 3) == 0) {
            if (sp > ep) {
                printf("ERR_POSITION\n");
                break;
            }
            memmove(sp, ep, strlen(ep) + 1);
            ep = sp;
        }

        if (strncmp(string, "crop", 4) == 0) {
            if (sp > ep) {
                printf("ERR_POSITION\n");
                break;
            }
            if (sp == ep) {
                op[0] = '\0';
                sp = ep = op;
            } else {
                int word_len = ep - sp;
                memmove(op, sp, word_len);
                op[word_len] = '\0';
                sp = op;
                ep = op + word_len;
            }
        }

        if (strncmp(string, "copy", 4) == 0) {
            if (sp > ep) {
                printf("ERR_POSITION\n");
                break;
            } else {
                int word_len = ep - sp;
                memmove(mem, sp, word_len);
                mem[word_len] = '\0';
            }
        }

        if (strncmp(string, "insert", 6) == 0) {
            int n = atoi(string + 7);
            if (sp > ep) {
                printf("ERR_POSITION\n");
                break;
            }
            if (n + strlen(op) > N) {
                printf("ERR_OVERFLOW\n");
                break;
            }
            memmove(sp + n, sp, strlen(sp) + 1);
            for (int i = 0; i < n; i++) {
                sp[i] = ' ';
            }
            ep += n;
        }

        if (strncmp(string, "insert mem", 10) == 0) {
            if (sp > ep) {
                printf("ERR_POSITION\n");
                break;
            }
            if (strlen(mem) + strlen(op) > N) {
                printf("ERR_OVERFLOW\n");
                break;
            }
            memmove(sp + strlen(mem), sp, strlen(sp) + 1);
            memcpy(sp, mem, strlen(mem));
            ep = sp + strlen(mem);
        }

        if (strncmp(string, "replace", 7) == 0) {
            if (sp > ep) {
                printf("ERR_POSITION\n");
                break;
            }
            if (sp == ep || mem[0] == '\0') {
                continue;
            }

            int sel_len = ep - sp;
            int mem_len = strlen(mem);

            if (strlen(op) - sel_len + mem_len > N) {
                printf("ERR_OVERFLOW\n");
                break;
            }

            memmove(sp + mem_len, ep, strlen(ep) + 1);
            memcpy(sp, mem, mem_len);
            ep = sp + mem_len;
        }
    }
}