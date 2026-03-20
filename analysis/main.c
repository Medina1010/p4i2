#include <stdio.h>
#include <math.h>

#define longitud 620

typedef struct {
	size_t count, capacity;
	float* data;
} arr_float_t;

void arr_float_free (arr_float_t* arr) {
	free(arr->data);
	free(arr);
}

void arr_float_push (arr_float_t* arr, float x) {
	if (arr->capacity == 0) {
		arr->capacity = 1;
		arr->data = realloc(arr->data, arr->capacity);
	}
	if (arr->capacity == arr->count) {
		arr->capacity *= 1.5;
		arr->data = realloc(arr->data, arr->capacity);
	}
	arr->data[arr->count++] = x;
}

int main (int argc, char** argv) {
	FILE* file = fopen(argv[1], "r");
	arr_float_t x, y, xx, yy, xy;
	char xlabel[64], ylabel[64];
	char line[128];
	fgets(line, 128, file);
	sscanf(line, "%s\t%s\n", xlabel, ylabel);
	while (fgets(line, 128, file) != NULL) {
		sscanf(line, "%f\t%f\n", &points[points_count].x, &points[points_count].y);
		points[points_count].xx = points[points_count].x*points[points_count].x;
		points[points_count].xy = points[points_count].x*points[points_count].y;
		points[points_count].yy = points[points_count].y*points[points_count].y;
		points_count++;
	}

	float X = 0, Y = 0, XY = 0, XX = 0, YY = 0;
	for (int i = 0; i < points_count; i++) {
		X += points[i].x;
		Y += points[i].y;
		XY += points[i].xy;
		XX += points[i].xx;
		YY += points[i].yy;
	}
	X /= points_count;
	Y /= points_count;
	XY /= points_count;
	XX /= points_count;
	YY /= points_count;

	float SSX = XX - X * X;
	float SSY = YY - Y * Y;
	float SXY = XY - X * Y;
	float m = (XY - X * Y) / (XX - X * X);
	float b = (XX * Y - X * XY) / (XX - X * X);
	float r = (SXY * SXY) / (SSX * SSY);
	float SSe = SSY - m * m * SSX;
	float SSm = SSe / SSX;
	float SSb = SSm * XX;

	printf(
		"####################################\n"
		"# Valores calculados para el %5s #\n"
		"####################################\n", argv[3]);
	printf("%s = (%s) * %.2f + %.2f, r = %.6f\n", ylabel, xlabel, m, b, r);
	printf("m = %.6f +- %.6f\n", m, sqrt(SSm));
	printf("b = %.4f +- %.4f\n", b, sqrt(SSb));
	printf("a = (%.3f +- %.3f) * 1e-6\n", 1000000 * m / longitud / 100, 1000000 * m / longitud / 100 * (1 / longitud + sqrt(SSm) / m));
	printf("\n");

	FILE* format_file = fopen("format.fmt", "r");
	char format[2048];
	int i = 0;
	int ch = 0;
	while ((ch = fgetc(format_file)) != -1)
		format[i++] = ch;
	format[i] = 0;
	fclose(format_file);

	FILE* output = fopen(argv[2], "w");
	fprintf(output, format,
		xlabel, ylabel,
		xlabel, ylabel, argv[1],
		points[0].x, points[points_count-1].x,
		m, b,
		ylabel, xlabel, argv[3], argv[3]
	);
	fclose(output);

	FILE* output_eq = fopen(argv[4], "w");
	fprintf(output_eq,
		"\\begin{equation}\n"
		"\\label{eq:%s}\n"
		"\\Delta l = T(%.2f\\pm %.2f)\\ \\si{\\degree C^{-1}} %.1f\\pm %.1f\n"
		"\\end{equation}\n",
		argv[3],
		m, sqrt(SSm),
		b, sqrt(SSb));
	fclose(output_eq);

	FILE* output_constante = fopen(argv[5], "w");
	fprintf(output_constante,
		"\\begin{equation}\n"
		"\\label{eq:constante %s}\n"
		"\\alpha = (%.1f\\pm %.1f)\\ \\si{\\degree C^{-1}}\n"
		"\\end{equation}\n",
		argv[3],
		10000 * m / longitud,
		10000 * m / longitud * (1 / longitud + sqrt(SSm) / m));
	fclose(output_constante);

	return 0;
}
