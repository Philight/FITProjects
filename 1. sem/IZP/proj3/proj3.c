/* Subor:  proj3.c
 * Datum:   December 2014
 * Autor:   Truong Lai Nhat Tuan, xlainh00@stud.fit.vutbr.cz
 * Projekt: Projekt 3 - Priechod bludiskom, pre predmet IZP
 * Popis:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#define LEFTPATH 0
#define RIGHTPATH 1

#define	UPBORDER 0
#define RIGHTBORDER 1
#define	DOWNBORDER 2
#define	LEFTBORDER 3

#define BIT1 1
#define	BIT2 2
#define	BIT3 4

#define VALID 1
#define INVALID 0

/*
 * Text s napovedou
 */
const char *HELPTEXT =
		"\n"
		"Program - Priechod bludiskom\n"
		"Autor:   Truong Lai Nhat Tuan, xlainh00\n"
		"Program nacita zo vstupneho subora bludisko, po vstupe do bludiska hlada potom cestu von.\n"
		"Pouzitie:\n"
		"\t./proj3 --help zobrazi napovedu\n"
		"\t./proj3 --test bludiste.txt otestuje ci subor obsahuje spravne definovanu maticu(kazde policko musi byt definovane, susedne policka musia mat rovnaky typ hranice)\n"
		"\t\tpokial je mapa bludiska spravne definovana - vypise 'Valid', v opacnom pripade 'Invalid'\n"
		"\t./proj3 --rpath R C bludiste.txt hlada priechod bludiskom podla pravidla pravej ruky.\n"
		"\t\tVstupne parametry R(riadok) a C(stlpec) oznacuju policko v ktorom sa zacina priechod\n"
		"\t./proj3 --lpath R C bludiste.txt hlada priechod bludiskom podla pravidla lavej ruky.\n"
		"\t\tVstupne parametry R(riadok) a C(stlpec) oznacuju policko v ktorom sa zacina priechod\n"
		"\t./proj3 --shortest R C bludiste.txt hlada najkratsiu cestu z bludiska\n";
/*
 * Enum pre rozne vstupy
 */
typedef enum
{
	HELP = 1,
	TEST,
	RPATH,
	LPATH
} EArg;

/*
 * Enum pre errory/chyby
 */
typedef enum
{
	E_OK,
	E_PARAMETER,
	E_OTVORENIE_SUBORU,
	E_NEDOSTATOK_PAMATE,
	E_FORMAT_BLUDISKA,
	E_VSTUP_DO_BLUDISKA,
	E_STARTOVNE_POLICKO,
	E_NEPRECHODNA_HRANICA,
	E_LEFTRIGHT,
} EError;

/*
 * Struktura pre maticu
 */
typedef struct
{
	int rows;
	int cols;
	unsigned char *cells;
} Map;

/////////////////////////////////////////////////////////////////////////////////////////////
//-----------------------------PROTOTYPY FUNKCII------------------------------///////////////
/////////////////////////////////////////////////////////////////////////////////////////////
//--Funkcie pre manipulaciu s maticou
void matrix_allocate(Map *matrix, int rows, int cols); // alokovanie matice
void matrix_create( Map *matrix,char filename[]); // Pomocna fcia, nacita a zapisuje do suboru
void matrix_vypis(Map *matrix); // vypise maticu
void matrix_free(Map *matrix); // uvolnenie pamate

//--Pomocne funkcie
EArg save_program_arguments(int argc, char *argv[]); // ulozi parametry
EError print_error(EError code); // vypise chybu
int position_value(Map *matrix, int row, int col); // zisti hodnotu na policku

//--Hlavne funkcie pre implementaciu rieseni
int test(Map *matrix); // testuje textfile
bool isborder(Map *map, int r, int c, int border); // overuje ci je na policku stena(true)
int start_border(Map *map, int r, int c, int leftright); // zaciatocne policko
void path_start(Map *matrix, int row, int col, int leftright, char filename[]); // zahajuje hladanie cesty
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

/***************************************************************************
**************************---Hlavna funkcia main---*************************
***************************************************************************/
int main(int argc, char* argv[])
{
	Map matrix; //vytvorenie struktury

	switch(save_program_arguments(argc, argv))
	{
		case(HELP):
			printf("%s\n", HELPTEXT);
			break;
		case(TEST):
			matrix_create(&matrix,argv[2]); //prva funkcia na vytvorenie matice
			test(&matrix);
			matrix_free(&matrix);
			break;

		case(RPATH):;
			int leftright = RIGHTPATH;
			path_start(&matrix, atoi(argv[2]), atoi(argv[3]), leftright, argv[4]);
			break;

		case(LPATH):
			leftright = LEFTPATH;
			path_start(&matrix, atoi(argv[2]), atoi(argv[3]), leftright, argv[4]);
			break;
	}
	return EXIT_SUCCESS;
}

/*
 * Alokacia pamate pre Mapu bludiska
 */
void matrix_allocate(Map *matrix, int rows, int cols)
{
	matrix->rows = rows;
	matrix->cols = cols;
	matrix->cells = malloc(sizeof(unsigned char)*rows*cols); //alokuje sa pamat pre pole riadkov
	if (matrix->cells == NULL)
	{
		print_error(E_NEDOSTATOK_PAMATE);
		exit(E_NEDOSTATOK_PAMATE);
	}
}

/*
 *Otvori subor, nacita cisla a ulozi do struktury
 */
void matrix_create( Map *matrix,char filename[])
{
	FILE *filepointer = fopen(filename,"r");  //read mode

	if (filepointer == NULL) // chyba pri otvarani, ukonci program
	{
		print_error(E_OTVORENIE_SUBORU);
		exit(E_OTVORENIE_SUBORU);
	}

	int rows,cols;
	fscanf(filepointer,"%d %d", &rows, &cols); //ziskanie rozmerov matice

	matrix_allocate(matrix, rows, cols); //volanie funkcie na alokovanie pamate

	for (int i = 0; i < matrix->rows; i++) //indexacia: i=riadok, j=stlpec
		for (int j = 0; j < matrix->cols; j++) //cyklus pre zapis matice do pola
		{
			fscanf(filepointer, "%hhu", &matrix->cells[i*matrix->cols+j]); //algoritmus na indexaciu
		}

	fclose(filepointer); //zatvori subor
}

/*
 *Vypis matice na obrazovku
 */
void matrix_vypis(Map *matrix)
{
	for (int i = 0; i < matrix->rows; i++)
	{
		for (int j = 0; j < matrix->cols; j++)
				printf("%hhu",matrix->cells[i*matrix->cols+j]);
		printf("\n");
	}
}

/*
 *Uvolnenie pamate
 */
void matrix_free(Map *matrix)
{
	free(matrix->cells);
	matrix->cells = NULL;
}

/*
 * Ulozi vstup a vyhodnoti aka funkcia ma prebehnut
 */
EArg save_program_arguments(int argc, char *argv[])
{
	EArg runcode;

	if (strcmp(argv[1],"--help") == 0 && argc == 2)
		runcode = HELP;
	else if ((argc == 3) && strcmp(argv[1],"--test") == 0)
		runcode = TEST;
	else if (argc == 5) // rpath alebo lpath
	{
		if (atoi(argv[2]) < 1 || atoi(argv[3]) < 1) // Row alebo Column je < 1
		{
			print_error(E_PARAMETER);
			exit(E_PARAMETER);
		}
		if (strcmp(argv[1],"--rpath") == 0)
			runcode = RPATH;
		else if (strcmp(argv[1],"--lpath") == 0)
			runcode = LPATH;
		else
		{
			print_error(E_PARAMETER);
			exit(E_PARAMETER);
		}
	}
	else
	{
		print_error(E_PARAMETER);
		exit(E_PARAMETER);
	}
	return runcode;
}

/*
 * Vypise error podla kodu
 */
EError print_error(EError code)
{
	switch(code)
	{
		case(E_OK):
			break;
		case(E_PARAMETER):
			fprintf(stderr,"Zadali ste chybny parameter.\n");
			break;
		case(E_OTVORENIE_SUBORU):
			fprintf(stderr,"Vyskytla sa chyba pri otvarani suboru. Ukoncujem program.\n");
			break;
		case(E_NEDOSTATOK_PAMATE):
			fprintf(stderr,"Program konci kvoli nedostatku pamate.\n");
			break;
		case(E_FORMAT_BLUDISKA):
			fprintf(stderr,"Format bludiska je chybny.\n");
			break;
		case(E_VSTUP_DO_BLUDISKA):
			fprintf(stderr,"Neda sa vstupit do bludiska so zadanou poziciou.\n");
			break;
		case(E_STARTOVNE_POLICKO):
			fprintf(stderr,"Zvolili ste zle startovne policko.\n");
			break;
		case(E_NEPRECHODNA_HRANICA):
			fprintf(stderr,"Zadana hranica je neprechodna.\n");
			break;
		case(E_LEFTRIGHT):
			fprintf(stderr,"Leftright parameter je chybny.\n");
			break;
	}

	return code;
}

/*
 * Zisti hodnotu na policku(row,col)
 */
int position_value(Map *matrix, int row, int col)
{
	return matrix->cells[(row-1)*matrix->cols+(col-1)];
}

/*
 * Otestuje ci je subor platny pre projekciu matice
 */
int test(Map *matrix) //
{
	int r = 1, c = 1, validity = VALID;
//porovnavanie hranic medzi polickami
	if (matrix->rows >= 2) // matica ma minimalne 2 riadky
		for (r = 1; r <= matrix->rows; r++)
			if (matrix->cols >=2) // matica ma minimalne 2 stlpce
				for (c = 1; c < matrix->cols; c++)
				{
					if (isborder(matrix, r, c, RIGHTBORDER) != isborder(matrix, r, c+1, LEFTBORDER)) // ak maju dve policka v riadku rozdielnu hranicu
						validity = INVALID;
					if ((r >= 2) && ((r+c)%2 == 0) && (isborder(matrix, r, c, UPBORDER) != isborder(matrix, r-1, c, DOWNBORDER))) // dve policka nad a pod sebou
						validity = INVALID;
				}

	if (validity == VALID)
		printf("Valid");
	else
		printf("Invalid");
	return E_NEDOSTATOK_PAMATE;
// ine chyby su osetrene uz v samostatnych funkciach
}

/*************************************************************
 * Zisti ci je dana hranica stenou (neda sa cez nu prechadzat)
 ************************************************************/
bool isborder(Map *map, int r, int c, int border) //see #define
{
	if (border == UPBORDER && (r+c)%2 != 0) // riadok+stlpec => parne/neparne policko, neparne nema UPBORDER
		exit(E_NEPRECHODNA_HRANICA);

	if (border == DOWNBORDER && (r+c)%2 == 0) // parne policko nema DOWNBORDER
		exit(E_NEPRECHODNA_HRANICA);

	bool truefalse = false;
	switch(border)
	{
		case UPBORDER: //0
			if ((BIT3 & position_value(map,r,c) ) == BIT3)
				truefalse = true;
			break;
		case RIGHTBORDER: //1
			if ((BIT2 & position_value(map,r,c) ) == BIT2)
				truefalse = true;
			break;
		case DOWNBORDER: //2
			if ((BIT3 & position_value(map,r,c) ) == BIT3)
				truefalse = true;
			break;
		case LEFTBORDER: //3
			if ((BIT1 & position_value(map,r,c) ) == BIT1)
				truefalse = true;
			break;
		default:
			break;
	}
	if (truefalse == 1)
		printf("true\n");
	else
		printf("false\n");

	return truefalse;
}

/*****************************************************
 * Startovne policko, row a column musi byt na hranici
 *****************************************************/
int start_border(Map *map, int r, int c, int leftright)
{
	int startborder = -1;

	if (leftright == RIGHTPATH) // pravidlo pravej ruky
	{
		if (r == 1 && (r+c)%2 == 0) // prvy riadok, parne policko
		{
			if (c == 1 && isborder(map, r, c, LEFTBORDER) == 0) // [1,1]prvy stlpec, vstup zlava
				startborder = RIGHTBORDER;
			else if (c == map->cols && isborder(map, r, c, RIGHTBORDER) == 0) // [1,m->c]posledny stlpec, vstup zprava
				startborder = UPBORDER;
			else if (isborder(map, r, c, UPBORDER) == 0) // vstup zhora, priechodna hranica
				startborder = LEFTBORDER;
			else
				print_error(E_VSTUP_DO_BLUDISKA);
		}
		else if (r == map->rows && (r+c)%2 != 0) // posledny riadok, neparne policko
		{
			if (c == 1 && isborder(map, r, c, LEFTBORDER) == 0) // [m->r,1]prvy stlpec, vstup zlava
				startborder = DOWNBORDER;
			else if (c == map->cols && isborder(map, r, c, RIGHTBORDER) == 0) // [m->r,m->c]posledny stlpec, vstup zprava
				startborder = LEFTBORDER;
			else if (isborder(map, r, c, DOWNBORDER) == 0)	// vstup zdola, priechodna hranica
				startborder = RIGHTBORDER;
			else
				print_error(E_VSTUP_DO_BLUDISKA);
		}
		else if (c == 1)	// vstup zlava/prvy stlpec
		{
			if (isborder(map, r, c, LEFTBORDER) == 0)
				if ((r+c)%2 == 0)
					startborder = RIGHTBORDER;
				else
					startborder = DOWNBORDER;
			else
				print_error(E_VSTUP_DO_BLUDISKA);
		}
		else if (c == map->cols)	// vstup zprava/posledny stlpec
		{
			if (isborder(map, r, c, RIGHTBORDER) == 0)
				if ((r+c)%2 == 0)
					startborder = UPBORDER;
				else
					startborder = LEFTBORDER;
			else
				print_error(E_VSTUP_DO_BLUDISKA);
		}
		else
			exit(print_error(E_STARTOVNE_POLICKO));
	}
	else if (leftright == LEFTPATH) // pravidlo lavej ruky
	{
		if (r == 1 && (r+c)%2 == 0) // prvy riadok, parne policko
		{
			if (c == 1 && isborder(map, r, c, LEFTBORDER) == 0) // [m->r,1]prvy stlpec, vstup zlava
				startborder = UPBORDER;
			else if (c == map->cols && isborder(map, r, c, RIGHTBORDER) == 0) // [m->r,m->c]posledny stlpec, vstup zprava
				startborder = DOWNBORDER;
			else if (isborder(map, r, c, UPBORDER) == 0) // Vzstup zhora, priechodna hranica
				startborder = RIGHTBORDER;
			else
				print_error(E_VSTUP_DO_BLUDISKA);
		}
		else if (r == map->rows && (r+c)%2 != 0) // posledny riadok, neparne policko
		{
			if (c == 1 && isborder(map, r, c, LEFTBORDER) == 0) // [m->r,1]prvy stlpec, vstup zlava
				startborder = RIGHTBORDER;
			else if (c == map->cols && isborder(map, r, c, RIGHTBORDER) == 0) // [m->r,m->c]posledny stlpec, vstup zprava
				startborder = DOWNBORDER;
			else if (isborder(map, r, c, DOWNBORDER) == 0)	// vstup zdola, priechodna hranica
				startborder = LEFTBORDER;
			else
				print_error(E_VSTUP_DO_BLUDISKA);
		}
		else if (c == 1)	// vstup zlava/prvy stlpec
		{
			if (isborder(map, r, c, LEFTBORDER) == 0)
				if ((r+c)%2 == 0)
					startborder = UPBORDER;
				else
					startborder = RIGHTBORDER;
			else
				print_error(E_VSTUP_DO_BLUDISKA);
		}
		else if (c == map->cols)	// vstup zprava/posledny stlpec
		{
			if (isborder(map, r, c, RIGHTBORDER) == 0)
				if ((r+c)%2 == 0)
					startborder = LEFTBORDER;
				else
					startborder = DOWNBORDER;
			else
				print_error(E_VSTUP_DO_BLUDISKA);
		}
		else
			exit(print_error(E_STARTOVNE_POLICKO));
	}
	else
		print_error(E_LEFTRIGHT);

	switch(startborder)
		{
			case UPBORDER: //0
				printf("UPBORDER\n");
				break;
			case RIGHTBORDER: //1
				printf("RIGHTBORDER\n");
				break;
			case DOWNBORDER: //2
				printf("DOWNBORDER\n");
				break;
			case LEFTBORDER: //3
				printf("LEFTBORDER\n");
				break;
		}

	return startborder;
}

/************************************************************
 ******** Zacne hladat cestu podla pravej/lavej ruky ********
 ***********************************************************/
void path_start(Map *matrix, int row, int col, int leftright, char filename[])
{
	matrix_create(matrix, filename); //prva funkcia na vytvorenie matice
	if (row > matrix->rows || col > matrix->cols) // startovne policko je mimo bludiska
				{
					print_error(E_PARAMETER);
					exit(E_PARAMETER);
				}
	int border = start_border(matrix, row, col, leftright);

//-----------------RPATH
	if (leftright == RIGHTPATH)
		while ((1 <= row) && (row <= matrix->rows) && (1 <= col) && (col <= matrix->cols))
		{
			if (isborder(matrix, row, col, border) == 0) // volna hranica
			{
				printf("%d,%d\n",row,col); // vypise sucasne policko
				switch(border)
				{
					case UPBORDER: //0
						row--;
						border = RIGHTBORDER;
						break;

					case RIGHTBORDER: //1
						col++;
						if ((row+col)%2 == 0) // ak je sucasne policko parne(r+c)/trojuholnik s vrcholom dole
							border = RIGHTBORDER;
						else
							border = DOWNBORDER;
						break;

					case DOWNBORDER: //2
						row++;
						border = LEFTBORDER;
						break;

					case LEFTBORDER: //3
						col--;
						if ((row+col)%2 == 0) // ak je sucasne policko parne(r+c)/trojuholnik s vrcholom dole
							border = UPBORDER;
						else
							border = LEFTBORDER;
						break;
				}

			}
			else // narazi na hranicu
			{
				switch(border)
				{
					case UPBORDER:
						border = LEFTBORDER;
						break;

					case RIGHTBORDER:
						if ((row+col)%2 == 0)
							border = UPBORDER;
						else
							border = LEFTBORDER;
						break;

					case DOWNBORDER:
						border = RIGHTBORDER;
						break;

					case LEFTBORDER:
						if ((row+col)%2 == 0)
							border = RIGHTBORDER;
						else
							border = DOWNBORDER;
						break;
				}
			}
		}
//---------------LPATH
	else
		while ((1 <= row) && (row <= matrix->rows) && (1 <= col) && (col <= matrix->cols))
		{
			if (isborder(matrix, row, col, border) == 0) // volna hranica
			{
				printf("%d,%d\n",row,col); // vypise sucasne policko
				switch(border)
				{
					case UPBORDER: //0
						row--;
						border = LEFTBORDER;
					break;

					case RIGHTBORDER: //1
						col++;
						if ((row+col)%2 == 0) // ak je sucasne policko parne(r+c)/trojuholnik s vrcholom dole
							border = UPBORDER;
						else
							border = RIGHTBORDER;
						break;

					case DOWNBORDER: //2
						row++;
						border = RIGHTBORDER;
					break;

					case LEFTBORDER: //3
						col--;
						if ((row+col)%2 == 0) // ak je sucasne policko parne(r+c)/trojuholnik s vrcholom dole
							border = LEFTBORDER;
						else
							border = DOWNBORDER;
						break;
				}

			}
			else // narazi na hranicu
			{
				switch(border)
				{
					case UPBORDER:
						border = RIGHTBORDER;
						break;

					case RIGHTBORDER:
						if ((row+col)%2 == 0)
							border = LEFTBORDER;
						else
							border = DOWNBORDER;
						break;

					case DOWNBORDER:
							border = LEFTBORDER;
						break;

					case LEFTBORDER:
						if ((row+col)%2 == 0)
							border = UPBORDER;
						else
							border = RIGHTBORDER;
						break;
				}
			}
		}

	matrix_free(matrix);
}



