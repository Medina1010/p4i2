#include <stdio.h>

int main (int argc, char** argv) {
	FILE* file = fopen(argv[1], "r");
	struct {
		float x, y, xy, xx, yy;
	} points[32];
	int points_count = 0;
	char xlabel[64], ylabel[64];
	char line[128];
	fgets(line, 128, file);
	sscanf(line, "%s\t%s\n", xlabel, ylabel);
	while (fgets(line, 128, file) != NULL) {
		sscanf(line, "%f\t%f\n", &points[points_count].x, &points[points_count].y);
		if (points_count > 0)
			points[points_count].x -= points[0].x;
		points[points_count].xx = points[points_count].x*points[points_count].x;
		points[points_count].xy = points[points_count].x*points[points_count].y;
		points[points_count].yy = points[points_count].y*points[points_count].y;
		points_count++;
	}
		points[0].x = 0;
		points[0].xx = 0;
		points[0].xy = 0;

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

	printf("%s = (%s) * %.2f + %.2f, r = %.4f, a = %0.4f\n", ylabel, xlabel, m, b, r, 1000000.0f * m / (64000.0f));

	return 0;
}
