/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
    *tree = NULL;
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
    while (tree != NULL)
    {
        if ( key < tree->key)
            tree = tree->left;
        else if (key < tree->key)
            tree = tree->right;
        else
        {
            *value = tree->value;
            return true;
        }

    }
    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
    bst_node_t *before_node;
    bst_node_t *node;
    bst_node_t *next_node = (bst_node_t *) malloc(sizeof(struct bst_node));
    if(next_node == NULL)
        return;

    before_node = NULL;
    node = *tree;
    next_node->key = key;
    next_node->value = value;
    next_node->left = NULL;
    next_node->right = NULL;

    while ( node != NULL)
    {
        before_node = node;

        if (key < node->key )
            node = node->left;
        else
            node = node->right;
    }

    if (before_node == NULL)
        *tree = next_node;
    else if(key < before_node->key)
        before_node->left = next_node;
    else if (key > before_node->key)
        before_node->right =  next_node;
    else if (key == before_node->key)
        before_node->value = next_node->value;
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {

}


/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) 
{
    bst_node_t *node = *tree;
    bst_node_t *node_to_change;
    bst_node_t *root = NULL;
    bst_node_t *root_to_change;

    while (node != NULL && key != node->key) 
    {
        root = node;
        if (key < node->key)
            node = node->left;
        else
            node = node->right;
    }

    if (node == NULL)
        return;

    if (node->left == NULL)
        node_to_change = node->right;
    else if (node->right == NULL)
        node_to_change = node->left;
    else
    {
        root_to_change = node;
        node_to_change = node->left;

        while (node_to_change->right != NULL)
        {
            root_to_change = node_to_change;
            node_to_change = node_to_change->right;
        }

        if (node_to_change != node)
        {
            root_to_change->right = node_to_change->left;
            node_to_change->left = node->left;
        }

        node_to_change->right = node->right;
    }

    if (root == NULL)
        *tree = node_to_change;
    else if (node->key < root->key)
        root->left = node_to_change;
    else
        root->right = node_to_change;

    free(node);
}
/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) 
{
    if(*tree == NULL)
        return;

    stack_bst_t stack;
    stack_bst_init(&stack);
    stack_bst_push(&stack, *tree);

    while (stack_bst_empty( &stack) == false)
    {
        bst_node_t * delete_node = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        if (delete_node->left != NULL)
            stack_bst_push(&stack, delete_node->left);
        if (delete_node->right != NULL )
            stack_bst_push(&stack, delete_node->right);
        free(delete_node );
    }
   
     bst_init(tree);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) 
{
    while (tree)
    {
        stack_bst_push(to_visit, tree);
        bst_print_node(tree);
        tree = tree->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) 
{
    stack_bst_t stack;
    stack_bst_init(&stack);
    bst_leftmost_preorder(tree, &stack);

    while (stack_bst_empty(&stack) == false )
    {
        tree = stack_bst_top(&stack);
        stack_bst_pop(&stack);
        bst_leftmost_preorder(tree->right, &stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) 
{
    while  (tree)
    {
        stack_bst_push(to_visit, tree);
        tree = tree->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) 
{
    stack_bst_t stack;
    stack_bst_init(&stack);
    bst_leftmost_inorder(tree, &stack);

    while (stack_bst_empty(&stack) == false)
    {
        tree = stack_bst_top(&stack);
        bst_print_node(tree);
        stack_bst_pop(&stack);
        bst_leftmost_inorder(tree->right, &stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit, stack_bool_t *first_visit) 
{
    while (tree)
    {
        stack_bst_push(to_visit, tree);
        stack_bool_push(first_visit, true);
        tree = tree->left;
    }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) 
{
    stack_bst_t bst_stack;
    stack_bst_init(&bst_stack);
    stack_bool_t stack_bool;
    stack_bool_init(&stack_bool);
    bool temp;

    bst_leftmost_postorder(tree, &bst_stack, &stack_bool);
    while (stack_bst_empty(&bst_stack) == false)
    {
        tree = stack_bst_top(&bst_stack);
        stack_bst_pop(&bst_stack);
        temp = stack_bool_top(&stack_bool);
        stack_bool_pop(&stack_bool);
        if (temp == true)
        {
            stack_bool_push(&stack_bool, false);
            stack_bst_push(&bst_stack, tree);
            bst_leftmost_postorder(tree->right, &bst_stack, &stack_bool);
        }
        else
            bst_print_node(tree);
    }
}