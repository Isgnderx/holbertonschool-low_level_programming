#include "hash_tables.h"
#include <stdlib.h>
#include <string.h>

/**
 * update_value - updates the value of an existing key
 * @node: node to update
 * @value: new value (string)
 *
 * Return: 1 on success, 0 on failure
 */
static int update_value(hash_node_t *node, const char *value)
{
	char *value_dup;

	value_dup = strdup(value);
	if (value_dup == NULL)
		return (0);

	free(node->value);
	node->value = value_dup;

	return (1);
}

/**
 * create_node - creates a new node with duplicated key and value
 * @key: key string
 * @value: value string
 *
 * Return: pointer to new node, or NULL on failure
 */
static hash_node_t *create_node(const char *key, const char *value)
{
	hash_node_t *node;

	node = malloc(sizeof(hash_node_t));
	if (node == NULL)
		return (NULL);

	node->key = strdup(key);
	if (node->key == NULL)
	{
		free(node);
		return (NULL);
	}

	node->value = strdup(value);
	if (node->value == NULL)
	{
		free(node->key);
		free(node);
		return (NULL);
	}

	node->next = NULL;
	return (node);
}

/**
 * hash_table_set - adds or updates an element in the hash table
 * @ht: hash table
 * @key: key (cannot be empty)
 * @value: value (must be duplicated)
 *
 * Return: 1 on success, 0 on failure
 */
int hash_table_set(hash_table_t *ht, const char *key, const char *value)
{
	unsigned long int idx;
	hash_node_t *node, *new_node;

	if (ht == NULL || ht->array == NULL || ht->size == 0)
		return (0);
	if (key == NULL || *key == '\0' || value == NULL)
		return (0);

	idx = key_index((const unsigned char *)key, ht->size);

	node = ht->array[idx];
	while (node != NULL)
	{
		if (strcmp(node->key, key) == 0)
			return (update_value(node, value));
		node = node->next;
	}

	new_node = create_node(key, value);
	if (new_node == NULL)
		return (0);

	new_node->next = ht->array[idx];
	ht->array[idx] = new_node;

	return (1);
}

