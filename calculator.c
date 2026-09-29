#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main() {
    char expression[1000];
    printf("Enter expression: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = '\0';
    double numbers[1000];
    char operators[1000];
    int j = 0, k = 0;
    int i = 0;
    while (expression[i] != '\0') {
        if (isdigit(expression[i])) {
            double num = 0;
            while (isdigit(expression[i])) {
                num = num * 10 + (expression[i] - '0');
                i++;
            }
            numbers[j++] = num;
        }
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            operators[k++] = expression[i];
            i++;
        }
        else {
            i++;
        }
    }
    double numbers1[1000];
    char operators1[1000];
    int j1 = 0;
    int k1 = 0;
    numbers1[j1++] = numbers[0];
    for (i = 0; i < k; i++) {
        if (operators[i] == '*') {
            numbers1[j1 - 1] = numbers1[j1 - 1] * numbers[i + 1];
        }
        else if (operators[i] == '/') {
            numbers1[j1 - 1] = numbers1[j1 - 1] / numbers[i + 1];
        }
        else {
            operators1[k1++] = operators[i];
            numbers1[j1++] = numbers[i + 1];
        }
    }
    double result = numbers1[0];
    for (i = 0; i < k1; i++) {
        if (operators1[i] == '+')
            result += numbers1[i + 1];
        else if (operators1[i] == '-')
            result -= numbers1[i + 1];
    }
    printf("Result = %.2f\n", result);
    return 0;
}
