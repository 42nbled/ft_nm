#include "ft_nm.h"

static void free_files(t_files *files) {
	t_files *current = files;
	t_files *next;

	while (current) {
		next = current->next;
		free(current);
		current = next;
	}
}

static t_files *add_file(t_files *files, char *name) {
	t_files *new_file = malloc(sizeof(t_files));
	if (!new_file) {
		free_files(files);
		fprintf(stderr, "Error: Memory allocation failed\n");
		return NULL;
	}

	new_file->name = name;
	new_file->next = NULL;

	return new_file;
}

t_files *arg_check(t_flags *flags, int argc, char **argv) {
	t_files *files = NULL;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] && argv[i][0] == '-') {
			for (int y = 1; argv[i][y]; y++) {
				switch (argv[i][y]) {
					case 'a': flags->a = 1; break;
					case 'g': flags->g = 1; break;
					case 'u': flags->u = 1; break;
					case 'r': flags->r = 1; break;
					case 'p': flags->p = 1; break;
					default:
						fprintf(stderr, "Error: Unimplemented flag '-%c'\n", argv[i][y]);
						free_files(files);
						return NULL;
				}
			}
		}
		else {
			if (!files) {
				files = add_file(NULL, argv[i]);
				if (!files)
					return NULL;
			}
			else {
				t_files *tmp = files;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = add_file(files, argv[i]);
				if (!tmp->next)
					return NULL;
			}
		}
	}

	if (!files) {
		files = add_file(NULL, "a.out");
		if (!files)
			return NULL;
	}

	return files;
}
