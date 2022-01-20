
#include "list_utils.h"

int same_list_elem(t_list *expected, t_list *result)
{
	if (expected->next == result->next && expected->content == result->content)
		return 1;
	return error("expected: {content: %p, next: %p} but got {content: %p, next: %p}\n",
			expected->content, expected->next, result->content, result->next);
}

t_list	*lstnew(void *content)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	new->content = content;
	return (new);
}

void	lstadd_front(t_list **list, t_list *new)
{
	new->next = *list;
	*list = new;
}

char *node_to_str(t_list *node)
{
	char *res = malloc(1000);
	if (node == NULL)
		sprintf(res, "(null)");
	else
		sprintf(res, "{node: %s}", escape_str((char *)node->content));
	return res;
}

char *list_to_str(t_list **head_ptr)
{
	char *str = malloc(2000);
	size_t offset = 0;
	t_list *head = *head_ptr;
	offset = sprintf(str, "<list: ");
	char *node_str;
	while (head != NULL)
	{
		node_str = node_to_str(head);
		offset += sprintf(str + offset, "%s -> ", node_str);
		free(node_str);
		head = head->next;
	}
	char *null = node_to_str(NULL);
	sprintf(str + offset, "%s>", null);
	free(null);
	return str;
}

t_list **create_list(int n_elems, ...)
{
	t_list **header_ptr = malloc(sizeof(t_list *));
	va_list argp;

	t_list	*elements[100];

	*header_ptr = NULL;
	va_start(argp, n_elems);
	for (int i = 0; i < n_elems; i++)
		elements[i] = lstnew(va_arg(argp, char *));

	for (int i = n_elems - 1; i >= 0; i--)
		lstadd_front(header_ptr, elements[i]);

	va_end(argp);
	return header_ptr;
}

int same_list(t_list **expected, t_list **result)
{
	t_list *ex_head = *expected;
	t_list *res_head = *result;

	while (ex_head && res_head && strcmp(ex_head->content, res_head->content) == 0)
	{
		ex_head = ex_head->next;
		res_head = res_head->next;
	}
	if (ex_head == NULL && res_head == NULL)
		return 1;

	char *res_list_str = list_to_str(result);
	char *expected_list_str = list_to_str(expected);
	error("different lists\n" YEL "Expected" NC ": %s\n" YEL "Result  " NC ": %s\n",
			expected_list_str, res_list_str);
	free(res_list_str); free(expected_list_str);
	return 0;
}