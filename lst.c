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

void	testfunction() {
	return;
}

void	_testfunction() {
	return;
}

void	test_function() {
	return;
}

void	testfunction_() {
	return;
}

void	_test_function() {
	return;
}

void	_testfunction_() {
	return;
}

void	test_function_() {
	return;
}

void	_test_function_() {
	return;
}


void	__testfunction() {
	return;
}

void	test__function() {
	return;
}

void	testfunction__() {
	return;
}

void	__test__function() {
	return;
}

void	__testfunction__() {
	return;
}

void	test__function__() {
	return;
}

void	__test__function___() {
	return;
}

void	mainb() {
	return;
}

void	maind() {
	return;
}

char	*remove_underscore(char *str) {

	size_t len = 0;

	for (int i = 0; str[i]; i++) {
		if (str[i] != '_' && str[i] != '@' && str[i] != '.')
			len++;
	}
	len++;

	char *new_str = malloc(len + 1);

	int y = 0;
	for (size_t i = 0; str[i]; i++)
	{
		if (str[i] != '_' && str[i] != '@' && str[i] != '.')
			new_str[y++] = str[i];
	}
	new_str[y] = '\0';

	// free(str);
	return new_str;

}

int count_start_underscores(char **str) {
	int count = 0;
	while (**str == '_') {
		count++;
		(*str)++;
	}
	return count;
}

int count_middle_underscores(const char *str) {
	int count = 0;
	for (int i = 0; str[i]; i++) {
		if (str[i] == '_')
			count++;
	}
	return count;
}

int count_end_underscores(char *str) {
	int count = 0;
	size_t len = strlen(str);

	for (int i = len - 1; i >= 0 && str[i] == '_'; i--) {
		count++;
		str[i] = '\0';
	}
	return count;
}

int diff_str(t_lst *a, t_lst *b) {
	if (!a || !a->data) return 0;
	if (!b || !b->data) return 0;

	char *c_orig = strdup(a->data->name);
	char *d_orig = strdup(b->data->name);
	if (!c_orig || !d_orig) {
		free(c_orig);
		free(d_orig);
		return 0;
	}

	char *c = c_orig;
	char *d = d_orig;

	for (int i = 0; c[i]; i++)
		c[i] = tolower(c[i]);
	for (int i = 0; d[i]; i++)
		d[i] = tolower(d[i]);

	int underscore_count_a_start = count_start_underscores(&c);
	int underscore_count_b_start = count_start_underscores(&d);
	int underscore_count_a_middle = count_middle_underscores(c);
	int underscore_count_b_middle = count_middle_underscores(d);
	int underscore_count_a_end = count_end_underscores(c);
	int underscore_count_b_end = count_end_underscores(d);

	char *c_clean = remove_underscore(c);
	char *d_clean = remove_underscore(d);
	int status = strcmp(c_clean, d_clean);

	free(c_orig);
	free(d_orig);
	free(c_clean);
	free(d_clean);

	if (status != 0)
		return status;
	if (underscore_count_a_start != underscore_count_b_start)
		return (underscore_count_b_start - underscore_count_a_start);
	if (underscore_count_a_middle != underscore_count_b_middle)
		return (underscore_count_b_middle - underscore_count_a_middle);
	if (underscore_count_a_end != underscore_count_b_end)
		return (underscore_count_a_end - underscore_count_b_end);

	return (a->data->i_value - b->data->i_value);
}

int reverse_diff_str(t_lst *a, t_lst *b) {
	if (!a || !a->data) return 0;
	if (!b || !b->data) return 0;

	char *c_orig = strdup(a->data->name);
	char *d_orig = strdup(b->data->name);
	if (!c_orig || !d_orig) {
		free(c_orig);
		free(d_orig);
		return 0;
	}

	char *c = c_orig;
	char *d = d_orig;

	for (int i = 0; c[i]; i++)
		c[i] = tolower(c[i]);
	for (int i = 0; d[i]; i++)
		d[i] = tolower(d[i]);

	int underscore_count_a_start = count_start_underscores(&c);
	int underscore_count_b_start = count_start_underscores(&d);
	int underscore_count_a_middle = count_middle_underscores(c);
	int underscore_count_b_middle = count_middle_underscores(d);
	int underscore_count_a_end = count_end_underscores(c);
	int underscore_count_b_end = count_end_underscores(d);

	char *c_clean = remove_underscore(c);
	char *d_clean = remove_underscore(d);
	int status = strcmp(c_clean, d_clean);

	free(c_orig);
	free(d_orig);
	free(c_clean);
	free(d_clean);

	if (status != 0)
		return -status;
	if (underscore_count_a_start != underscore_count_b_start)
		return -(underscore_count_b_start - underscore_count_a_start);
	if (underscore_count_a_middle != underscore_count_b_middle)
		return -(underscore_count_b_middle - underscore_count_a_middle);
	if (underscore_count_a_end != underscore_count_b_end)
		return -(underscore_count_a_end - underscore_count_b_end);

	return -(b->data->i_value - a->data->i_value);
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
