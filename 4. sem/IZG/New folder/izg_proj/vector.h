/******************************************************************************
 * Projekt - Zaklady pocitacove grafiky - IZG
 * spanel@fit.vutbr.cz
 *
 * $Id: vector.h 416 2015-03-10 08:07:46Z spanel $
 *
 * Pouziti:
 *   #include "vector.h"
 *
 *   USE_VECTOR_OF(int, ivec);
 *   #define ivecGet(pVec, i) (*ivecGetPtr((pVec), (i)))
 *
 *   ...
 *
 *   S_Vector * vec = ivecCreate(3);
 *   ivecGet(vec, 0) = 10;
 *   ivecGet(vec, 1) = 11;
 *   ivecSet(vec, 2, 12);
 *   int i = ivecPushBack(vec, 13);
 *   int size = ivecSize(vec);
 *   ivecResize(vec, 2 * size);
 *   ivecGet(vec, 4) = 14;
 *   ivecRelease(&vec);
 *
 *   int * ptr = ivecGetPtr(vec, 0);
 *   ptr[0] = 20;
 *   ptr[1] = 21;
 *   ptr[2] = 22;
 *   ptr[3] = 23;
 *
 * Opravy a modifikace:
 * -
 */

#ifndef Vector_H
#define Vector_H

/******************************************************************************
 * Includes
 */

#include "base.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * Definice typu a fci pro praci s vektorem prvku, tj. dynamickym polem
 */

/* Struktura reprezentujici dynamicke pole prvku libovolne velikosti */
typedef struct S_Vector
{
    int     size;           /* Aktualni pocet prvku ve vektoru */
    int     reserved;       /* Delka alokovane pameti */
    int     elemSize;       /* Velikost prvku vektoru v bytech */
    char    * data;         /* Ukazatel na data */
} S_Vector;


/* Vytvori a inicializuje prazdny vektor prvku zadane velikosti */
S_Vector * vecCreateEmpty(int elemSize);

/* Vytvori a inicializuje vektor prvku */
S_Vector * vecCreate(int size, int elemSize);

/* Kompletne zrusi strukturu vektoru a uvolni pamet */
void vecRelease(S_Vector ** ppVec);

/* Inicializace existujici struktury vektoru.
 * pVec - ukazatel na vektor
 * elemSize - velikost prvku vektoru v bytech */
void vecInit(S_Vector *pVec, int elemSize);

/* Zmena velikosti vektoru. Fce alokuje pamet dle potreby. */
void vecResize(S_Vector *pVec, int size);

/* Nemeni velikost vektoru, pouze si predem alokuje potrebnou pamet. */
void vecReserve(S_Vector *pVec, int size);

/* Smaze vektor a uvolni pamet. */
void vecClear(S_Vector *pVec);

/* Prida novy prvek na konec vektoru. Fce alokuje pamet dle potreby. */
/* Vraci index nove pridaneho prvku */
int vecPushBack(S_Vector *pVec, void *pElem);

/* Vraci aktualni delku vektoru */
IZG_INLINE int vecSize(S_Vector *pVec)
{
    IZG_ASSERT(pVec);
    return pVec->size;
}

/* Vrati ukazatel na i-ty prvek vektoru, indexovano od nuly. */
IZG_INLINE void * vecGetPtr(S_Vector *pVec, int i)
{
    IZG_ASSERT(pVec && i < pVec->size);
    return pVec->data + i * pVec->elemSize;
}

/* Nastavi i-ty prvek vektoru, indexovano od nuly. */
IZG_INLINE void vecSet(S_Vector *pVec, int i, void * pElem)
{
    IZG_ASSERT(pVec && i < pVec->size && pElem);
    MEMCOPY(pVec->data + i * pVec->elemSize, pElem, pVec->elemSize);
}


/******************************************************************************
 * makra pro praci s vektorem konkretnich prvku
 */

/* Makro definuje sadu funkci pro praci s vektorem konkretniho typu. */
#define USE_VECTOR_OF(type, funcpref) \
    IZG_INLINE S_Vector * funcpref##CreateEmpty() \
    { \
        return vecCreateEmpty(sizeof(type)); \
    } \
    IZG_INLINE S_Vector * funcpref##Create(int size) \
    { \
        return vecCreate(size, sizeof(type)); \
    } \
    IZG_INLINE void funcpref##Release(S_Vector **pVec) \
    { \
        vecRelease(pVec); \
    } \
    IZG_INLINE void funcpref##Init(S_Vector *pVec) \
    { \
        vecInit(pVec, sizeof(type)); \
    } \
    IZG_INLINE void funcpref##Resize(S_Vector *pVec, int size) \
    { \
        vecResize(pVec, size); \
    } \
    IZG_INLINE void funcpref##Reserve(S_Vector *pVec, int size) \
    { \
        vecReserve(pVec, size); \
    } \
    IZG_INLINE void funcpref##Clear(S_Vector *pVec) \
    { \
        vecClear(pVec); \
    } \
    IZG_INLINE int funcpref##PushBack(S_Vector *pVec, type elem) \
    { \
        return vecPushBack(pVec, &elem); \
    } \
    IZG_INLINE int funcpref##Size(S_Vector *pVec) \
    { \
        return vecSize(pVec); \
    } \
    IZG_INLINE type * funcpref##GetPtr(S_Vector *pVec, int i) \
    { \
        return (type *)vecGetPtr(pVec, i); \
    } \
    IZG_INLINE void funcpref##Set(S_Vector *pVec, int i, type elem) \
    { \
        vecSet(pVec, i, &elem); \
    }


#ifdef __cplusplus
}
#endif

#endif /* Vector_H */

/*****************************************************************************/
/*****************************************************************************/
