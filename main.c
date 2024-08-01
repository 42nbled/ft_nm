#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include "ft_nm.h"

#define ELF32_ST_BIND(i)   ((i)>>4)
#define ELF32_ST_TYPE(i)   ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

#define ELF64_ST_BIND(i)   ((i)>>4)
#define ELF64_ST_TYPE(i)   ((i)&0xf)
#define ELF64_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

struct s_pair {
	t_Elf64_Sym	*symtable;
	char		*strtable;
};

typedef struct s_lst {
	struct s_lst	*next;
	t_name_table	*data;
} t_lst;

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

char	get_symbol_type(char *secname, int bind, int type, unsigned long addr)
{
	char	*names = 
		".bss\0b\0"
		".text\0t\0"
		".init\0t\0"
		".fini\0t\0"
		".data\0d\0"
		".got.plt\0d\0"
		".init_array\0d\0"
		".dynamic\0d\0"
		".fini_array\0d\0"
		".rodata\0r\0"
		".eh_frame\0r\0"
		".eh_frame_hdr\0r\0"
		".gcc_except_table\0r\0"
		".interp\0r\0";

	if (bind == STB_WEAK)
	{
		if (type == STT_FUNC || type == STT_NOTYPE) {
			if (addr != 0)
				return 'W';
			if (!addr)
				return 'w';
		} else if (type == STT_OBJECT) {
			if (addr != 0)
				return 'V';
			if (!addr)
				return 'v';
		}
	}
	int	c = '.';
	if (!*secname)
		c = 'u';
	else
	{
		for (int i=0, j=0; i < 15; i++) {
			if (!strcmp(secname, names + j))
			{
				c = names[j + strlen(names + j) + 1];
				break ;
			}
			j += strlen(names + j) + 1 + 2;
		}
	}
	if (bind == STB_LOCAL)
		return c;
	else if (bind == STB_GLOBAL)
		return '_' & c; // '_' & 'n' <=> uppercase(n), also ' ' | 'N' <=> lowercase(n)
	return ' ';
}

//int	main()
//{
//	printf("%c\n",
//		get_symbol_type(".interp", STB_GLOBAL, STT_OBJECT, 123));
//
//}
// TODO: Parse all tables all at once
t_name_table *parse_table(t_Elf64_Sym *symbol_table, char *stringtable, size_t size, char *shstrtab)
{
	t_name_table *name_table = malloc(sizeof(t_name_table) * (size + 1));
	if (!name_table)
		assert(0 && "TODO: free all and exit");
	for (size_t i = 0; i < size; i++)
	{
		name_table[i].name = &stringtable[symbol_table[i].st_name];
		sprintf(name_table[i].value, "%#08lx", symbol_table[i].st_value);

		name_table[i].type = get_symbol_type(
				shstrtab + symbol_table[i].st_shndx,
				ELF64_ST_BIND(symbol_table[i].st_info),
				ELF64_ST_TYPE(symbol_table[i].st_info),
				symbol_table[i].st_value);
	}
	name_table[size].name = NULL;
	return (name_table);
}


void	run(int fd, t_fileinfo64 fileh, t_lst *root)
{
	static char		*names[1024] = {0};
	static size_t	inames = 0;
	t_secinfo64		section;

	static size_t	i = 0;
	static char	*strtab = NULL;
	static t_secinfo64 *sections = NULL;

	if (!sections)
		sections = get_section_headers(fd, fileh);

	if (!strtab)
		strtab = read_strtable(fd, sections[fileh.e_shstrndx]);

	if (i == (size_t)fileh.e_shnum)
	{
		free(strtab);
		free(sections);
		for (size_t j = 0; j < inames; j++)
			free(names[j]);
	}
	section = sections[i];
	if (section.sh_type == 0x02 || section.sh_type == 0x0B)
	{

		t_Elf64_Sym	*symtable = read_symtable(fd, section);
		char		*strtable = read_strtable(fd, sections[section.sh_link]);
		lst_append(root, parse_table(symtable,
			 strtable,
			 section.sh_size / sizeof(t_Elf64_Sym),
			 strtab));
		free(symtable);
		names[inames++] = strtable;
	}
	i++;
}

int main(int argc, char **argv) {
	int				fd;
	t_eident		identifiers;
	t_fileinfo64	file_header;
	t_lst			name_tables = {.next=NULL, .data=NULL};

	if (argc == 1) {
		printf("Usage: %s [file]\n", argv[0]);
		return (1);
	}
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
	for (t_lst *l=name_tables.next; l != NULL; l=l->next)
		for (size_t i=0; l->data[i].name != NULL; i++)
			printf("%s %c %s\n", l->data[i].value, l->data[i].type, l->data[i].name);

	lst_clear(name_tables.next);
}
