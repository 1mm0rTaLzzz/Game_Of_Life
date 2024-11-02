#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25

void initMatrix(int (*matrix)[WIDTH + 2]);
void printgame(int (*matrix)[WIDTH + 2]);
void generateLife(int (*matrix)[WIDTH + 2]);
int checkRule(int (*matrix)[WIDTH + 2], int i, int j);
void updateMatrix(int (*matrix)[WIDTH + 2], int (*newMatrix)[WIDTH + 2]);
void chooseFile(int (*matrix)[WIDTH + 2]);
void startGame();
void finishGame();
int kbhit();
// void inputGame(int (*matrix)[WIDTH + 2]);

int main() {
    int matrix[HEIGHT + 2][WIDTH + 2];
    int newMatrix[HEIGHT + 2][WIDTH + 2];
    int fl = 1;
    initMatrix(matrix);
    initMatrix(newMatrix);
    chooseFile(matrix);
    // inputGame(matrix);
    printf("\033[0d\033[2J");
    printgame(matrix);
    startGame();
    int time = 200000;
    while (fl) {
        updateMatrix(matrix, newMatrix);
        printgame(newMatrix);
        printf("\033[0d\033[2J");
        usleep(time);
        for (int i = 0; i < HEIGHT + 2; i++) {
            for (int j = 0; j < WIDTH + 2; j++) {
                matrix[i][j] = newMatrix[i][j];
            }
        }

        int allZero = 0;
        for (int i = 1; i <= HEIGHT; i++) {
            for (int j = 1; j <= WIDTH; j++) {
                if (matrix[i][j] != 0) {
                    allZero += 1;
                }
            }
        }

        if (allZero == 0) {
            fl = 0;
        }

        if (kbhit()) {
            int ch = getchar();
            if (ch == 'q') {
                fl = 0;
            } else if (ch == '1') {
                time = 50000;
            } else if (ch == '2') {
                time = 100000;
            } else if (ch == '3') {
                time = 600000;
            }
        }
    }
    finishGame();
    return 0;
}

void chooseFile(int (*matrix)[WIDTH + 2]) {
    int choice;
    startGame();
    printf("Choose a file to open:\n");
    printf("1. fig1.txt\n");
    printf("2. fig2.txt\n");
    printf("3. fig3.txt\n");
    printf("4. fig4.txt\n");
    printf("5. fig5.txt\n");
    printf("6. Generate random pattern\n");
    printf("Attention! If the parameter is not correct, the default value is 1. \n");
    printf("Enter your choice (1-6): ");
    scanf("%d", &choice);
    int flag = 1;
    if (choice < 1 || choice > 6) {
        printf("Invalid choice. Using default file: flower.txt\n");
        choice = 1;
    }
    if (choice == 6) {
        generateLife(matrix);

        flag = 0;
    }
    if (flag) {
        char* filenames[] = {"presets_fig/fig1.txt", "presets_fig/fig2.txt", "presets_fig/fig3.txt",
                             "presets_fig/fig4.txt", "presets_fig/fig5.txt"};
        FILE* file = fopen(filenames[choice - 1], "r");

        if (file == NULL) {
            perror("Failed to open file");
            flag = 0;
        }
        for (int i = 1; i < HEIGHT + 1 && flag; i++) {
            for (int j = 1; j < WIDTH + 1; j++) {
                fscanf(file, "%d", &matrix[i][j]);
            }
        }
        fclose(file);
    }
}

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    int flag = 0;
    if (ch != EOF) {
        ungetc(ch, stdin);
        flag = 1;
    }
    return flag;
}

int checkRule(int (*matrix)[WIDTH + 2], int i, int j) {
    int lifeCount = 0;
    for (int k = i - 1; k <= i + 1; k++) {
        for (int n = j - 1; n <= j + 1; n++) {
            int ni = k;
            int nj = n;
            if (ni < 1) ni = HEIGHT;
            if (ni > HEIGHT) ni = 1;
            if (nj < 1) nj = WIDTH;
            if (nj > WIDTH) nj = 1;
            lifeCount += matrix[ni][nj];
        }
    }
    lifeCount -= matrix[i][j];
    int result = matrix[i][j];
    if (lifeCount == 3 && matrix[i][j] == 0) {
        result = 1;
    }
    if ((lifeCount < 2 || lifeCount > 3) && matrix[i][j] == 1) {
        result = 0;
    }
    return result;
}

void generateLife(int (*matrix)[WIDTH + 2]) {
    for (int i = 1; i < HEIGHT + 1; i++) {
        for (int j = 1; j < WIDTH + 1; j++) {
            matrix[i][j] = rand() % 2;
        }
    }
}

void initMatrix(int (*matrix)[WIDTH + 2]) {
    for (int i = 0; i < HEIGHT + 2; i++) {
        for (int j = 0; j < WIDTH + 2; j++) {
            matrix[i][j] = 0;
        }
    }
}

void printgame(int (*matrix)[WIDTH + 2]) {
    for (int i = 1; i < HEIGHT + 1; i++) {
        for (int j = 1; j < WIDTH + 1; j++) {
            if (matrix[i][j] == 1) {
                printf("██");
            } else {
                printf("  ");
            }
        }
        printf("\n");
    }
}

void updateMatrix(int (*matrix)[WIDTH + 2], int (*newMatrix)[WIDTH + 2]) {
    for (int i = 1; i < HEIGHT + 1; i++) {
        for (int j = 1; j < WIDTH + 1; j++) {
            newMatrix[i][j] = checkRule(matrix, i, j);
        }
    }
}

void startGame() {
    printf("\033[0d\033[2J");
    printf("     ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ███████╗    ██╗     ██╗███████╗███████╗\n");
    printf("    ██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██╔════╝    ██║     ██║██╔════╝██╔════╝\n");
    printf("    ██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║█████╗      ██║     ██║█████╗  █████╗  \n");
    printf("    ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║██╔══╝      ██║     ██║██╔══╝  ██╔══╝  \n");
    printf("    ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝██║         ███████╗██║██║     ███████╗\n");
    printf("     ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝ ╚═╝         ╚══════╝╚═╝╚═╝     ╚══════╝\n");
    printf("                                                                                            \n");
    printf("                        Press \"1, 2, 3, 4, 5\" to change the speed                         \n");
    printf("                                                                                            \n");
}

void finishGame() {
    printf("\033[0d\033[2J");
    printf("  ██████╗  █████╗ ███╗   ███╗███████╗     ██████╗ ██╗   ██╗███████╗██████╗ ██╗\n");
    printf(" ██╔════╝ ██╔══██╗████╗ ████║██╔════╝    ██╔═══██╗██║   ██║██╔════╝██╔══██╗██║\n");
    printf(" ██║  ███╗███████║██╔████╔██║█████╗      ██║   ██║██║   ██║█████╗  ██████╔╝██║\n");
    printf(" ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝      ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗╚═╝\n");
    printf(" ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗    ╚██████╔╝ ╚████╔╝ ███████╗██║  ██║██╗\n");
    printf("  ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝     ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝╚═╝\n");
    printf("                                                                              \n");
}

// void inputGame(int (*matrix)[WIDTH + 2]) {
//     for (int i = 1; i < HEIGHT + 1; i++) {
//         for (int j = 1; j < WIDTH + 1; j++) {
//             scanf("%d", &matrix[i][j]);
//         }
//     }
// }