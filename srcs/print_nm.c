#include "ft_nm.h"

void print_nm32(t_lst name_tables, t_flags flags) {
	t_lst **lst = &(name_tables.next);
	if (name_tables.next) {
		if (!flags.p) {
			if (flags.r)
				ft_lst_sort(lst, reverse_diff_str);
			else
				ft_lst_sort(lst, diff_str);
		}
		for (t_lst *l = *lst; l != NULL; l = l->next) {
			if (l->data->type == 'a')
			{
				if (flags.a && !flags.g && !flags.u)
					printf("%s %c %s\n", l->data->value, l->data->type, l->data->name);
			}
			else if (l->data->type == 'w' || l->data->type == 'U')
				printf("         %c %s\n", l->data->type, l->data->name);
			else if (!flags.u && (l->data->type == 'w' || isupper(l->data->type)))
				printf("%s %c %s\n", l->data->value, l->data->type, l->data->name);
			else if (!flags.g && !flags.u)
				printf("%s %c %s\n", l->data->value, l->data->type, l->data->name);
		}
	}
	lst_clear(*lst);
}

void print_nm64(t_lst name_tables, t_flags flags, char *executable_name) {
	t_lst **lst = &(name_tables.next);
	if (name_tables.next) {
		if (!flags.p) {
			if (flags.r)
				ft_lst_sort(lst, reverse_diff_str);
			else
				ft_lst_sort(lst, diff_str);
		}
		for (t_lst *l = *lst; l != NULL; l = l->next) {
			if ((flags.g || flags.u) && l->data->name[0] == '.')
				continue ;
			if (l->data->stt_type == STT_SECTION)
			{
				if (flags.a)
					printf("%s %c %s\n", l->data->value, l->data->type, l->data->name);
				continue ;
			}
			if (l->data->name[0] == '\0' && l->data->type != 'a' && !strcmp(l->data->value, "0000000000000000"))
				continue;

			if (l->data->type == 'a')
			{
				if (flags.a && !flags.g && !flags.u)
					printf("%s %c %s\n", l->data->value, l->data->type, l->data->name);
			}
			else if (l->data->type == 'w' || l->data->type == 'U')
				printf("                 %c %s\n", l->data->type, l->data->name);
			else if (!flags.u && (l->data->type == 'w' || isupper(l->data->type)))
				printf("%s %c %s\n", l->data->value, l->data->type, l->data->name);
			else if (!flags.g && !flags.u)
				printf("%s %c %s\n", l->data->value, l->data->type, l->data->name);
		}
	}
	else {
		printf("ft_nm: %s no symbols\n", executable_name);
	}
	lst_clear(*lst);
}
