#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include "ft_nm.h"


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

struct s_pair	get_pair(int fd, t_secinfo64 symh, t_secinfo64 *sections)
{
	t_secinfo64 strh = sections[symh.sh_link];
	t_Elf64_Sym	*symtable = malloc(symh.sh_size);
	char	*strtable = malloc(strh.sh_size);
	if (!symtable || !strtable)
		assert(0 && "TODO: free all and exit");
	lseek(fd, symh.sh_offset, SEEK_SET);
	read(fd, symtable, symh.sh_size);

	lseek(fd, strh.sh_offset, SEEK_SET);
	read(fd, strtable, strh.sh_size);

	struct s_pair	p = {symtable, strtable};
	return (p);
}

// TODO: Parse all tables all at once
t_name_table *parse_table(t_Elf64_Sym *symbol_table, char *stringtable, size_t size)
{
	t_name_table *name_table = malloc(sizeof(t_name_table) * (size + 1));
	if (!name_table)
		assert(0 && "TODO: free all and exit");
	for (size_t i = 0; i < size; i++)
	{
		name_table[i].name = &stringtable[symbol_table[i].st_name];
		sprintf(name_table[i].value, "%#08lx", symbol_table[i].st_value);
	}
	name_table[size].name = NULL;
	return (name_table);
}

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Usage: %s [file]\n", argv[0]);
		return (1);
	}
	int fd;
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "Error while opening the file.");
		return (1);
	}

	t_eident identifiers;

	read(fd, &identifiers, sizeof(identifiers));
	if (identifiers.EI_CLASS == 1) {
		assert(0 && "32 bits currently not supported.");
	}
	t_fileinfo64 file_header;
	read(fd, &file_header, sizeof(t_fileinfo64));
	t_secinfo64 *sections = get_section_headers(fd, file_header);
	t_lst	name_tables = {0};

	char	*names[1024] = {0};
	size_t	index = 0;

	for (int i=0; i < file_header.e_shnum; i++)
	{
		if (sections[i].sh_type == 0x02) {
			struct s_pair p = get_pair(fd, sections[i], sections);
			t_name_table *t = parse_table(p.symtable, p.strtable, sections[i].sh_size / sizeof(t_Elf64_Sym));
			lst_append(&name_tables, t);
////			printf("----- SYM SECTION -----\n");
////			for (size_t j=0; t[j].name != NULL; j++)
////				printf("%s _ %s\n", t[j].value, t[j].name);
			free(p.symtable);
			names[index++] = p.strtable;
		}
		if (sections[i].sh_type == 0x0B) {
			struct s_pair p = get_pair(fd, sections[i], sections);
			t_name_table *t = parse_table(p.symtable, p.strtable, sections[i].sh_size / sizeof(t_Elf64_Sym));
			lst_append(&name_tables, t);
//			printf("----- DYM SECTION -----\n");
//			for (size_t j=0; t[j].name != NULL; j++)
//				printf("%s _ %s\n", t[j].value, t[j].name);
			free(p.symtable);
			names[index++] = p.strtable;
		}
	}
	close(fd);

	for (t_lst *l=name_tables.next; l != NULL; l=l->next)
	{
		for (size_t i=0; l->data[i].name != NULL; i++)
			printf("%s _ %s\n", l->data[i].value, l->data[i].name);
	}

	lst_clear(name_tables.next);

	for (size_t i=0; i < index; i++)
		free(names[i]);
	free(sections);
}
