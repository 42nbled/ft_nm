#include "ft_nm.h"

char get_symbol_type(char *secname, int bind, int type, unsigned long addr, short st_shndx) {
	const char *names =
		".bss\0b\0"
		".text\0t\0"
		".init\0t\0"
		".fini\0t\0"
		".data\0d\0"
		".got.plt\0d\0"
		".init_array\0d\0"
		".dynamic\0d\0"
		".fini_array\0d\0"
		".got\0d\0"
		".rodata\0r\0"
		".eh_frame\0r\0"
		".eh_frame_hdr\0r\0"
		".gcc_except_table\0r\0"
		".note.ABI-tag\0r\0"
		".interp\0r\0";

	// Handle weak symbols
	if (bind == STB_WEAK) {
		if (type == STT_FUNC || type == STT_NOTYPE) {
			return (addr != 0) ? 'W' : 'w';
		} else if (type == STT_OBJECT) {
			return (addr != 0) ? 'V' : 'v';
		}
	}

	// Default to undefined
	char symbol_type = 'U';

	if (secname && *secname) {
		if (strncmp(secname, ".debug_", 7) == 0)
			return 'N';
		for (const char *p = names; *p != '\0'; p += strlen(p) + 1) {
			if (strcmp(secname, p) == 0) {
				symbol_type = *(p + strlen(p) + 1);
				break;
			}
		}
	}
	if (st_shndx < 0)
		symbol_type = 'a';

	// Handle global and local bindings
	if (bind == STB_GLOBAL) {
		return toupper(symbol_type);
	} else if (bind == STB_LOCAL) {
		return symbol_type;
	}

	return '?';
}

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

//			if (l->data->stt_type == STT_SECTION)
//				TYPE == SECTION
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

t_files	*arg_check(t_flags *flags, int argc, char **argv) {
	t_files	*files = NULL;

	for (int i = 1; i < argc; i++) {
		if (argv[i][0] && argv[i][0] == '-') {
			for (int y = 1; argv[i][y]; y++) {
				if (argv[i][y] == 'a')
					flags->a = 1;
				else if (argv[i][y] == 'g')
					flags->g = 1;
				else if (argv[i][y] == 'u')
					flags->u = 1;
				else if (argv[i][y] == 'r')
					flags->r = 1;
				else if (argv[i][y] == 'p')
					flags->p = 1;
				else
					assert(0 && "Unimplemented flag.");
			}
		}
		else {
			if (!files) {
				files = malloc(sizeof(t_files));
				files->name = argv[i];
				files->next = NULL;
			}
			else {
				t_files *tmp = files;
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = malloc(sizeof(t_files));
				tmp->next->name = argv[i];
				tmp->next->next = NULL;
			}
		}
	}
	if (!files) {
		files = malloc(sizeof(t_files));
		files->name = "a.out";
		files->next = NULL;
	}
	return files;
}


void	main_32(int fd, t_flags flags) {
	t_fileinfo32	file_header;
	t_lst			name_tables = {.next=NULL, .data=NULL};

	read(fd, &file_header, sizeof(t_fileinfo32));
	for (int i=0; i < file_header.e_shnum; i++)
		run_32(fd, file_header, &name_tables);

	run_32(fd, file_header, &name_tables);
	close(fd);
	print_nm32(name_tables, flags);
}

void	main_64(int fd, t_flags flags, char *executable_name) {
	t_fileinfo64	file_header;
	t_lst			name_tables = {.next=NULL, .data=NULL};

	read(fd, &file_header, sizeof(t_fileinfo64));
	for (int i=0; i < file_header.e_shnum; i++)
		run_64(fd, file_header, &name_tables);

	run_64(fd, file_header, &name_tables);
	close(fd);
	print_nm64(name_tables, flags, executable_name);
}

int main(int argc, char **argv) {
	int				fd;
	t_eident		identifiers;
	t_flags			flags = {0};
	t_files			*files;

	if (argc == 1) {
		printf("Usage: %s [-agurp] [file]\n", argv[0]);
		return (1);
	}
	files = arg_check(&flags, argc, argv);
	int nfiles = 0;
	for (t_files *tmp = files; tmp; tmp = tmp->next)
		nfiles++;
	while (files) {
		if (nfiles > 1)
			printf("\n%s:\n", files->name);
		if ((fd = open(files->name, O_RDONLY)) < 0) {
			fprintf(stderr, "Error while opening the file \'%s\'.", files->name);
			return (1);
		}
		read(fd, &identifiers, sizeof(identifiers));
		if (identifiers.EI_CLASS == 1)
			main_32(fd, flags);
		else
			main_64(fd, flags, files->name);
		files = files->next;
	}
	return (0);
}
