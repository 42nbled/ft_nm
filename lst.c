#include "ft_nm.h"

t_lst *lst_last(t_lst *node)
{
	t_lst	*last = node;
	while (last && last->next)
		last = last->next;
	return last;
}

void	lst_append(t_lst *root, t_name_table *table)
{
	t_lst *last = lst_last(root);
	last->next = malloc(sizeof(t_lst));
	if (!last->next)
		assert(0 && "TODO: free all and exit");
	last->next->data = table;
	last->next->next = NULL;
}

void	lst_clear(t_lst *root)
{
	t_lst	*ptr = root;
	t_lst	*tmp;
	while (ptr)
	{
		tmp = ptr->next;
		free(ptr->data->name);
		free(ptr->data);
		free(ptr);
		ptr = tmp;
	}
}

int	lst_size(t_lst *node)
{
	if (node->next)
		return (lst_size(node->next) + 1);
	return (1);
}

int	diff_str(t_lst *a, t_lst *b) {
	if (!a || !a->data)
		return 0;
	if (!b || !b->data)
		return 0;
	int status = strcmp(a->data->name, b->data->name);
	if (!status)
		return (a->data->i_value - b->data->i_value);
	return (status);
}

int	reverse_diff_str(t_lst *b, t_lst *a) {
	if (!a || !a->data)
		return 0;
	if (!b || !b->data)
		return 0;
	int status = strcmp(a->data->name, b->data->name);
	if (!status)
		return (b->data->i_value - a->data->i_value);
	return (status);
}

void ft_swap(t_lst **a, t_lst **b) {
	t_lst *c = *a;
	*a = *b;
	*b = c;
}

void ft_lst_swap(t_lst **header, t_lst *a, t_lst *b) {
	if (a == b)
		return;
	if (a == NULL || b == NULL)
		return;

	t_lst **first = header;
	while (*first && *first != a)
		first = &(*first)->next;

	t_lst **second = header;
	while (*second && *second != b)
		second = &(*second)->next;

	ft_swap(first, second);
	ft_swap(&(*first)->next, &(*second)->next);
}

void	ft_lst_sort(t_lst **begin_list, int (*cmp)())
{
	if (!begin_list)
		return;
	if (!*begin_list)
		return;

	t_lst *a;
	t_lst *b;
	t_lst *c;
	t_lst *d;
	a = *begin_list;

	while (a && a->next)
	{
		b = a->next;
		c = a;
		while (b)
		{
			if (cmp(c, b) > 0) {
				c = b;
			}
			b = b->next;
		}
		d = a->next;
		ft_lst_swap(begin_list, a, c);
		a = d;
	}
}
