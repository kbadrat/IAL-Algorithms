/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {

  for (unsigned int i = 0; i < HT_SIZE; i++) 
  {                                    
    (*table)[i] = NULL;                
  }                                    
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) 
{
  ht_item_t *element = (*table)[get_hash(key)];

  while (element != NULL)              
  {                                  
    if (element->key == key)         
      return element;                        
    else 
      element = element->next;                 
  }                                  
  return NULL;                       
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) 
{
  ht_item_t *element = ht_search(table, key); 
  unsigned int hash = get_hash(key);       

  if (element == NULL)                                
  {                                                
    ht_item_t *created_element = malloc(sizeof(ht_item_t));
    if (created_element == NULL) 
      return;                   
    
    created_element->key = key;                            
    created_element->value = value; 
    created_element->next = (*table)[hash];
    (*table)[hash] = created_element;                                   
  }                                               
  else 
    element->value = value;                
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) 
{
  ht_item_t *element = ht_search(table, key);

  if (element != NULL)
    return &(element->value);
  else
    return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) 
{
  unsigned int hash = get_hash(key);
  ht_item_t *element = (*table)[hash];
  ht_item_t *previous_element;

  while (element != NULL)
  {
    if (element->key == key)
    {
      if (previous_element == NULL)
        (*table)[hash] = element->next;
      else
        previous_element->next = element->next;
      free(element);
    }
    else 
    {
      previous_element = element;
      element = element->next;
    }
  }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) 
{
  ht_item_t *element;
  ht_item_t *delete;

  for (unsigned int i = 0; i < HT_SIZE; i++)
  {
    element = (*table)[i];

    while (element)
    {
      delete = element;
      element = element->next;
      free(delete);
    }

    (*table)[i] = NULL;
  }
}
