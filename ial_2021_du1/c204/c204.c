
/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

int solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {

    //Promenna pro hodnotu vrcholu zasobniku.
    char TOP;
    
    //Spusti cyklus, pokud neni zasobnik prazdny.
    while (!Stack_IsEmpty(stack))
    {
        //Zkopiruje hodnotu z vrcholu zasobniku.
        Stack_Top(stack, &TOP);

        //Odebere z vrcholu zasobniku jeden prvek.
        Stack_Pop(stack);

        //Zkontroluji hodnotu vrcholu zasobniku. Pokud je tam leva zavorka, vyskocim z cyklu.
        if (TOP == '(')
            break;

        //Zapise do vystupniho retezce hodnotu vrcholu zasobnik.
        postfixExpression[*postfixExpressionLength] = TOP;
        //Posune ukazatel na delku vystupniho retezce o jednu.
        *postfixExpressionLength += 1;
    }
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
    
    //Promenna pro hodnotu vrcholu zasobniku.
    char TOP;

    //Spusti cyklus, pokud neni zasobnik prazdny.
    while(!Stack_IsEmpty(stack)) //|| stack->topIndex == '(')
    {
        //Zkopiruje hodnotu z vrcholu zasobniku.
        Stack_Top(stack, &TOP);

        //Zkontroluji hodnotu vrcholu zasobniku. Pokud je tam leva zavorka, vyskocim z cyklu.
        if (TOP == '(')
            break;
        //Zkontroluji hodnotu vrcholu zasobniku a znak operatoru. Pokud vrchol je '+' nebo '-' a znak je '*' nebo '/', vyskocim z cyklu.
        else if ((TOP == '+' || TOP == '-') && (c == '*' || c == '/'))
            break;
        
        //Odebere z vrcholu zasobniku jeden prvek.
        Stack_Pop(stack);  

        //Zapise do vystupniho retezce hodnotu vrcholu zasobnik.
        postfixExpression[*postfixExpressionLength] = TOP;
        //Posune ukazatel na delku vystupniho retezce o jednu.
        *postfixExpressionLength += 1;
    }
    //Prida do vrcholu zasobniku operator.
    Stack_Push(stack, c);
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression Znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns Znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {

    //Vytvori proměnnou typu Stack
    Stack * isStack;

    //Alokuji pamet pro zasobnik Stack velikosti. A do promenne vrati odkaz na prvni prvek pole. 
    isStack = (Stack *) malloc(sizeof(Stack)); 

    //Inicializuji zasobnik.
    Stack_Init(isStack);

    //Alokuji pamet pro vystupni retezec.
    char *postfixExpression = malloc(sizeof(char) * MAX_LEN);

    //Zkontroluji, jestli alokovani probehlo spravne. V opacnem pripade uvolnim pamet a ukoncim funkci. 
    if (postfixExpression == NULL || isStack == NULL) {
        free(postfixExpression);
        free(isStack);
        return NULL;
    }

    //Vytvori a inicializuji promennou typu unsigned int pro delku vystupniho retezce.
    unsigned int post_count = 0;

    //Vytvori a inicializuji promennou typy char pro znakovy retezec.
    char c = *infixExpression;
    
    //Spusti cyklus, pokud hodnota neni '='.
    while (c != '=')
    {   
        //Zkontroluji hodnotu.
        if ((c >= 'a' && 'z' >= c) || 
            (c >= 'A' && 'Z' >= c) ||
            (c >= '0' && '9' >= c))
        {   
            //Prida hodnotu do vystupniho retezce.
            postfixExpression[post_count] = c;
            //Posune delku vystupniho retezce o jednu.
            post_count++;
        }
        else if (c == '+' ||
                 c == '-' ||
                 c == '*' ||
                 c == '/')
            //Zavola funkci, ktera zpracuje operátor.
            doOperation(isStack, c, postfixExpression, &post_count);

        else if (c == '(')
            //Prida do vrcholu zasobniku '('.
            Stack_Push(isStack, c);

        else if (c == ')')
            //Zavola funkci, ktera vyprazdni zasobnik az po levou zavorku.
            untilLeftPar(isStack, postfixExpression, &post_count);
        
        //Posune znak o jednu.
        c = *(++infixExpression);   
    }

    //Spusti cyklus, pokud neni zasobnik prazdny.
    while (!Stack_IsEmpty(isStack))
        {
            //Zkopiruje hodnotu z vrcholu zasobniku do vystupniho retezce.
            Stack_Top(isStack, &postfixExpression[post_count]);
            post_count++;
            Stack_Pop(isStack);
        }

    //Prida hodnotu do vystupniho retezce.
    postfixExpression[post_count] = c;
    post_count++;

    //Pridam znak pro ukonceni retezci.
    postfixExpression[post_count] = '\0';

    //Uvolni alokovane pamet.
    free(isStack);

    return postfixExpression;
}

/* Konec c204.c */
