
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"


/*============================================
  IMPORANT NOTE

  Ambient light is represeneted by a color value

  Point light sources are 2D arrays of doubles.
       - The fist index (LOCATION) represents the vector to the light.
       - The second index (COLOR) represents the color.

  Reflection constants (ka, kd, ks) are represened as arrays of
  doubles (red, green, blue)
  ============================================*/


//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
  color i;
  i.red = calculate_ambient(alight, areflect).red + calculate_diffuse(light, dreflect, normal).red + calculate_specular(light, sreflect, view, normal).red;
  i.green = calculate_ambient(alight, areflect).green + calculate_diffuse(light, dreflect, normal).green + calculate_specular(light, sreflect, view, normal).green;
  i.blue = calculate_ambient(alight, areflect).blue + calculate_diffuse(light, dreflect, normal).blue + calculate_specular(light, sreflect, view, normal).blue;
  limit_color(&i);
  // printf("%d %d %d\n", i.red, i.green, i.blue);
  return i;
}

color calculate_ambient(color alight, double *areflect ) {
  color a;
  a.red = alight.red * areflect[RED];
  a.green = alight.green * areflect[GREEN];
  a.blue = alight.blue * areflect[BLUE];
  limit_color(&a);
  return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
  color d;
  normalize(normal);
  normalize(light[LOCATION]);
  d.red = light[COLOR][RED] * dreflect[RED] * dot_product(normal, light[LOCATION]);
  d.green = light[COLOR][GREEN] * dreflect[GREEN] * dot_product(normal, light[LOCATION]);
  d.blue = light[COLOR][BLUE] * dreflect[BLUE] * dot_product(normal, light[LOCATION]);
  limit_color(&d);
  return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
  color s;
  int n = 5;
  normalize(normal);
  normalize(light[LOCATION]);
  normalize(view);
  double cos_alpha = dot_product(subtract(multiply(2 * dot_product(normal, light[LOCATION]), normal), light[LOCATION]), view);
  s.red = light[COLOR][RED] * sreflect[RED] * pow(cos_alpha, n);
  s.green = light[COLOR][GREEN] * sreflect[GREEN] * pow(cos_alpha, n);
  s.blue = light[COLOR][BLUE] * sreflect[BLUE] * pow(cos_alpha, n);
  limit_color(&s);
  return s;
}

//limit each component of c to a max of 255
void limit_color( color * c ) {
  if (c->red >= 512){
    c->red = 0;
  }
  if (c->green > 512){
    c->green = 0;
  }
  if (c->blue > 512){
    c->blue = 0;
  }
}

//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
  double magnitude;
  magnitude = sqrt( vector[0] * vector[0] +
                    vector[1] * vector[1] +
                    vector[2] * vector[2] );
  int i;
  for (i=0; i<3; i++) {
    vector[i] = vector[i] / magnitude;
  }
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double *multiply(double a, double *b){
  double *product = (double *)malloc(3 * sizeof(double));
  product[0] = b[0] * a;
  product[1] = b[1] * a;
  product[2] = b[2] * a;
  return product;
}

double *subtract(double *a, double *b) {
  double *quotient = (double *)malloc(3 * sizeof(double));
  quotient[0] = a[0] - b[0];
  quotient[1] = a[1] - b[1];
  quotient[2] = a[2] - b[2];
  return quotient;
}


//Calculate the surface normal for the triangle whose first
//point is located at index i in polygons
double *calculate_normal(struct matrix *polygons, int i) {

  double A[3];
  double B[3];
  double *N = (double *)malloc(3 * sizeof(double));

  A[0] = polygons->m[0][i+1] - polygons->m[0][i];
  A[1] = polygons->m[1][i+1] - polygons->m[1][i];
  A[2] = polygons->m[2][i+1] - polygons->m[2][i];

  B[0] = polygons->m[0][i+2] - polygons->m[0][i];
  B[1] = polygons->m[1][i+2] - polygons->m[1][i];
  B[2] = polygons->m[2][i+2] - polygons->m[2][i];

  N[0] = A[1] * B[2] - A[2] * B[1];
  N[1] = A[2] * B[0] - A[0] * B[2];
  N[2] = A[0] * B[1] - A[1] * B[0];

  return N;
}
