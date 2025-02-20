#include "ft_nm.h"

static char	*remove_underscore(char *str) {

	size_t len = 0;

	for (int i = 0; str[i]; i++) {
		if (str[i] != '_' && str[i] != '@' && str[i] != '.')
			len++;
	}
	len++;

	char *new_str = malloc(len + 1);
	if (!new_str)
		return NULL;

	int y = 0;
	for (size_t i = 0; str[i]; i++)
	{
		if (str[i] != '_' && str[i] != '@' && str[i] != '.')
			new_str[y++] = str[i];
	}
	new_str[y] = '\0';

	return new_str;
}

static int count_start_underscores(char **str) {
	int count = 0;
	while (**str == '_') {
		count++;
		(*str)++;
	}
	return count;
}

static int count_middle_underscores(const char *str) {
	int count = 0;
	for (int i = 0; str[i]; i++) {
		if (str[i] == '_')
			count++;
	}
	return count;
}

static int count_end_underscores(char *str) {
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

	char *c_poiter = strdup(a->data->name);
	char *d_poiter = strdup(b->data->name);
	if (!c_poiter || !d_poiter) {
		free(c_poiter);
		free(d_poiter);
		return 0;
	}

	char *c = c_poiter;
	char *d = d_poiter;

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
	if (!c_clean || !d_clean) {
		free(c_poiter);
		free(d_poiter);
		free(c_clean);
		free(d_clean);
		return 0;
	}
	int status = strcmp(c_clean, d_clean);

	free(c_poiter);
	free(d_poiter);
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

	char *c_poiter = strdup(a->data->name);
	char *d_poiter = strdup(b->data->name);
	if (!c_poiter || !d_poiter) {
		free(c_poiter);
		free(d_poiter);
		return 0;
	}

	char *c = c_poiter;
	char *d = d_poiter;

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
	if (!c_clean || !d_clean) {
		free(c_poiter);
		free(d_poiter);
		free(c_clean);
		free(d_clean);
		return 0;
	}
	int status = strcmp(c_clean, d_clean);

	free(c_poiter);
	free(d_poiter);
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
