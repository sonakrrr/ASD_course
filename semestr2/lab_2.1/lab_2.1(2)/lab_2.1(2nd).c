#include <stdio.h>
#include <math.h>

//recursive return

double sum = 0;

double recursiveReturn(double x, unsigned int n) {
    double res;

    if (n == 1) {
        res = 1;
    }
    else {
        res = -recursiveReturn(x, n-1) * x * x /(4 * (n - 1) * (n - 1) - 2 * (n - 1));
    }

    sum += res;
    return res;
}

int main() {
    double x;
    int n;
    printf(" Enter x: ");
    scanf("%lf", &x);
    printf(" Enter depth: ");
    scanf("%d", &n);
    recursiveReturn(x, n);
    double func = cos(x);
    printf("\n x: %.2lf\n depth: %d\n sum: %f\n cos(%.2lf) = %lf", x, n, sum, x, func);
    return 0;
}
