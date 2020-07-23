	
/* c206.c **********************************************************}
{* Téma: Dvousmìrnì vázaný lineární seznam
**
**                   Návrh a referenèní implementace: Bohuslav Køena, øíjen 2001
**                            Pøepracované do jazyka C: Martin Tuèek, øíjen 2004
**                                            Úpravy: Bohuslav Køena, øíjen 2015
**
** Implementujte abstraktní datový typ dvousmìrnì vázaný lineární seznam.
** U¾iteèným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou
** typu tDLList (DL znamená Double-Linked a slou¾í pro odli¹ení
** jmen konstant, typù a funkcí od jmen u jednosmìrnì vázaného lineárního
** seznamu). Definici konstant a typù naleznete v hlavièkovém souboru c206.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu
** s vý¹e uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ
** obousmìrnì vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DLDisposeList ... zru¹ení v¹ech prvkù seznamu,
**      DLInsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      DLInsertLast .... vlo¾ení prvku na konec seznamu, 
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek, 
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku, 
**      DLDeleteFirst ... zru¹í první prvek seznamu,
**      DLDeleteLast .... zru¹í poslední prvek seznamu, 
**      DLPostDelete .... ru¹í prvek za aktivním prvkem,
**      DLPreDelete ..... ru¹í prvek pøed aktivním prvkem, 
**      DLPostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vlo¾í nový prvek pøed aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal¹í prvek seznamu,
**      DLPred .......... posune aktivitu na pøedchozí prvek seznamu, 
**      DLActive ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci jednotlivých funkcí nevolejte ¾ádnou z funkcí
** implementovaných v rámci tohoto pøíkladu, není-li u funkce
** explicitnì uvedeno nìco jiného.
**
** Nemusíte o¹etøovat situaci, kdy místo legálního ukazatele na seznam 
** pøedá nìkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální promìnná -- pøíznak o¹etøení chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Ru¹ené prvky seznamu budou korektnì
** uvolnìny voláním operace free. 
**/
	tDLElemPtr eachElem = L->First; // nastavi ukazatel na prvy prvok v zozname
	tDLElemPtr nextElem;
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;

	while (eachElem != NULL)
	{
		nextElem = eachElem->rptr; // ukazatel na dalsi prvok
		free(eachElem);
		eachElem = nextElem; // ukazatel sa presmeruje na dalsi prvok v zozname
	}
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo¾í nový prvek na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	tDLElemPtr newElem = malloc(sizeof(struct tDLElem));
	if (newElem == NULL)
		DLError();
	else
	{
		newElem->data = val;
		newElem->lptr = NULL; // zaciatok zoznamu, ukazatel na predosly prvok vzdy NULL
		newElem->rptr = L->First; // naviazanie noveho prvku z lava na First prvok, ak First == NULL, rptr == NULL

		if (L->First == NULL)
			L->Last = newElem; // prvy prvok je zaroven aj posledny
		else
			L->First->lptr = newElem;  // naviazanie First prvku na new prvok

		L->First = newElem; // presmerovanie zaciatku zoznamu na novy prvok
	}
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo¾í nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/ 	
	tDLElemPtr newElem = malloc(sizeof(struct tDLElem));
	if (newElem == NULL)
		DLError();
	else
	{
		newElem->data = val;
		newElem->rptr = NULL;
		newElem->lptr = L->Last;

		if (L->First == NULL) // prazdny zoznam
			L->First = newElem; 
		else
			L->Last->rptr = newElem; 

		L->Last = newElem;
	}
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First;	
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;	
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->First == NULL)
		DLError();
	else
		*val = L->First->data;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
	if (L->Last == NULL)	
		DLError();
	else
		*val = L->Last->data;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru¹í první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/
	if (L->First != NULL) // pokial zoznam nie je prazdny
	{
		if (L->First == L->Act)
			L->Act = NULL;
		if (L->First == L->Last) // zoznam s iba 1 prvkom
			L->Last = NULL;

		tDLElemPtr nextElem = L->First->rptr; // zapamatam si poziciu dalsieho prvku

		L->First->rptr = NULL;
		free(L->First);
		L->First = nextElem; // presmerovanie zaciatku zoznamu na dalsi prvok
		L->First->lptr = NULL;
	}
}	

void DLDeleteLast (tDLList *L) {
/*
** Zru¹í poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/ 
	if (L->Last != NULL) // pokial zoznam nie je prazdny
	{
		if (L->Last == L->Act)
			L->Act = NULL;
		if (L->Last == L->First) // zoznam s iba 1 prvkom
			L->First = NULL;

		tDLElemPtr prevElem = L->Last->lptr; // zapamatam si poziciu predosleho prvku

		L->Last->lptr = NULL;
		free(L->Last);
		L->Last = prevElem; // presmerovanie konca zoznamu na predosly prvok
		L->Last->rptr = NULL;
	}
	
}

void DLPostDelete (tDLList *L) {
/*
** Zru¹í prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se nedìje.
**/
	if (!(L->Act == NULL || L->Act == L->Last))
	{
		tDLElemPtr nextElem = L->Act->rptr->rptr; // ukazatel na dalsi prvok v poradi po mazanom prvku (o 2 prvky dalej ako Act)
		
		if (L->Act->rptr == L->Last) // ak za Aktivnym prvkom nasleduje Last prvok
			L->Last = L->Act;		// Act prvok bude zaroven Last
		else
			nextElem->lptr = L->Act; // naviazanie z pravej strany

		free(L->Act->rptr); // zrusenie dalsieho prvku za Aktivnym 	
		L->Act->rptr = nextElem; // naviazanie Act prvku na dalsi
	}
}

void DLPreDelete (tDLList *L) {
/*
** Zru¹í prvek pøed aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se nedìje.
**/
	if (!(L->Act == NULL || L->Act == L->First))
	{
		tDLElemPtr prevElem = L->Act->lptr->lptr; // ukazatel na predosly prvok v poradi pred mazanym prvkom 
													// (o 2 prvky dalej ako Act)
		if (L->Act->lptr == L->First) // ak je pred Aktivnym prvkom First prvok
			L->First = L->Act;		// Act prvok bude zaroven First
		else
			prevElem->rptr = L->Act; // naviazanie z lavej strany

		free(L->Act->lptr); // zrusenie predosleho prvku pred Aktivnym 	
		L->Act->lptr = prevElem; // naviazanie Act prvku na predosly
	}
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	if (L->Act == NULL)
		return;

	tDLElemPtr newElem = malloc(sizeof(struct tDLElem));
	if (newElem == NULL)
		DLError();
	else
	{
		newElem->data = val;
		newElem->rptr = L->Act->rptr;
		newElem->lptr = L->Act;

		L->Act->rptr = newElem; // naviazanie noveho prvku do zoznamu
		newElem->rptr->lptr = newElem;

		if (L->Act == L->Last) // ak bol aktivny prvok poslednym, novy prvok sa stane Last
			L->Last = newElem;
	}
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek pøed aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/
	if (L->Act == NULL)
		return;

	tDLElemPtr newElem = malloc(sizeof(struct tDLElem));
	if (newElem == NULL)
		DLError();
	else
	{
		newElem->data = val;
		newElem->lptr = L->Act->lptr;
		newElem->rptr = L->Act;

		L->Act->lptr = newElem; // naviazanie noveho prvku do zoznamu
		newElem->lptr->rptr = newElem;

		if (L->Act == L->First) // ak bol aktivny prvok prvym, novy prvok sa stane First
			L->First = newElem;
	}	
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
	if (L->Act == NULL)
		DLError();
	else
		*val = L->Act->data;	
}

void DLActualize (tDLList *L, int val) {
/*
** Pøepí¹e obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic.
**/
	if (L->Act != NULL)
		L->Act->data = val;	
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na posledním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL)
		L->Act = L->Act->rptr;	
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na pøedchozí prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na prvním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL)
		L->Act = L->Act->lptr;		
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním pøíkazem return.
**/
	return (L->Act != NULL);	
}

/* Konec c206.c*/
