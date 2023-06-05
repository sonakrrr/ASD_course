#include <stdio.h>
#include <math.h>

//recursive descent

double recursiveDescent(double x, unsigned int i, unsigned int n, double segment, double sum) {
    double res;

    if (i == 1) {
        res = 1;
    }
    else {
        res = -segment * x * x / (4 * (i -1) * (i -1) - 2 * (i -1));
    }

    if (i == n) {
        sum += res;
    }
    else {
        sum += res;
        sum = recursiveDescent(x, i+1, n, res, sum);
    }

    return sum;
}

int main() {
    double x;
    int n;
    printf(" Enter x: ");
    scanf("%lf", &x);
    printf(" Enter depth: ");
    scanf("%d", &n);
    double sum = recursiveDescent(x, 1, n, 0, 0);
    double func = cos(x);
    printf("\n x: %.2lf\n depth: %d\n sum: %f\n cos(%.2lf) = %lf", x, n, sum, x, func);
    return 0;
}

