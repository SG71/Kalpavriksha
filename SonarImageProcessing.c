#include<stdio.h>

int dir[8][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, -1}, {-1, 1}, {1, 1}, {-1, -1}};

void arrayRotate90Clockwise(int *p, int n) {
    // Transpose matrix
    for(int i=0;i<n;i++) {
        for(int j=i+1;j<n;j++) {
            int temp = *(p+ i*n +j);
            *(p+ i*n +j) = *(p+ j*n +i);
            *(p+ j*n +i) = temp;
        }
    }
    // Reversing rows
    for(int i=0;i<n;i++) {
        for(int j=0;j<n/2;j++) {
            int temp = *(p+ i*n +j);
            *(p+ i*n +j) = *(p+ i*n +(n-1-j));
            *(p+ i*n +(n-1-j)) = temp;
        }
    }
}

void smoothMatrix(int *p, int n) {
    int updated[n*n];
    for (int i=0;i<n;i++) {
        for (int j=0;j<n;j++) {
            int sum = *(p + i*n + j);
            int count = 1;
            for (int k=0;k<8;k++) {
                int r = i + dir[k][0];
                int c = j + dir[k][1];
                if (r >= 0 && r < n && c >= 0 && c < n) {
                    sum += *(p + r*n + c);
                    count++;
                }
            }
            updated[i*n+j] = sum / count;
        }
    }
    for (int i=0;i<n*n;i++) {
        *(p+i) = updated[i];
    }
}

int main() {
    int n;
    printf("Enter integer value between 2 and 10 (inclusive): ");
    scanf("%d", &n);
    int matrix[n][n];
    printf("Enter %d elements of the matrix:\n", n * n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
    printf("\n");
    printf("Original : \n");
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    int *p = &matrix[0][0];
    arrayRotate90Clockwise(p, n);
    printf("\n");
    printf("Rotated : \n");
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    smoothMatrix(p, n);
    printf("\n");
    printf("Final Output : \n");
    for(int i=0;i<n;i++) {
        for(int j=0;j<n;j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    return 0;
}