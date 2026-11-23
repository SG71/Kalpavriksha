#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"

#define NAME_LENGTH 51

typedef enum {
    BATSMAN = 1,
    BOWLER = 2,
    ALL_ROUNDER = 3
} Role;

typedef struct Player {
    int playerId;
    char name[NAME_LENGTH];
    char teamName[50];
    Role role;
    int totalRuns;
    double battingAverage;
    double strikeRate;
    int wickets;
    double economyRate;
    double performanceIndex;
    struct Player *next;
} Player;

typedef struct {
    int teamId;
    char name[50];
    int totalPlayers;
    double averageBattingStrikeRate;
    Player *players;
} Team;

typedef struct {
    int teamId;
    char name[50];
    int totalPlayers;
    double averageBattingStrikeRate;
} SortEntry;

Team teams[TEAM_COUNT];
int teamCount = TEAM_COUNT;

double performance(Player *player) {
    if (player->role == BATSMAN)
        return (player->battingAverage * player->strikeRate) / 100.0;
    if (player->role == BOWLER)
        return (player->wickets * 2.0) + (100.0 - player->economyRate);
    return ((player->battingAverage * player->strikeRate) / 100.0) + (player->wickets * 2.0);
}

const char *roleName(Role role) {
    if (role == BATSMAN)
        return "Batsman";
    if (role == BOWLER)
        return "Bowler";
    return "All-Rounder";
}

void update(Team *team) {
    Player *node = team->players;
    int battingCount = 0;
    team->totalPlayers = 0;
    team->averageBattingStrikeRate = 0.0;
    while (node != NULL) {
        team->totalPlayers++;
        if (node->role == BATSMAN || node->role == ALL_ROUNDER) {
            team->averageBattingStrikeRate += node->strikeRate;
            battingCount++;
        }
        node = node->next;
    }
    if (battingCount > 0)
        team->averageBattingStrikeRate /= battingCount;
}

void insert(Player **head, Player *newPlayer) {
    Player *previous = NULL;
    Player *current = *head;
    while (current != NULL && current->performanceIndex >= newPlayer->performanceIndex) {
        previous = current;
        current = current->next;
    }
    newPlayer->next = current;
    if (previous == NULL)
        *head = newPlayer;
    else
        previous->next = newPlayer;
}

void add(Team *team, Player *player) {
    player->next = NULL;
    strncpy(player->teamName, team->name, sizeof(player->teamName) - 1);
    player->teamName[sizeof(player->teamName) - 1] = '\0';
    player->performanceIndex = performance(player);
    insert(&team->players, player);
    update(team);
}

void initialize() {
    int teamIndex;
    int playerIndex;
    for (teamIndex = 0; teamIndex < TEAM_COUNT; teamIndex++) {
        teams[teamIndex].teamId = teamIndex + 1;
        strncpy(teams[teamIndex].name, TEAM_NAMES[teamIndex], sizeof(teams[teamIndex].name) - 1);
        teams[teamIndex].name[sizeof(teams[teamIndex].name) - 1] = '\0';
        teams[teamIndex].players = NULL;
        teams[teamIndex].totalPlayers = 0;
        teams[teamIndex].averageBattingStrikeRate = 0.0;
        for (playerIndex = 0; playerIndex < PLAYERS_PER_TEAM; playerIndex++) {
            Player *player = (Player *)malloc(sizeof(Player));
            player->playerId = 0;
            player->name[0] = '\0';
            player->teamName[0] = '\0';
            player->role = BATSMAN;
            player->totalRuns = 0;
            player->battingAverage = 0.0;
            player->strikeRate = 0.0;
            player->wickets = 0;
            player->economyRate = 0.0;
            player->performanceIndex = 0.0;
            player->next = NULL;
            player->playerId = 101 + teamIndex * 100 + playerIndex;
            strncpy(player->name, PLAYER_NAMES[teamIndex][playerIndex], sizeof(player->name) - 1);
            player->name[sizeof(player->name) - 1] = '\0';
            strncpy(player->teamName, TEAM_NAMES[teamIndex], sizeof(player->teamName) - 1);
            player->teamName[sizeof(player->teamName) - 1] = '\0';
            if (playerIndex % 5 == 4)
                player->role = ALL_ROUNDER;
            else if (playerIndex % 5 == 3)
                player->role = BOWLER;
            else
                player->role = BATSMAN;
            player->battingAverage = 20.0 + ((playerIndex * 7 + teamIndex * 3) % 38);
            player->strikeRate = 65.0 + ((playerIndex * 11 + teamIndex * 5) % 70);
            player->totalRuns = 500 + ((playerIndex * 137 + teamIndex * 241) % 12000);
            player->wickets = 5 + ((playerIndex * 13 + teamIndex * 7) % 300);
            player->economyRate = 3.5 + ((playerIndex * 3 + teamIndex) % 35) / 10.0;
            player->performanceIndex = performance(player);
            add(&teams[teamIndex], player);
        }
    }
}

Team *findTeamById(int teamId) {
    int low = 0;
    int high = teamCount - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (teams[mid].teamId == teamId)
            return &teams[mid];
        if (teams[mid].teamId < teamId)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return NULL;
}

Role readRole() {
    int value;
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ");
    scanf("%d", &value);
    return (Role)value;
}

void printPlayer(const Player *player) {
    printf("%-4d %-22.22s %-12s %-7d %-6.1f %-6.1f %-5d %-5.1f %.2f\n",
           player->playerId, player->name, roleName(player->role),
           player->totalRuns, player->battingAverage, player->strikeRate,
           player->wickets, player->economyRate, player->performanceIndex);
}

void addPlayer() {
    int teamId;
    printf("Enter Team ID to add player: ");
    scanf("%d", &teamId);
    Team *team = findTeamById(teamId);
    if (team == NULL) {
        printf("Team ID not found.\n");
        return;
    }
    if (team->totalPlayers >= 50) {
        printf("Team cannot have more than 50 players.\n");
        return;
    }
    Player *player = (Player *)malloc(sizeof(Player));
    if (player == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    player->playerId = 0;
    player->name[0] = '\0';
    player->teamName[0] = '\0';
    player->role = BATSMAN;
    player->totalRuns = 0;
    player->battingAverage = 0.0;
    player->strikeRate = 0.0;
    player->wickets = 0;
    player->economyRate = 0.0;
    player->performanceIndex = 0.0;
    player->next = NULL;
    printf("Enter Player Details:\n");
    printf("Player ID: ");
    scanf("%d", &player->playerId);
    printf("Name: ");
    scanf(" %50[^\n]", player->name);
    player->role = readRole();
    printf("Total Runs: ");
    scanf("%d", &player->totalRuns);
    printf("Batting Average: ");
    scanf("%lf", &player->battingAverage)
    printf("Strike Rate: ");
    scanf("%lf", &player->strikeRate);
    printf("Wickets: ");
    scanf("%d", &player->wickets);
    printf("Economy Rate: ");
    scanf("%lf", &player->economyRate);
    add(team, player);
    printf("Player added successfully to Team %s!\n", team->name);
}

void displayTeam() {
    int teamId;
    printf("Enter Team ID: ");
    scanf("%d", &teamId);
    Team *team = findTeamById(teamId);
    if (team == NULL) {
        printf("Team ID not found.\n");
        return;
    }
    printf("\nPlayers of Team %s:\n", team->name);
    printf("ID   Name                  Role         Runs    Avg    SR     Wkts ER    Perf.Index\n");
    printf("--------------------------------------------------------------------------------\n");
    Player *node = team->players;
    while (node != NULL) {
        printPlayer(node);
        node = node->next;
    }
    printf("Total Players: %d\n", team->totalPlayers);
    printf("Average Batting Strike Rate: %.2f\n", team->averageBattingStrikeRate);
}

int compareTeam(void *left, void *right) {
    const SortEntry *a = (const SortEntry *)left;
    const SortEntry *b = (const SortEntry *)right;
    if (a->averageBattingStrikeRate < b->averageBattingStrikeRate)
        return 1;
    if (a->averageBattingStrikeRate > b->averageBattingStrikeRate)
        return -1;
    return 0;
}

void displayTeams() {
    SortEntry entries[TEAM_COUNT];
    int i;
    for (i = 0; i < TEAM_COUNT; i++) {
        entries[i].teamId = teams[i].teamId;
        strncpy(entries[i].name, teams[i].name, sizeof(entries[i].name) - 1);
        entries[i].name[sizeof(entries[i].name) - 1] = '\0';
        entries[i].totalPlayers = teams[i].totalPlayers;
        entries[i].averageBattingStrikeRate = teams[i].averageBattingStrikeRate;
    }
    qsort(entries, TEAM_COUNT, sizeof(SortEntry), compareTeam);
    printf("\nTeams Sorted by Average Batting Strike Rate\n");
    printf("ID  Team Name         Avg Bat SR  Total Players\n");
    printf("------------------------------------------------\n");
    for (i = 0; i < TEAM_COUNT; i++) {
        printf("%-3d %-18s %-11.2f %d\n", entries[i].teamId,
               entries[i].name, entries[i].averageBattingStrikeRate,
               entries[i].totalPlayers);
    }
}

void displayTopK() {
    int teamId;
    printf("Enter Team ID: ");
    scanf("%d", &teamId);
    Team *team = findTeamById(teamId);
    if (team == NULL) {
        printf("Team ID not found.\n");
        return;
    }
    Role role = readRole();
    int k;
    printf("Enter number of players: ");
    scanf("%d", &k);
    if (k < 1) {
        printf("K must be at least 1.\n");
        return;
    }
    printf("\nTop %d %ss of Team %s:\n", k, roleName(role), team->name);
    printf("ID   Name                  Role         Runs    Avg    SR     Wkts ER    Perf.Index\n");
    printf("--------------------------------------------------------------------------------\n");
    Player *node = team->players;
    int printed = 0;
    while (node != NULL && printed < k) {
        if (node->role == role) {
            printPlayer(node);
            printed++;
        }
        node = node->next;
    }
    if (printed == 0)
        printf("No players with this role in the team.\n");
}

Player *mergePlayerLists(Player *first, Player *second) {
    Player dummy;
    Player *tail = &dummy;
    dummy.next = NULL;
    while (first != NULL && second != NULL) {
        if (first->performanceIndex >= second->performanceIndex) {
            tail->next = first;
            first = first->next;
        } else {
            tail->next = second;
            second = second->next;
        }
        tail = tail->next;
    }
    tail->next = (first != NULL) ? first : second;
    return dummy.next;
}

void displayAllByRole() {
    Role role = readRole();
    Player *merged = NULL;
    int i;
    for (i = 0; i < TEAM_COUNT; i++) {
        Player dummy;
        Player *roleHead = NULL;
        Player *roleTail = &dummy;
        dummy.next = NULL;
        Player *node = teams[i].players;
        while (node != NULL) {
            if (node->role == role) {
                Player *copy = (Player *)malloc(sizeof(Player));
                *copy = *node;
                copy->next = NULL;
                roleTail->next = copy;
                roleTail = copy;
            }
            node = node->next;
        }
        roleTail->next = NULL;
        roleHead = dummy.next;
        merged = mergePlayerLists(merged, roleHead);
    }
    printf("\n%s players of all teams:\n", roleName(role));
    printf("ID   Name                  Team               Role         Runs    Avg    SR     Wkts ER    Perf.Index\n");
    printf("------------------------------------------------------------------------------------------------------\n");
    Player *node = merged;
    int count = 0;
    while (node != NULL) {
        printf("%-4d %-22.22s %-18.18s %-12s %-7d %-6.1f %-6.1f %-5d %-5.1f %.2f\n",
               node->playerId, node->name, node->teamName, roleName(node->role),
               node->totalRuns, node->battingAverage, node->strikeRate,
               node->wickets, node->economyRate, node->performanceIndex);
        count++;
        node = node->next;
    }
    if (count == 0)
        printf("No players with this role found.\n");
    while (merged != NULL) {
        Player *next = merged->next;
        free(merged);
        merged = next;
    }
}

int main() {
    initialize();
    int choice;
    do {
        printf("\n=============================================================================\n");
        printf(" ICC ODI Player Performance Analyzer\n");
        printf("=============================================================================\n");
        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. Exit\n");
        printf("=============================================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                addPlayer();
                break;
            case 2:
                displayTeam();
                break;
            case 3:
                displayTeams();
                break;
            case 4:
                displayTopK();
                break;
            case 5:
                displayAllByRole();
                break;
            case 6:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 6);
    return 0;
}
