#include "ft_nm.h"

t_secinfo64	*get_section_headers(int fd, t_fileinfo64 file_header)
{
	t_secinfo64	*lst = malloc(sizeof(t_secinfo64) * file_header.e_shnum);
	if (!lst)
		assert(0 && "TODO: free all and exit");
	lseek(fd, file_header.e_shoff, SEEK_SET);
	for (int i=0; i < file_header.e_shnum; i++)
		read(fd, &lst[i], sizeof(t_secinfo64));
	return (lst);
}

t_Elf64_Sym	*read_symtable(int fd, t_secinfo64 symh)
{
	t_Elf64_Sym	*symtable = malloc(symh.sh_size);
	if (!symtable)
		assert(0 && "TODO: free all and exit");
	lseek(fd, symh.sh_offset, SEEK_SET);
	read(fd, symtable, symh.sh_size);
	return (symtable);
}

char	*read_strtable(int fd, t_secinfo64 strh)
{
	char	*strtable = malloc(strh.sh_size);
	if (!strtable)
		assert(0 && "TODO: free all and exit");
	lseek(fd, strh.sh_offset, SEEK_SET);
	read(fd, strtable, strh.sh_size);
	return (strtable);
}

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


void parse_table(t_lst *root, t_Elf64_Sym *symbol_table, char *stringtable, size_t size, char *shstrtab, t_secinfo64 *sections)
{
	for (size_t i = 1; i < size; i++)
	{
		t_name_table *t = malloc(sizeof(t_name_table));
		if (!t)
			assert(0 && "TODO: free all and exit");
		t->name = strdup(&stringtable[symbol_table[i].st_name]);
		sprintf(t->value, "%016llx", symbol_table[i].st_value);
		t->i_value = symbol_table[i].st_value;

		t->type = get_symbol_type(
				shstrtab + (symbol_table[i].st_shndx > 0 ? sections[symbol_table[i].st_shndx].sh_name : 0),
				ELF64_ST_BIND(symbol_table[i].st_info),
				ELF64_ST_TYPE(symbol_table[i].st_info),
				symbol_table[i].st_value,
				symbol_table[i].st_shndx);
		lst_append(root, t);
	}
}


void run(int fd, t_fileinfo64 fileh, t_lst *root)
{
	static char     *names[1024] = {0};
	static size_t   inames = 0;
	static size_t   i = 0;
	static char     *strtab = NULL;
	static t_secinfo64 *sections = NULL;

	if (!sections)
		sections = get_section_headers(fd, fileh);

	if (!strtab)
		strtab = read_strtable(fd, sections[fileh.e_shstrndx]);

	if (i < (size_t)fileh.e_shnum)
	{
		t_secinfo64 section = sections[i];
		if (section.sh_type == SHT_SYMTAB)
		{
			t_Elf64_Sym *symtable = read_symtable(fd, section);
			char        *strtable = read_strtable(fd, sections[section.sh_link]);
			if (symtable && strtable)
			{
				parse_table(root, symtable, strtable, section.sh_size / sizeof(t_Elf64_Sym), strtab, sections);
				free(symtable);
				if (inames < 1024)
					names[inames++] = strtable;
				else
					free(strtable);
			}
		}
		i++;
	}
	else if (i == (size_t)fileh.e_shnum)
	{
		// Cleanup
		free(strtab);
		free(sections);
		for (size_t j = 0; j < inames; j++)
			free(names[j]);
		strtab = NULL;
		sections = NULL;
		inames = 0;
		i = 0;
	}
}

void print_nm(t_lst name_tables, t_flags flags) {
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
				printf("                 %c %s\n", l->data->type, l->data->name);
			else if (!flags.u && (l->data->type == 'w' || isupper(l->data->type)))
				printf("%s %c %s\n", l->data->value, l->data->type, l->data->name);
			else if (!flags.g && !flags.u)
				printf("%s %c %s\n", l->data->value, l->data->type, l->data->name);
		}
	}
	lst_clear(*lst);
}

void	arg_check(t_flags *flags, int argc, char **argv) {
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
	}
}

int main(int argc, char **argv) {
	int				fd;
	t_eident		identifiers;
	t_fileinfo64	file_header;
	t_lst			name_tables = {.next=NULL, .data=NULL};
	t_flags			flags = {0};

	if (argc == 1) {
		printf("Usage: %s [-agurp] [file]\n", argv[0]);
		return (1);
	}
	arg_check(&flags, argc, argv);
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "Error while opening the file.");
		return (1);
	}

	read(fd, &identifiers, sizeof(identifiers));
	if (identifiers.EI_CLASS == 1) {
		assert(0 && "32 bits currently not supported.");
	}
	read(fd, &file_header, sizeof(t_fileinfo64));

	for (int i=0; i < file_header.e_shnum; i++)
		run(fd, file_header, &name_tables);

	run(fd, file_header, &name_tables);
	close(fd);
	print_nm(name_tables, flags);
}
