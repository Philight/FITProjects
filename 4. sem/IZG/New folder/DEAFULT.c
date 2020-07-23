/******************************************************************************
* Projekt - Zaklady pocitacove grafiky - IZG
* spanel@fit.vutbr.cz
*
* $Id:$
*/

#include "student.h"
#include "transform.h"
#include "fragment.h"

#include <memory.h>
#include <math.h>


/*****************************************************************************
* Globalni promenne a konstanty
*/

/* Typ/ID rendereru (nemenit) */
const int           STUDENT_RENDERER = 1;

float				myClock;


/*****************************************************************************
* Funkce vytvori vas renderer a nainicializuje jej
*/

S_Renderer * studrenCreate()
{
	S_StudentRenderer * renderer = (S_StudentRenderer *)malloc(sizeof(S_StudentRenderer));
	IZG_CHECK(renderer, "Cannot allocate enough memory");

	/* inicializace default rendereru */
	renderer->base.type = STUDENT_RENDERER;
	renInit(&renderer->base);

	/* nastaveni ukazatelu na upravene funkce */
	/* napr. renderer->base.releaseFunc = studrenRelease; */
	renderer->base.releaseFunc = studrenRelease;
	renderer->base.projectTriangleFunc = studrenProjectTriangle;
	//renInit(&renderer->base);
	/* inicializace nove pridanych casti */
	S_RGBA *texture = loadBitmap(TEXTURE_FILENAME, &(renderer->tWidth), &(renderer->tHeight));
	renderer->texture = texture;

	return (S_Renderer *)renderer;
}

/*****************************************************************************
* Funkce korektne zrusi renderer a uvolni pamet
*/

void studrenRelease(S_Renderer **ppRenderer)
{
	S_StudentRenderer * renderer;

	if (ppRenderer && *ppRenderer)
	{
		/* ukazatel na studentsky renderer */
		renderer = (S_StudentRenderer *)(*ppRenderer);

		/* pripadne uvolneni pameti */
		free(renderer->texture);

		/* fce default rendereru */
		renRelease(ppRenderer);
	}
}

/******************************************************************************
* Nova fce pro rasterizaci trojuhelniku s podporou texturovani
* Upravte tak, aby se trojuhelnik kreslil s texturami
* (doplnte i potrebne parametry funkce - texturovaci souradnice, ...)
* v1, v2, v3 - ukazatele na vrcholy trojuhelniku ve 3D pred projekci
* n1, n2, n3 - ukazatele na normaly ve vrcholech ve 3D pred projekci
* x1, y1, ... - vrcholy trojuhelniku po projekci do roviny obrazovky
*/

void studrenDrawTriangle(S_Renderer *pRenderer,
	S_Coords *v1, S_Coords *v2, S_Coords *v3,
	S_Coords *n1, S_Coords *n2, S_Coords *n3,
	int x1, int y1,
	int x2, int y2,
	int x3, int y3,
	S_Coords *t
	)
{
	/* zaklad fce zkopirujte z render.c */

	int         minx, miny, maxx, maxy;
	int         a1, a2, a3, b1, b2, b3, c1, c2, c3;
	int         s1, s2, s3;
	int         x, y, e1, e2, e3;
	double      alpha, beta, gamma, w1, w2, w3, z;
	S_RGBA      col1, col2, col3, color;

	IZG_ASSERT(pRenderer && v1 && v2 && v3 && n1 && n2 && n3);

	/* vypocet barev ve vrcholech */
	col1 = pRenderer->calcReflectanceFunc(pRenderer, v1, n1);
	col2 = pRenderer->calcReflectanceFunc(pRenderer, v2, n2);
	col3 = pRenderer->calcReflectanceFunc(pRenderer, v3, n3);

	/* obalka trojuhleniku */
	minx = MIN(x1, MIN(x2, x3));
	maxx = MAX(x1, MAX(x2, x3));
	miny = MIN(y1, MIN(y2, y3));
	maxy = MAX(y1, MAX(y2, y3));

	/* oriznuti podle rozmeru okna */
	miny = MAX(miny, 0);
	maxy = MIN(maxy, pRenderer->frame_h - 1);
	minx = MAX(minx, 0);
	maxx = MIN(maxx, pRenderer->frame_w - 1);

	/* Pineduv alg. rasterizace troj.
	hranova fce je obecna rovnice primky Ax + By + C = 0
	primku prochazejici body (x1, y1) a (x2, y2) urcime jako
	(y1 - y2)x + (x2 - x1)y + x1y2 - x2y1 = 0 */

	/* normala primek - vektor kolmy k vektoru mezi dvema vrcholy, tedy (-dy, dx) */
	a1 = y1 - y2;
	a2 = y2 - y3;
	a3 = y3 - y1;
	b1 = x2 - x1;
	b2 = x3 - x2;
	b3 = x1 - x3;

	/* koeficient C */
	c1 = x1 * y2 - x2 * y1;
	c2 = x2 * y3 - x3 * y2;
	c3 = x3 * y1 - x1 * y3;

	/* vypocet hranove fce (vzdalenost od primky) pro protejsi body */
	s1 = a1 * x3 + b1 * y3 + c1;
	s2 = a2 * x1 + b2 * y1 + c2;
	s3 = a3 * x2 + b3 * y2 + c3;

	if (!s1 || !s2 || !s3)
	{
		return;
	}

	/* normalizace, aby vzdalenost od primky byla kladna uvnitr trojuhelniku */
	if (s1 < 0)
	{
		a1 *= -1;
		b1 *= -1;
		c1 *= -1;
	}
	if (s2 < 0)
	{
		a2 *= -1;
		b2 *= -1;
		c2 *= -1;
	}
	if (s3 < 0)
	{
		a3 *= -1;
		b3 *= -1;
		c3 *= -1;
	}

	/* koeficienty pro barycentricke souradnice */
	alpha = 1.0 / ABS(s2);
	beta = 1.0 / ABS(s3);
	gamma = 1.0 / ABS(s1);

	/* vyplnovani... */
	for (y = miny; y <= maxy; ++y)
	{
		/* inicilizace hranove fce v bode (minx, y) */
		e1 = a1 * minx + b1 * y + c1;
		e2 = a2 * minx + b2 * y + c2;
		e3 = a3 * minx + b3 * y + c3;

		for (x = minx; x <= maxx; ++x)
		{
			if (e1 >= 0 && e2 >= 0 && e3 >= 0)
			{
				/* interpolace pomoci barycentrickych souradnic
				e1, e2, e3 je aktualni vzdalenost bodu (x, y) od primek */
				w1 = alpha * e2;
				w2 = beta * e3;
				w3 = gamma * e1;

				/* interpolace z-souradnice */
				z = w1 * v1->z + w2 * v2->z + w3 * v3->z;
				/* interpolacia texturovych suradnic*/

				double tx = w1 * t[0].x + w2 * t[1].x + w3 * t[2].x;
				double ty = w1 * t[0].y + w2 * t[1].y + w3 * t[2].y;

				S_RGBA tColor = studrenTextureValue((S_StudentRenderer *)pRenderer, tx, ty);
				double tRed = tColor.red / 255.0;
				double tGreen = tColor.green / 255.0;
				double tBlue = tColor.blue / 255.0;
				/* interpolace barvy */
				color.red = ROUND2BYTE(w1 * col1.red + w2 * col2.red + w3 * col3.red) * tRed;
				color.green = ROUND2BYTE(w1 * col1.green + w2 * col2.green + w3 * col3.green) * tGreen;
				color.blue = ROUND2BYTE(w1 * col1.blue + w2 * col2.blue + w3 * col3.blue) * tBlue;
				color.alpha = 255;
				
				/*
				color.red = color.red * tColor.red / 255.0;
				color.green = color.red * tColor.red / 255.0;
				color.blue = color.red * tColor.red / 255.0;
				*/
				/* vykresleni bodu */
				if (z < DEPTH(pRenderer, x, y))
				{
					PIXEL(pRenderer, x, y) = color;
					DEPTH(pRenderer, x, y) = z;
				}
			}

			/* hranova fce o pixel vedle */
			e1 += a1;
			e2 += a2;
			e3 += a3;
		}
	}
}

/******************************************************************************
* Vykresli i-ty trojuhelnik n-teho klicoveho snimku modelu
* pomoci nove fce studrenDrawTriangle()
* Pred vykreslenim aplikuje na vrcholy a normaly trojuhelniku
* aktualne nastavene transformacni matice!
* Upravte tak, aby se model vykreslil interpolovane dle parametru n
* (cela cast n udava klicovy snimek, desetinna cast n parametr interpolace
* mezi snimkem n a n + 1)
* i - index trojuhelniku
* n - index klicoveho snimku (float pro pozdejsi interpolaci mezi snimky)
*/

void studrenProjectTriangle(S_Renderer *pRenderer, S_Model *pModel, int i, float n)
{
	/* zaklad fce zkopirujte z render.c */

	S_Coords    aa, bb, cc;             /* souradnice vrcholu po transformaci */
	S_Coords    naa, nbb, ncc;          /* normaly ve vrcholech po transformaci */
	S_Coords    nn;                     /* normala trojuhelniku po transformaci */
	int         u1, v1, u2, v2, u3, v3; /* souradnice vrcholu po projekci do roviny obrazovky */
	S_Triangle  * triangle;
	int         vertexOffset, normalOffset; /* offset pro vrcholy a normalove vektory trojuhelniku */
	int         i0, i1, i2, in;             /* indexy vrcholu a normaly pro i-ty trojuhelnik n-teho snimku */

	int			nextVertexOffset;
	int			nextNormalOffset;
	int			nexti0, nexti1, nexti2, nextin;

	IZG_ASSERT(pRenderer && pModel && i >= 0 && i < trivecSize(pModel->triangles) && n >= 0);

	/* z modelu si vytahneme i-ty trojuhelnik */
	triangle = trivecGetPtr(pModel->triangles, i);

	/* ziskame offset pro vrcholy n-teho snimku */
	vertexOffset = (((int)n) % pModel->frames) * pModel->verticesPerFrame;

	/* ziskame offset pro normaly trojuhelniku n-teho snimku */
	normalOffset = (((int)n) % pModel->frames) * pModel->triangles->size;

	/* ziskame offset pro vrcholy (n+1)-teho snimku */
	nextVertexOffset = (((int)n + 1) % pModel->frames) * pModel->verticesPerFrame;
	/* ziskame offset pro normaly trojuhelniku (n+1)-teho snimku */
	nextNormalOffset = (((int)n + 1) % pModel->frames) * pModel->triangles->size;

	/* indexy vrcholu pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
	i0 = triangle->v[0] + vertexOffset;
	i1 = triangle->v[1] + vertexOffset;
	i2 = triangle->v[2] + vertexOffset;

	/* indexy vrcholu pro i-ty trojuhelnik (n+1)-teho snimku - pricteni offsetu */
	nexti0 = triangle->v[0] + nextVertexOffset;
	nexti1 = triangle->v[1] + nextVertexOffset;
	nexti2 = triangle->v[2] + nextVertexOffset;

	/* index normaloveho vektoru pro i-ty trojuhelnik n-teho snimku - pricteni offsetu */
	in = triangle->n + normalOffset;

	/* index normaloveho vektoru pro i-ty trojuhelnik (n+1)-teho snimku - pricteni offsetu */
	nextin = triangle->n + nextNormalOffset;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	///===================================================================================================////
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/* desatinna cast n*/
	float floatPointN = fmodf(n, 1.0);

	/* Interpolacia vrcholov*/
	S_Coords *interpVertex0 = cvecGetPtr(pModel->vertices, i0);
	S_Coords *nextVertex0 = cvecGetPtr(pModel->vertices, nexti0);
	interpVertex0->x = (1 - floatPointN) * interpVertex0->x + floatPointN * nextVertex0->x;
	interpVertex0->y = (1 - floatPointN) * interpVertex0->y + floatPointN * nextVertex0->y;
	interpVertex0->z = (1 - floatPointN) * interpVertex0->z + floatPointN * nextVertex0->z;

	S_Coords *interpVertex1 = cvecGetPtr(pModel->vertices, i1);
	S_Coords *nextVertex1 = cvecGetPtr(pModel->vertices, nexti1);
	interpVertex1->x = (1 - floatPointN) * interpVertex1->x + floatPointN * nextVertex1->x;
	interpVertex1->y = (1 - floatPointN) * interpVertex1->y + floatPointN * nextVertex1->y;
	interpVertex1->z = (1 - floatPointN) * interpVertex1->z + floatPointN * nextVertex1->z;

	S_Coords *interpVertex2 = cvecGetPtr(pModel->vertices, i2);
	S_Coords *nextVertex2 = cvecGetPtr(pModel->vertices, nexti2);
	interpVertex2->x = (1 - floatPointN) * interpVertex2->x + floatPointN * nextVertex2->x;
	interpVertex2->y = (1 - floatPointN) * interpVertex2->y + floatPointN * nextVertex2->y;
	interpVertex2->z = (1 - floatPointN) * interpVertex2->z + floatPointN * nextVertex2->z;

	/* transformace vrcholu matici model */
	/*trTransformVertex(&aa, cvecGetPtr(pModel->vertices, i0));
	trTransformVertex(&bb, cvecGetPtr(pModel->vertices, i1));
	trTransformVertex(&cc, cvecGetPtr(pModel->vertices, i2));
	*/
	/* transformace interpolovaneho vrcholu matici model */
	trTransformVertex(&aa, interpVertex0);
	trTransformVertex(&bb, interpVertex1);
	trTransformVertex(&cc, interpVertex2);

	/* promitneme vrcholy trojuhelniku na obrazovku */
	trProjectVertex(&u1, &v1, &aa);
	trProjectVertex(&u2, &v2, &bb);
	trProjectVertex(&u3, &v3, &cc);


	S_Coords *interpNormVertex0 = cvecGetPtr(pModel->normals, i0);
	S_Coords *nextNormVertex0 = cvecGetPtr(pModel->normals, nexti0);
	interpNormVertex0->x = (1 - floatPointN) * interpNormVertex0->x + floatPointN * nextNormVertex0->x;
	interpNormVertex0->y = (1 - floatPointN) * interpNormVertex0->y + floatPointN * nextNormVertex0->y;
	interpNormVertex0->z = (1 - floatPointN) * interpNormVertex0->z + floatPointN * nextNormVertex0->z;

	S_Coords *interpNormVertex1 = cvecGetPtr(pModel->normals, i1);
	S_Coords *nextNormVertex1 = cvecGetPtr(pModel->normals, nexti1);
	interpNormVertex1->x = (1 - floatPointN) * interpNormVertex1->x + floatPointN * nextNormVertex1->x;
	interpNormVertex1->y = (1 - floatPointN) * interpNormVertex1->y + floatPointN * nextNormVertex1->y;
	interpNormVertex1->z = (1 - floatPointN) * interpNormVertex1->z + floatPointN * nextNormVertex1->z;

	S_Coords *interpNormVertex2 = cvecGetPtr(pModel->normals, i2);
	S_Coords *nextNormVertex2 = cvecGetPtr(pModel->normals, nexti2);
	interpNormVertex2->x = (1 - floatPointN) * interpNormVertex2->x + floatPointN * nextNormVertex2->x;
	interpNormVertex2->y = (1 - floatPointN) * interpNormVertex2->y + floatPointN * nextNormVertex2->y;
	interpNormVertex2->z = (1 - floatPointN) * interpNormVertex2->z + floatPointN * nextNormVertex2->z;

	/* pro osvetlovaci model transformujeme take normaly ve interpolovaanyh vrcholech */
	trTransformVector(&naa, interpNormVertex0);
	trTransformVector(&nbb, interpNormVertex1);
	trTransformVector(&ncc, interpNormVertex2);

	/* normalizace normal */
	coordsNormalize(&naa);
	coordsNormalize(&nbb);
	coordsNormalize(&ncc);

	S_Coords *interpNorm = cvecGetPtr(pModel->trinormals, in);
	S_Coords *nextNorm = cvecGetPtr(pModel->trinormals, nextin);
	interpNorm->x = (1 - floatPointN) * interpNorm->x + floatPointN * nextNorm->x;
	interpNorm->y = (1 - floatPointN) * interpNorm->y + floatPointN * nextNorm->y;
	interpNorm->z = (1 - floatPointN) * interpNorm->z + floatPointN * nextNorm->z;

	/* transformace interpolovanej normaly trojuhelniku matici model */
	trTransformVector(&nn, interpNorm);

	/* normalizace normaly */
	coordsNormalize(&nn);

	/* je troj. privraceny ke kamere, tudiz viditelny? */
	if (!renCalcVisibility(pRenderer, &aa, &nn))
	{
		/* odvracene troj. vubec nekreslime */
		return;
	}

	/* rasterizace trojuhelniku */
	studrenDrawTriangle(pRenderer,
		&aa, &bb, &cc,
		&naa, &nbb, &ncc,
		u1, v1, u2, v2, u3, v3,
		triangle->t
		);
}

/******************************************************************************
* Vraci hodnotu v aktualne nastavene texture na zadanych
* texturovacich souradnicich u, v
* Pro urceni hodnoty pouziva bilinearni interpolaci
* Pro otestovani vraci ve vychozim stavu barevnou sachovnici dle uv souradnic
* u, v - texturovaci souradnice v intervalu 0..1, ktery odpovida sirce/vysce textury
*/

S_RGBA studrenTextureValue(S_StudentRenderer * pRenderer, double u, double v)
{
	/* Vychodzi stav
	unsigned char c = ROUND2BYTE(((fmod(u * 10.0, 1.0) > 0.5) ^ (fmod(v * 10.0, 1.0) < 0.5)) * 255);
	return makeColor(c, 255 - c, 0);
	*/

// Bilinearna interpolacia
	double x = u * pRenderer->tWidth;
	double y = v * pRenderer->tHeight;

	int x1 = ROUND(x - 0.5); int x2 = ROUND(x + 0.5);
	int y1 = ROUND(y + 0.5); int y2 = ROUND(y - 0.5);
	int d = (x2 - x1) * (y2 - y1); // denominator

	S_RGBA q11, q12, q21, q22;
	q11 = pRenderer->texture[(x1 * pRenderer->tHeight) + y1];
	q12 = pRenderer->texture[(x1 * pRenderer->tHeight) + y2];

	q21 = pRenderer->texture[(x2 * pRenderer->tHeight) + y1];
	q22 = pRenderer->texture[(x2 * pRenderer->tHeight) + y2];

	S_RGBA bilinearColor;
	bilinearColor.red	=	(q11.red * (x2 - x) * (y2 - y) )/ d + (q11.red * (x2 - x) * (y - y1) / d) +
							(q11.red * (x - x1) * (y2 - y) )/ d + (q11.red * (x - x1) * (y - y1) / d);

	bilinearColor.green	=	(q11.green * (x2 - x) * (y2 - y) / d) + (q11.green * (x2 - x) * (y - y1) / d) +
							(q11.green * (x - x1) * (y2 - y) / d) + (q11.green * (x - x1) * (y - y1) / d);

	bilinearColor.blue	=	(q11.blue * (x2 - x) * (y2 - y) / d) + (q11.blue * (x2 - x) * (y - y1) / d) +
							(q11.blue * (x - x1) * (y2 - y) / d) + (q11.blue * (x - x1) * (y - y1) / d);

	bilinearColor.alpha =	(q11.alpha * (x2 - x) * (y2 - y) / d) + (q11.alpha * (x2 - x) * (y - y1) / d) +
							(q11.alpha * (x - x1) * (y2 - y) / d) + (q11.alpha * (x - x1) * (y - y1) / d);
	return bilinearColor;
}

/******************************************************************************
******************************************************************************
* Funkce pro vyrenderovani sceny, tj. vykresleni modelu
* Upravte tak, aby se model vykreslil animovane
* (volani renderModel s aktualizovanym parametrem n)
*/

void renderStudentScene(S_Renderer *pRenderer, S_Model *pModel)
{
	/* zaklad fce zkopirujte z main.c */
	/* test existence frame bufferu a modelu */
	IZG_ASSERT(pModel && pRenderer);

	/* nastavit projekcni matici */
	trProjectionPerspective(pRenderer->camera_dist, pRenderer->frame_w, pRenderer->frame_h);

	/* vycistit model matici */
	trLoadIdentity();

	/* nejprve nastavime posuv cele sceny od/ke kamere */
	trTranslate(0.0, 0.0, pRenderer->scene_move_z);

	/* nejprve nastavime posuv cele sceny v rovine XY */
	trTranslate(pRenderer->scene_move_x, pRenderer->scene_move_y, 0.0);

	/* natoceni cele sceny - jen ve dvou smerech - mys je jen 2D... :( */
	trRotateX(pRenderer->scene_rot_x);
	trRotateY(pRenderer->scene_rot_y);

	/* nastavime material */
	S_Material MAT_WHITE_AMBIENT = { 1, 1, 1, 1.0 };
	S_Material MAT_WHITE_DIFFUSE = { 1, 1, 1, 1.0 };
	S_Material MAT_WHITE_SPECULAR = { 1, 1, 1, 1.0 };

	renMatAmbient(pRenderer, &MAT_WHITE_AMBIENT);
	renMatDiffuse(pRenderer, &MAT_WHITE_DIFFUSE);
	renMatSpecular(pRenderer, &MAT_WHITE_SPECULAR);

	/* a vykreslime nas model (ve vychozim stavu kreslime pouze snimek 0) */

	renderModel(pRenderer, pModel, myClock);
}

/* Callback funkce volana pri tiknuti casovace
* ticks - pocet milisekund od inicializace */
void onTimer(int ticks)
{
	/* uprava parametru pouzivaneho pro vyber klicoveho snimku
	* a pro interpolaci mezi snimky */
	myClock = (float)ticks / 97;
}

/*****************************************************************************
*****************************************************************************/
