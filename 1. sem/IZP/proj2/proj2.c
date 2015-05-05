
/* Subor:  proj2.c
 * Datum:   November 2014
 * Autor:   Truong Lai Nhat Tuan, xlainh00@stud.fit.vutbr.cz
 * Projekt: Projekt 2 - Iteraèní výpoèty pre predmet IZP
 * Popis:   Program
 */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/*
 * Text s napovedou
 */
const char *HELPTEXT =
		"Program - Iteracne vypocty\n"
		"Autor:   Truong Lai Nhat Tuan, xlainh00\n"
		"Program pocita vzdialenost meriaceho pristroja od daneho objekta a vysku objekta na zaklade zadanych parametrov.\n"
		"Pouzitie:\n"
		"\t./proj2 --help zobrazi napovedu\n"
		"\t./proj2 --tan A N M porovna presnost vypoctu uhloh tangens A v radianoch medzi funkciou tan, taylor_tan a cfrac_tan\n"
		"\t\tArgumenty N a M [0 < N <= M < 14] udavaju interval iteracneho vypoctu v ktorom ma porovnavanie prebiehat.\n"
		"\t./proj2 [-c X] -m A [B] zmeria vzdialenost podla uhla A(v radianoch)\n"
		"\t\tArgumenty -c X a B su volitelne, -c X nastavi vysku meriaceho pristroja na c=[0 < X <= 100]\n"
		"\t\tAk je zadany uhol B, program vypocita a vypise vysku meraneho objekta\n";

/*
 * taylor_tan - vypocet tan pomocou taylor. polynomu
 */
double taylor_tan(double x, unsigned int n) //x - uhol v radianoch, n - pocet polynomov
{
	double taylorpolynom = 0;
	double citatel[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862, 58870668456604};
	double menovatel[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
	double kvocient = 1;

	for(unsigned int i = 0; i <= n-1; i++)
	{
		taylorpolynom = taylorpolynom + citatel[i]/menovatel[i]*x*kvocient;
		kvocient = kvocient*x*x; //zvysovanie kvocienta x^
	}

	return taylorpolynom;
}

/*
 *funkcia cfrac_tan - vypocet tan pomocou zretazenych zlomkov
 */
double cfrac_tan(double x, unsigned int n)
{
	double b = 2*n - 1;
	double a;
		if (n == 1) { a = x; } //osetrenie IF n=1, jedna iteracia
		else { a = x*x; }

	double retazzlomok = a / b; //1. krok vypoctu

	for(n = n-1; n > 0; n--)
	{
		b = n*2 - 1; //b sa zmensuje podla n
		if (b == 1) { a = x; } //posledny krok, a = x

		retazzlomok = a / (b - retazzlomok);
	}
	return retazzlomok;
}

/*
 * funkcia tan z math.h
 */
double tang(double x)
{
	double tangents = tan(x);
	return tangents;
}

/*
 * funkcia absolute - vypocet absolutnych chyb
 */
double absolute(double a,double b)//b = funkcia tang
{
	double abso = (a-b); //test abs chyby taylor_tan/cfrac_tan
	if (abso < 0) { abso = -abso; }
	return abso;
}
/*
 *funkcia m - meranie dlzky
 */
double m(double x, double c)
{
	double d = c / cfrac_tan(x, 10); //tg alfa = c/d
	return d;
}

/*
 *funkcia - meranie vysky v1, c + v1 = v
 */
 double vyska1(double x, double d) //x - uhol B
 {
 	double v = tang(x)*d;
 	return v;
 }


/*
 * Hlavna funkcia main
 */
int main (int argc, char* argv[])
{
	if (argc >= 1 && argv[1] == NULL)
	{
		printf("Chybne argumenty\n");
		return EXIT_FAILURE;
	}

		if (strcmp(argv[1],"--help") == 0) //vstup do napovedy
			printf("%s/n", HELPTEXT);
/*
 *funkcia --tan, porovnavanie vypoctov tan, kontrola vstupu
 */
		else if (strcmp(argv[1],"--tan") == 0 && argc == 5 && 0 < atof(argv[2]) && atof(argv[2]) <= 1.4 && 0 < atoi(argv[3]) && atoi(argv[3]) <= atoi(argv[4]) && atoi(argv[4]) < 14)
		{
			double A = atof(argv[2]);  //uhol v rad
			int N = atoi(argv[3]);
			int M = atoi(argv[4]);
			for(; N <= M; N++)
				printf("%d %e %e %e %e %e\n", N, tang(A), taylor_tan(A,N), absolute(taylor_tan(A,N),tang(A)), cfrac_tan(A,N), absolute(cfrac_tan(A,N),tang(A)));
		}
		else if (strcmp(argv[1],"-m") == 0 && 0 < atof(argv[2]) && atof(argv[2]) <= 1.4)  //vstup funkcie -m // vypocet dlzky
		{
			double vyskac = 1.5;
			double A = atof(argv[2]);
			printf("%.10e\n", m(A, vyskac));
			if (argc == 4 && 0 < atof(argv[3]) && atof(argv[3]) <= 1.4)   //ak je zadany uhol B // vypocet vysky
			{
				double B = atof(argv[3]);
				double vyska = vyska1(B, m(A, vyskac)) + vyskac;
				printf("%.10e\n",vyska);
			}
		}
		else if (strcmp(argv[1],"-c") == 0 && 0 < atof(argv[2]) && atof(argv[2]) <= 100) // vstup parametra -c //definuje sa vlastna vyska c
		{
			double A;

			if (argc < 7 && argc >= 5 && 0 < atof(argv[4]) && atof(argv[4]) <= 1.4)  //podmienka pre nacitanie uhla A
			{
				A = atof(argv[4]);
				double vyskac = atof(argv[2]);
				printf("%.10e\n", m(A, vyskac));
			}
			else
			{
				printf("Chybne argumenty\n");
				return EXIT_FAILURE;
			}
			if (argc == 6 && 0 < atof(argv[5]) && atof(argv[5]) <= 1.4)   //ak je zadany uhol B // vypocet vysky
			{
				double vyskac = atof(argv[2]);
				double B = atof(argv[5]);
				double vyska = vyska1(B, m(A, vyskac)) + vyskac;
				printf("%.10e\n",vyska);
			}
		}
		else //argv[2] je chybny parameter
		{
			printf("Chybne argumenty\n");
			return EXIT_FAILURE;
		}



	return 0;
}
