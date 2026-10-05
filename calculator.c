#include <stdio.h>
#include <string.h>
#include <ctype.h>

int parsing(char expression[], int numbers[], char operators[]) {
    int j = 0;  // Variable for number array
    int k = 0;  // Varaible for operator array
    int i = 0;
    int check = 1;  // 1 means number is required and 0 means operator is required
    while (expression[i] != '\0') {
        if (isspace(expression[i])) {
            i++;
            continue;
        }
        if ((expression[i] == '+' || expression[i] == '-') && check == 1) {
            int sign = 1;   // sign checks for unary operators -> used for arithematic involving -ve numbers
            if (expression[i] == '-') {
                sign = -1;
            }
            i++;
            while (isspace(expression[i])) {
                i++;
            }
            if (!isdigit(expression[i])) {
                printf("Error: Invalid Expression.\n");
                return 0;
            }
            int num = 0;
            while (isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                i++;
            }
            numbers[j++] = sign * num;
            check = 0;
            continue;
        }
        if (isdigit(expression[i])) {
            if (check == 0) {  // check = 0 means expectation was operator but number was provided
                printf("Error: Invalid Expression.\n");
                return 0;
            }
            int num = 0;
            while (isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                i++;
            }
            numbers[j++] = num;
            check = 0;
        }
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            if (check == 1) {  // check = 1 means expectation was number but operator was provided
                printf("Error: Invalid Expression.\n");
                return 0;
            }
            operators[k++] = expression[i];
            i++;
            check = 1;
        }
        else {  // checking if expression contains invalid input
            printf("Error: Invalid expression.\n");
            return 0;
        }
    }
    if (j == 0 || check == 1) {  // j = 0 means no number was provided or expression is empty and check = 1 means last input was operator which is invalid
        printf("Error: Invalid expression.\n");
        return 0;
    }
    return j;
}

int result(int numbers[], char operators[], int count) {
    int numbers1[1000];
    char operators1[1000];
    int j1 = 0;
    int k1 = 0;
    numbers1[j1++] = numbers[0];
    // Pass 1 here we solve * and / first as they have higher precedence
    for (int i = 0; i < count; i++) {
        if (operators[i] == '*') {
            numbers1[j1 - 1] = numbers1[j1 - 1] * numbers[i + 1];
        }
        else if (operators[i] == '/') {
            if (numbers[i + 1] == 0) {
                printf("Error: Division by zero.\n");
                return 0;
            }
            numbers1[j1 - 1] = numbers1[j1 - 1] / numbers[i + 1];
        }
        else {
            operators1[k1++] = operators[i];
            numbers1[j1++] = numbers[i + 1];
        }
    }
    int answer = numbers1[0];
    // Pass 2 here we solve + and - second as they have lower precedence
    for (int i = 0; i < k1; i++) {
        if (operators1[i] == '+')
            answer += numbers1[i + 1];
        else if (operators1[i] == '-')
            answer -= numbers1[i + 1];
    }
    return answer;
}

int main() {
    char expression[1000];
    printf("Enter expression: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = '\0';
    int numbers[1000];
    char operators[1000];
    int count = parsing(expression, numbers, operators);
    int ans = result(numbers, operators, count);
    printf("Result = %d\n", ans);
    return 0;
}
