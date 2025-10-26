

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_LEN 64

typedef struct {
    char name[NAME_LEN];
    double net;

typedef struct {
    Friend *arr;
    int count;
    int capacity;
} FriendList;

void init_list(FriendList *fl) {
    fl->count = 0;
    fl->capacity = 4;
    fl->arr = malloc(sizeof(Friend) * fl->capacity);
    if (!fl->arr) { perror("malloc"); exit(1); }
}

void ensure_capacity(FriendList *fl) {
    if (fl->count >= fl->capacity) {
        fl->capacity *= 2;
        fl->arr = realloc(fl->arr, sizeof(Friend) * fl->capacity);
        if (!fl->arr) { perror("realloc"); exit(1); }
    }
}

int find_friend_index(FriendList *fl, const char *name) {
    for (int i = 0; i < fl->count; ++i) {
        if (strcmp(fl->arr[i].name, name) == 0) return i;
    }
    return -1;
}

void add_friend(FriendList *fl, const char *name) {
    if (find_friend_index(fl, name) != -1) {
        printf("Friend '%s' already exists.\n", name);
        return;
    }
    ensure_capacity(fl);
    strncpy(fl->arr[fl->count].name, name, NAME_LEN-1);
    fl->arr[fl->count].name[NAME_LEN-1] = '\0';
    fl->arr[fl->count].net = 0.0;
    fl->count++;
    printf("Added friend '%s'.\n", name);
}

void list_friends(FriendList *fl) {
    if (fl->count == 0) {
        printf("No friends added yet.\n");
        return;
    }
    printf("Friends:\n");
    for (int i = 0; i < fl->count; ++i) {
        printf("  %d: %s\n", i+1, fl->arr[i].name);
    }
}

void show_balances(FriendList *fl) {
    if (fl->count == 0) { printf("No friends.\n"); return; }
    printf("Current balances (positive => should receive):\n");
    for (int i = 0; i < fl->count; ++i) {
        printf("  %s : %+.2f\n", fl->arr[i].name, fl->arr[i].net);
    }
}

void add_expense(FriendList *fl) {
    if (fl->count == 0) { printf("Add friends first.\n"); return; }
    char payer[NAME_LEN];
    double amount = 0.0;
    int num_participants = 0;

    printf("Enter payer name: ");
    if (scanf("%63s", payer) != 1) { while(getchar()!='\n'); return; }
    int payer_idx = find_friend_index(fl, payer);
    if (payer_idx == -1) { printf("Payer not found.\n"); return; }

    printf("Enter total amount: ");
    if (scanf("%lf", &amount) != 1 || amount <= 0.0) { printf("Invalid amount.\n"); while(getchar()!='\n'); return; }

    printf("Split among how many participants? (enter 0 to split among ALL friends): ");
    if (scanf("%d", &num_participants) != 1) { while(getchar()!='\n'); return; }

    int *participants = NULL;
    if (num_participants == 0) {
        num_participants = fl->count;
        participants = malloc(sizeof(int) * num_participants);
        for (int i = 0; i < num_participants; ++i) participants[i] = i;
    } else {
        participants = malloc(sizeof(int) * num_participants);
        printf("Enter participant names (space/newline separated):\n");
        for (int i = 0; i < num_participants; ++i) {
            char pname[NAME_LEN];
            if (scanf("%63s", pname) != 1) { printf("Invalid input.\n"); free(participants); while(getchar()!='\n'); return; }
            int idx = find_friend_index(fl, pname);
            if (idx == -1) { printf("Participant '%s' not found. Aborting expense.\n", pname); free(participants); return; }
            participants[i] = idx;
        }
    }


    double share = amount / num_participants;


    fl->arr[payer_idx].net += amount;
    for (int i = 0; i < num_participants; ++i) {
        int idx = participants[i];
        fl->arr[idx].net -= share;
    }


    printf("Expense added: %s paid %.2f split among %d participant(s) (each %.2f).\n",
           fl->arr[payer_idx].name, amount, num_participants, share);

    free(participants);
}


void settle_debts(FriendList *fl) {
    if (fl->count == 0) { printf("No friends.\n"); return; }


    int n = fl->count;
    double *nets = malloc(sizeof(double) * n);
    int *idxs = malloc(sizeof(int) * n);
    for (int i = 0; i < n; ++i) {
        nets[i] = fl->arr[i].net;
        idxs[i] = i;
    }


    const double EPS = 0.005;

    printf("Suggested settlement transactions (minimised by greedy matching):\n");
    int steps = 0;
    while (1) {

        int cred = -1, debt = -1;
        double maxCredit = EPS, maxDebt = -EPS;
        for (int i = 0; i < n; ++i) {
            if (nets[i] > maxCredit) { maxCredit = nets[i]; cred = i; }
            if (nets[i] < maxDebt) { maxDebt = nets[i]; debt = i; }
        }
        if (cred == -1 || debt == -1) break;

        double settle = (maxCredit < -maxDebt) ? maxCredit : -maxDebt;

        printf("  %s receives %.2f from %s\n", fl->arr[idxs[cred]].name, settle, fl->arr[idxs[debt]].name);
        nets[cred] -= settle;
        nets[debt] += settle;
        steps++;
        if (steps > 10000) break;
    }

    if (steps == 0) printf("  All settled. No one owes anything.\n");

    free(nets);
    free(idxs);
}

void prompt_menu() {
    printf("\n--- Smart Bill Split App ---\n");
    printf("1) Add friend\n");
    printf("2) List friends\n");
    printf("3) Add expense\n");
    printf("4) Show balances\n");
    printf("5) Suggest settlements\n");
    printf("6) Exit\n");
    printf("Choose: ");
}

int main(void) {
    FriendList fl;
    init_list(&fl);


    int choice = 0;
    while (1) {
        prompt_menu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        while (getchar() != '\n');

        if (choice == 1) {
            char name[NAME_LEN];
            printf("Enter name: ");
            if (scanf("%63s", name) != 1) { while(getchar()!='\n'); continue; }
            add_friend(&fl, name);
            while(getchar()!='\n');
        } else if (choice == 2) {
            list_friends(&fl);
        } else if (choice == 3) {
            add_expense(&fl);
        } else if (choice == 4) {
            show_balances(&fl);
        } else if (choice == 5) {
            settle_debts(&fl);
        } else if (choice == 6) {
            printf("Exiting. Bye.\n");
            break;
        } else {
            printf("Unknown choice.\n");
        }
    }

    free(fl.arr);
    return 0;
}
