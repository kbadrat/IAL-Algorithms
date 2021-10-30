
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {

    //Inicializuji prvky seznamu list.
    list->activeElement = NULL;
    list->firstElement = NULL;
    list->lastElement = NULL;

}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {

    //Vytvori proměnnou pro zruseni prvku.
    DLLElementPtr kill = list->firstElement;

    //Cykl pro zruseni.
    while(list->firstElement != NULL) 
    {
        list->firstElement = list->firstElement->nextElement;
        free(kill);
        kill = list->firstElement;

    }

    //Inicializuji prvky seznamu list.
    list->activeElement = NULL;
    list->lastElement = NULL;
    

}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

    //Alokuji pamet pro novy prvek.
    DLLElementPtr new_elem = malloc(sizeof(struct DLLElement));

    //Zkontroluji, jestli alokovani probehlo spravne. V opacnem pripade zavola funkci DLL_Error.
    if (new_elem == NULL)
        DLL_Error();
    else
    {
        //Zpracuje hodnotu ke vlozeni na zacatek.
        new_elem->data = data;
        new_elem->nextElement = list->firstElement;

        //Zkontroluji, zda seznam neni prazdny a zpracuje ho.
        if (list->firstElement != NULL) 
            list->firstElement->previousElement = new_elem;
        else
            list->lastElement = new_elem;

        list->firstElement = new_elem;
    }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {

    //Alokuji pamet pro novy prvek.
    DLLElementPtr new_elem = malloc(sizeof(struct DLLElement));

    //Zkontroluji, jestli alokovani probehlo spravne. V opacnem pripade zavola funkci DLL_Error.
    if (new_elem == NULL)
        DLL_Error();
    else 
    {
        //Zpracuje hodnotu ke vlozeni na zacatek.
        new_elem->data = data;
        new_elem->previousElement = list->lastElement;
        
        //Zkontroluji, zda seznam neni prazdny a zpracuje ho.
        if (list->lastElement != NULL)
            list->lastElement->nextElement = new_elem;
        else
            list->firstElement = new_elem;

        list->lastElement = new_elem;
    }
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

    //Prvni prvek seznamu bude aktivni.
    list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {

    //Posledni prvek seznamu bude aktivni.
    list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

    //Zkontroluji, zda seznam je prazdny. V tomto pripade zavola funkci DLL_Error. 
    if (list->firstElement == NULL)
        DLL_Error();
    else
        //Zkopiruje hodnotu prvniho prvku do parametru dataPtr.
        *dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

    //Zkontroluji, zda seznam je prazdny. V tomto pripade zavola funkci DLL_Error. 
    if (list->lastElement == NULL)
        DLL_Error();
    else  
        //Zkopiruje hodnotu posledniho prvku do parametru dataPtr.
        *dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {

    //Vytvori a inicializuji proměnnou pro zruseni prvniho prvku.
    DLLElementPtr f_elem = list->firstElement;

    //Zkontroluji, zda seznam neni prazdny. 
    if (list != NULL)
    {
        //Zkontroluji, zda prvni prvek je aktivni. Pokud ano, aktivita se ztraci.
        if (f_elem == list->activeElement)
            list->activeElement = NULL;
        else 
        {   
            //Prvni prvek stane se druhym. 
            list->firstElement = f_elem->nextElement;
            list->firstElement->previousElement = NULL;
        }

    }
    //Uvolni pamet.
    free(f_elem);
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    //Vytvori a inicializuji proměnnou pro zruseni posledniho prvku.
    DLLElementPtr l_elem = list->lastElement;

    //Zkontroluji, zda seznam neni prazdny. 
    if (l_elem != NULL)
    {   
        //Zkontroluji, zda prvni prvek je aktivni. Pokud ano, aktivita se ztraci.
        if (l_elem == list->activeElement)
            list->activeElement = NULL;
        else
        {
            //Posledni prvek stane se predposlednim. 
            list->lastElement = l_elem->previousElement;
            l_elem->previousElement->nextElement = NULL;
        }
        
    }
    //Uvolni pamet.
    free(l_elem);
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek.
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu.
 */
void DLL_DeleteAfter( DLList *list ) {

    //Vytvori a inicializuji proměnnou pro zruseni prvku za aktivnim prvkem.
    DLLElementPtr a_elem = list->activeElement->nextElement;

    //Zkontroluji, zda seznam je neaktivni nebo pokud neni aktivni prvek poslednim prvkem seznamu.
    if (list == NULL || list->activeElement != list->lastElement)
    {   
        list->activeElement->nextElement = a_elem->nextElement;
        //Zkontroluji, zda prvek je posledni. Pokud ano, tak posledni prvek se stane aktivni.
        if (a_elem == list->lastElement)
            list->lastElement = list->activeElement;
        else
            //Aktivnim prvkem se stane predchozi ukazatel prvku.
            a_elem->nextElement->previousElement = list->activeElement;   
    }
    //Uvolni pamet.
    free(a_elem);
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {

    //Vytvori a inicializuji proměnnou pro zruseni prvku pred aktivnim prvkem.
    DLLElementPtr b_elem = list->activeElement->previousElement;

    //Zkontroluji, zda seznam je neaktivni nebo pokud neni aktivni prvek prvnim prvkem seznamu.
    if (list == NULL || list->activeElement != list->firstElement)
    { 
        list->activeElement->previousElement = b_elem->previousElement;
        //Zkontroluji, zda prvek je prvni. Pokud ano, tak prvni prvek se stane aktivni.
        if (b_elem == list->firstElement)
            list->firstElement = list->activeElement;
        else
            //Aktivnim prvkem se stane nasledujici ukazatel prvku.
            b_elem->previousElement->nextElement = list->activeElement;
    }
    //Uvolni pamet.
    free(b_elem);
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

    //Alokuji pamet.
    DLLElementPtr a_elem = malloc(sizeof(struct DLLElement));

    //Zkontroluji, ze alokovani probehlo uspesne.
    if (a_elem == NULL)
        DLL_Error();

    //Zkontroluji, jestli seznam neni aktivni. Pokud ano, tak vlozi prvek za aktivni prvek.
    if (list->activeElement != NULL)
    {
        //Zpracuje hodnotu ke vlozeni za aktivni prvek.
        a_elem->data = data;
        list->activeElement->nextElement->previousElement = a_elem;
    }
    //Uvolni pamet.
    free(a_elem);
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

    //Alokuji pamet.
    DLLElementPtr b_elem = malloc(sizeof(struct DLLElement));
    //Zkontroluji, ze alokovani probehlo uspesne.
    if (b_elem == NULL)
        DLL_Error();

    //Zkontroluji, jestli seznam neni aktivni. Pokud ano, tak vlozi prvek pred aktivni prvek.
    if (list->activeElement != NULL)
    {   
        //Zpracuje hodnotu ke vlozeni pred aktivni prvek.
        b_elem->data = data;
        list->activeElement->previousElement->nextElement = b_elem;
    }
    //Uvolni pamet.
    free(b_elem);
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

    //Zkontroluji, jestli seznam neni aktivni. Pokud ano, tak zavola funkci DLL_Error.
    if (list->activeElement == NULL)
        DLL_Error();
    else
        //Vrati hodnotu aktivniho prvku.
        *dataPtr = list->activeElement->data;
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {

    //Zkontroluji, jestli seznam je aktivni. Pokud ano, tak prepise obsah aktivniho prvku.
    if (list->activeElement != NULL)
        list->activeElement->data = data;
}
/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {

    //Zkontroluji, jestli seznam je aktivni. Pokud ano, tak se nasledujici prvek stane aktivni. 
    if (list->activeElement != NULL)
        list->activeElement = list->activeElement->nextElement;
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

    //Zkontroluji, jestli seznam je aktivni. Pokud ano, tak se predchozi prvek stane aktivni. 
    if(list->activeElement != NULL)
        list->activeElement = list->activeElement->previousElement;
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
    
    //Vrati aktivni prvek nebo 0.
    return list->activeElement != NULL;
}

/* Konec c206.c */
