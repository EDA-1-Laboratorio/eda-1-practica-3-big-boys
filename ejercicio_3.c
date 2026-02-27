#include <stdio.h>
#include <math.h>

typedef struct {
    double x;
    double y;
    double r;
} Circulo;

double areaInterseccion(Circulo c1, Circulo c2) {
    double d = sqrt((c1.x - c2.x)*(c1.x - c2.x) + (c1.y - c2.y)*(c1.y - c2.y));

    if (d >= c1.r + c2.r) return 0.0;
    if (d <= fabs(c1.r - c2.r)) {
        double rmin = c1.r < c2.r ? c1.r : c2.r;
        return M_PI * rmin * rmin;
    }

    double r1 = c1.r;
    double r2 = c2.r;

    double alfa = acos((d*d + r1*r1 - r2*r2) / (2*d*r1)) * 2;
    double beta = acos((d*d + r2*r2 - r1*r1) / (2*d*r2)) * 2;

    double area1 = 0.5 * r1*r1 * (alfa - sin(alfa));
    double area2 = 0.5 * r2*r2 * (beta - sin(beta));

    return area1 + area2;
}

int main() {
    Circulo c1, c2;

    printf("Ingresa x, y y radio del primer circulo: ");
    scanf("%lf %lf %lf", &c1.x, &c1.y, &c1.r);

    printf("Ingresa x, y y radio del segundo circulo: ");
    scanf("%lf %lf %lf", &c2.x, &c2.y, &c2.r);

    if (c1.r <= 0 || c2.r <= 0) {
        printf("Los radios deben ser positivos.\n");
        return 0;
    }

    double d = sqrt((c1.x - c2.x)*(c1.x - c2.x) + (c1.y - c2.y)*(c1.y - c2.y));

    if (d > c1.r + c2.r) {
        printf("Los circulos no se intersectan.\n");
    } else {
        printf("Los circulos SI se intersectan.\n");
        double area = areaInterseccion(c1, c2);
        printf("Area de interseccion: %.6f\n", area);
    }

    return 0;
}
