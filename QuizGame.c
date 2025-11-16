#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum Category { GENERAL, CPROG, MATH, CS, RANDOM };

struct Question {
    char question[300];
    char A[200];
    char B[200];
    char C[200];
    char D[200];
    char correct;
    int category;
};

// Function declarations
void mainMenu();
void startQuiz(int category, int timeLimit);
void chooseCategory();
void setTimeLimit();
void showHighScore();
void saveScore(int score);
void printHeader();
void printCorrect();
void printWrong(char correct);
int askQuestion(struct Question q, int timeLimit);

// Global
int selectedCategory = GENERAL;
int globalTimeLimit = 15;

// ====== QUESTION BANK ======
struct Question qbank[] = {
    {"What is the capital of France?", "A) Paris", "B) Madrid", "C) Rome", "D) Berlin", 'A', GENERAL},
    {"Which data type stores integers in C?", "A) float", "B) int", "C) char", "D) double", 'B', CPROG},
    {"What is 12 * 8?", "A) 96", "B) 108", "C) 88", "D) 72", 'A', MATH},
    {"What does CPU stand for?", "A) Central Process Unit", "B) Central Processing Unit",
     "C) Computer Power Unit", "D) Computer Processing Utility", 'B', CS},
    {"Which symbol starts a single-line comment in C?", "A) //", "B) /*", "C) #", "D) */", 'A', CPROG},
    {"What is 15 + 27?", "A) 30", "B) 42", "C) 40", "D) 52", 'D', MATH},
    {"Who developed the C Programming Language?", "A) Dennis Ritchie", "B) Bjarne Stroustrup",
     "C) James Gosling", "D) Tim Berners-Lee", 'A', CPROG},
    {"Which company created the first computer mouse?", "A) IBM", "B) Apple", "C) Xerox", "D) Microsoft", 'C', GENERAL}
};

int qsize = sizeof(qbank) / sizeof(qbank[0]);

// ================= MAIN =================
int main() {
    mainMenu();
    return 0;
}

// ================= HEADER =================
void printHeader() {
    printf("\n=============================================\n");
    printf("               QUIZ GAME v2.0\n");
    printf("=============================================\n");
}

// ================= MAIN MENU =================
void mainMenu() {
    int choice;

    while (1) {
        printHeader();
        printf("  [1] Start Quiz\n");
        printf("  [2] Choose Category\n");
        printf("  [3] Set Time Limit\n");
        printf("  [4] High Score\n");
        printf("  [5] Exit\n");
        printf("=============================================\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                startQuiz(selectedCategory, globalTimeLimit);
                break;
            case 2:
                chooseCategory();
                break;
            case 3:
                setTimeLimit();
                break;
            case 4:
                showHighScore();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
}

// ================= CATEGORY =================
void chooseCategory() {
    int c;
    printHeader();

    printf("============== CATEGORIES ==============\n");
    printf("  [0] General Knowledge\n");
    printf("  [1] C Programming\n");
    printf("  [2] Mathematics\n");
    printf("  [3] Computer Science\n");
    printf("  [4] Random Mix\n");
    printf("========================================\n");
    printf("Select category: ");
    scanf("%d", &c);

    if (c >= 0 && c <= 4) {
        selectedCategory = c;
        printf("Category selected!\n");
    } else {
        printf("Invalid category!\n");
    }
}

// ================= TIME LIMIT =================
void setTimeLimit() {
    printHeader();
    printf("Enter time per question (seconds): ");
    scanf("%d", &globalTimeLimit);

    if (globalTimeLimit < 3) globalTimeLimit = 3;

    printf("Time limit set to %d seconds!\n", globalTimeLimit);
}

// ================= SHOW HIGH SCORE =================
void showHighScore() {
    FILE *f = fopen("highscore.txt", "r");
    int score = 0;

    if (f) {
        fscanf(f, "%d", &score);
        fclose(f);
    }

    printHeader();
    printf("High Score: %d\n", score);
}

// ================= SAVE SCORE =================
void saveScore(int score) {
    int high = 0;
    FILE *f = fopen("highscore.txt", "r");

    if (f) {
        fscanf(f, "%d", &high);
        fclose(f);
    }

    if (score > high) {
        f = fopen("highscore.txt", "w");
        fprintf(f, "%d", score);
        fclose(f);
        printf("NEW HIGH SCORE!!! 🏆\n");
    }
}

// ================= CORRECT / WRONG SCREENS =================
void printCorrect() {
    printf("---------------------------------------------\n");
    printf("                 ✅ CORRECT!\n");
    printf("---------------------------------------------\n");
}

void printWrong(char correct) {
    printf("---------------------------------------------\n");
    printf("                 ❌ WRONG!\n");
    printf("        Correct answer was: %c\n", correct);
    printf("---------------------------------------------\n");
}

// ================= ASK QUESTION =================
int askQuestion(struct Question q, int timeLimit) {
    time_t start, end;
    char answer;

    printHeader();
    printf("Category: %d\n", q.category);
    printf("Time Limit: %d seconds\n\n", timeLimit);

    printf("%s\n", q.question);
    printf("%s\n%s\n%s\n%s\n", q.A, q.B, q.C, q.D);

    printf("Your answer: ");
    start = time(NULL);
    scanf(" %c", &answer);
    end = time(NULL);

    if (end - start > timeLimit) {
        printf("\n⏳ Time is up!\n");
        return 0;
    }

    if (answer == q.correct) {
        printCorrect();
        return 10;
    } else {
        printWrong(q.correct);
        return 0;
    }
}

// ================= START QUIZ =================
void startQuiz(int category, int timeLimit) {
    int score = 0;

    for (int i = 0; i < qsize; i++) {
        if (category == RANDOM || qbank[i].category == category) {
            score += askQuestion(qbank[i], timeLimit);
        }
    }

    printHeader();
    printf("QUIZ COMPLETED!\n");
    printf("Your Score: %d\n\n", score);

    saveScore(score);
}
