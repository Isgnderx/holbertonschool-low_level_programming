#include "hash_tables.h"
#include <stdlib.h>
#include <string.h>

/**
 * hash_table_set - adds or updates an element in the hash table
 * @ht: hash table
 * @key: key (cannot be empty)
 * @value: value (must be duplicated; can be empty string)
 *
 * Return: 1 on success, 0 on failure
 */
int hash_table_set(hash_table_t *ht, const char *key, const char *value)
{
	unsigned long int idx;
	hash_node_t *node;
	char *value_dup;

	if (ht == NULL || ht->array == NULL || ht->size == 0)
		return (0);

	if (key == NULL || *key == '\0')
		return (0);

	if (value == NULL)
		return (0);

	idx = key_index((const unsigned char *)key, ht->size);

	/* check if key already exists -> update */
	node = ht->array[idx];
	while (node != NULL)
	{
		if (strcmp(node->key, key) == 0)
		{
			value_dup = strdup(value);
			if (value_dup == NULL)
				return (0);

			free(node->value);
			node->value = value_dup;
			return (1);
		}
		node = node->next;
	}

	/* key not found -> create new node at beginning */
	node = malloc(sizeof(hash_node_t));
	if (node == NULL)
		return (0);

	node->key = strdup(key);
	if (node->key == NULL)
	{
		free(node);
		return (0);
	}

	node->value = strdup(value);
	if (node->value == NULL)
	{
		free(node->key);
		free(node);
		return (0);
	}

	node->next = ht->array[idx];
	ht->array[idx] = node;

	return (1);
}

