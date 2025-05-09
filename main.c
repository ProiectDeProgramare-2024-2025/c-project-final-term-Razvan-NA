#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

typedef struct {
    char name[25];
    int score;
    char date[30];
} Player;

void banner(){
    printf("        _     _  __   __  _______      _     _  _______  __    _  _______  _______               \n");
    printf("       | | _ | ||  | |  ||       |    | | _ | ||   _   ||  |  | ||       ||       |              \n");
    printf("       | || || ||  |_|  ||   _   |    | || || ||  |_|  ||   |_| ||_     _||  _____|              \n");
    printf("       |       ||       ||  | |  |    |       ||       ||       |  |   |  | |_____               \n");
    printf("       |       ||       ||  |_|  |    |       ||       ||  _    |  |   |  |_____  |              \n");
    printf("       |   _   ||   _   ||       |    |   _   ||   _   || | |   |  |   |   _____| |              \n");
    printf("       |__| |__||__| |__||_______|    |__| |__||__| |__||_|  |__|  |___|  |_______|              \n");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("  __   __  ___   ___      ___      ___   _______  __    _  _______  ___   ______    _______  ______  \n");
    printf(" |  |_|  ||   | |   |    |   |    |   | |       ||  |  | ||   _   ||   | |    _ |  |       ||      | \n");
    printf(" |       ||   | |   |    |   |    |   | |   _   ||   |_| ||  |_|  ||   | |   | ||  |    ___||___   | \n");
    printf(" |       ||   | |   |    |   |    |   | |  | |  ||       ||       ||   | |   |_||_ |   |___   __|  | \n");
    printf(" |       ||   | |   |___ |   |___ |   | |  |_|  ||  _    ||       ||   | |    __  ||    ___| |_____| \n");
    printf(" | ||_|| ||   | |       ||       ||   | |       || | |   ||   _   ||   | |   |  | ||   |___    __    \n");
    printf(" |_|   |_||___| |_______||_______||___| |_______||_|  |__||__| |__||___| |___|  |_||_______|  |__|    \n");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void clear() {
    for (int i = 0; i <= 100; i++)
        printf("\n");
}

void displayOptions(const char* a, const char* b, const char* c, const char* d, int hide1, int hide2) {
    if (hide1 != 'a' && hide2 != 'a') printf("a) %s\n", a); else printf("a) ---\n");
    if (hide1 != 'b' && hide2 != 'b') printf("b) %s\n", b); else printf("b) ---\n");
    if (hide1 != 'c' && hide2 != 'c') printf("c) %s\n", c); else printf("c) ---\n");
    if (hide1 != 'd' && hide2 != 'd') printf("d) %s\n", d); else printf("d) ---\n");
}

int askQuestion(const char* question, const char* a, const char* b, const char* c, const char* d, char correct, int* lifelineUsed, float* points) {
    char answer[5];
    setColor(14);
    printf("\n%s\n", question);

    int hide1 = 0, hide2 = 0;

    if (!(*lifelineUsed)) {
        printf("\nType '5050' to use your 50/50 lifeline.\n");
    }

    displayOptions(a, b, c, d, 0, 0);
    setColor(7);
    printf("Your answer: ");
    scanf("%s", answer);

    if (!(*lifelineUsed) && strcmp(answer, "5050") == 0) {
        *lifelineUsed = 1;
        char options[] = {'a', 'b', 'c', 'd'};
        int count = 0;
        while (count < 2) {
            int i = rand() % 4;
            if (options[i] != correct && options[i] != hide1 && options[i] != hide2) {
                if (count == 0) hide1 = options[i];
                else hide2 = options[i];
                count++;
            }
        }
        printf("\n[50/50 LIFELINE USED]\n");
        displayOptions(a, b, c, d, hide1, hide2);
        printf("Your answer: ");
        scanf("%s", answer);
        if (tolower(answer[0]) == tolower(correct)) {
            *points += 0.5f;
        }
    } else if (tolower(answer[0]) == tolower(correct)) {
        *points += 1.0f;
    }

    if (tolower(answer[0]) == tolower(correct)) {
        clear();
        banner();
        setColor(10);
        printf("Correct!\n");
        setColor(7);
        return 1;
    } else {
        clear();
        banner();
        setColor(12);
        printf("Incorrect! The correct answer was '%c'.\n", correct);
        setColor(7);
        return 0;
    }
}

void howToPlay() {
    clear();
    printf("===== How to Play =====\n");
    printf("Answer the multiple-choice questions by typing a, b, c, or d.\n");
    printf("Each correct answer gives you a point.\n");
    printf("If you use 50/50 lifeline and get it right, you get only 0.5 points.\n");
    printf("You have one 50/50 lifeline per game. Type '5050' to use it.\n");
    printf("Game ends immediately if you answer incorrectly.\n");
    printf("=======================\n");
    printf("Press any key to return to menu...\n");
    getchar(); getchar();
}

void leaderboard() {
    clear();
    printf("===== Leaderboard (local only) =====\n");

    FILE* f = fopen("leaderboard.txt", "r");
    if (!f) {
        printf("No leaderboard data available.\n");
        printf("=======================\n");
        printf("Press any key to return to menu...\n");
        getchar(); getchar();
        return;
    }

    Player players[100];
    int count = 0;
    char line[100];

    while (fgets(line, sizeof(line), f) && count < 100) {
        if (sscanf(line, "%24s - %d/10 - %29[^\n]", players[count].name, &players[count].score, players[count].date) == 3) {
            count++;
        }
    }
    fclose(f);

    // Update to keep the best score for each player
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(players[i].name, players[j].name) == 0) {
                if (players[j].score > players[i].score) {
                    players[i].score = players[j].score;
                    strcpy(players[i].date, players[j].date);
                }
                for (int k = j; k < count - 1; k++) {
                    players[k] = players[k + 1];
                }
                count--;
                j--;
            }
        }
    }

    // Sort players by score
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (players[j].score > players[i].score) {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    // Display leaderboard
    for (int i = 0; i < count; i++) {
        printf("%s - %d/10 - %s\n", players[i].name, players[i].score, players[i].date);
    }

    printf("=======================\n");
    printf("Press any key to return to menu...\n");
    getchar(); getchar();
}

void history() {
    clear();
    char query[25];
    printf("Enter the name to view game history: ");
    scanf("%24s", query);

    FILE* f = fopen("history.txt", "r");
    if (!f) {
        printf("No history data available.\n");
        printf("Press any key to return to menu...\n");
        getchar(); getchar();
        return;
    }

    char line[100];
    int found = 0;

    while (fgets(line, sizeof(line), f)) {
        char name[25], date[30];
        int score;
        if (sscanf(line, "%24s - %d/10 - %29[^\n]", name, &score, date) == 3) {
            if (strcmp(name, query) == 0) {
                printf("%s - %d/10 - %s\n", name, score, date);
                found = 1;
            }
        }
    }
    fclose(f);

    if (!found) printf("No history found for that name.\n");

    printf("Press any key to return to menu...\n");
    getchar(); getchar();
}

int main() {
    srand(time(NULL));
    Player player;
    char answer[10];
    int choice;

    while (1) {
        clear();
        banner();
        printf("1. Play\n2. How to Play\n3. Leaderboard\n4. View History\nChoose an option: ");
        scanf("%d", &choice);

        if (choice == 2) {
            howToPlay();
        } else if (choice == 3) {
            leaderboard();
        } else if (choice == 4) {
            history();
        } else if (choice == 1) {
            clear();
            banner();
            printf("Welcome to Who Wants to Be a Millionaire! What is your name?\n");
            scanf("%24s", player.name);
            printf("Nice to meet you %s! Let's get started.\n", player.name);

            player.score = 0;
            float points = 0;
            int lifelineUsed = 0;

            if (!askQuestion("1. What is the capital of France?", "Berlin", "Madrid", "Paris", "Rome", 'c', &lifelineUsed, &points)) goto endGame;
            if (!askQuestion("2. Which planet is known as the Red Planet?", "Earth", "Mars", "Jupiter", "Saturn", 'b', &lifelineUsed, &points)) goto endGame;
            if (!askQuestion("3. Who wrote 'Romeo and Juliet'?", "Dickens", "Shakespeare", "Hemingway", "Poe", 'b', &lifelineUsed, &points)) goto endGame;
            if (!askQuestion("4. What is the largest mammal?", "Elephant", "Blue Whale", "Shark", "Giraffe", 'b', &lifelineUsed, &points)) goto endGame;
            if (!askQuestion("5. What gas do plants breathe in?", "Oxygen", "Nitrogen", "Carbon Dioxide", "Hydrogen", 'c', &lifelineUsed, &points)) goto endGame;
            if (!askQuestion("6. How many continents are there?", "5", "6", "7", "8", 'c', &lifelineUsed, &points)) goto endGame;
            if (!askQuestion("7. What is H2O commonly known as?", "Hydrogen", "Water", "Oxygen", "Salt", 'b', &lifelineUsed, &points)) goto endGame;
            if (!askQuestion("8. Which ocean is the largest?", "Atlantic", "Indian", "Pacific", "Arctic", 'c', &lifelineUsed, &points)) goto endGame;
            if (!askQuestion("9. Who painted the Mona Lisa?", "Van Gogh", "Picasso", "Da Vinci", "Michelangelo", 'c', &lifelineUsed, &points)) goto endGame;
            if (!askQuestion("10. Which language is primarily spoken in Brazil?", "Spanish", "Portuguese", "French", "English", 'b', &lifelineUsed, &points)) goto endGame;

endGame:
            player.score = (int)points;
            time_t t = time(NULL);
            struct tm* tm_info = localtime(&t);
            char timeStr[30];
            strftime(timeStr, 30, "%Y-%m-%d %H:%M:%S", tm_info);

            clear();
            banner();
            printf("\nNice try %s! You scored ", player.name);
            setColor(11);
            printf("%.1f points \n", points);
            setColor(7);

            printf("Would you like to be added to the leaderboard? (yes/no): ");
            scanf("%9s", answer);
            if (strcmp(answer, "yes") == 0 || strcmp(answer, "Yes") == 0) {
                FILE* f = fopen("leaderboard.txt", "a+");
                FILE* h = fopen("history.txt", "a");
                if (f && h) {
                    // Check for existing name, update if better score
                    Player existingPlayer;
                    int updated = 0;
                    while (fscanf(f, "%24s - %d/10 - %29[^\n]", existingPlayer.name, &existingPlayer.score, existingPlayer.date) == 3) {
                        if (strcmp(existingPlayer.name, player.name) == 0 && existingPlayer.score < player.score) {
                            fseek(f, -(strlen(existingPlayer.name) + strlen(existingPlayer.date) + 25), SEEK_CUR);
                            fprintf(f, "%s - %d/10 - %s\n", player.name, player.score, timeStr);
                            updated = 1;
                            break;
                        }
                    }
                    if (!updated) {
                        fprintf(f, "%s - %d/10 - %s\n", player.name, player.score, timeStr);
                    }
                    fprintf(h, "%s - %d/10 - %s\n", player.name, player.score, timeStr);
                    fclose(f);
                    fclose(h);
                }
            }
            printf("Press any key to return to menu...\n");
            getchar(); getchar();
        }
    }
}
