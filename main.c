#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include "ft_nm.h"
#include <ctype.h>

// Section types
#define SHT_SYMTAB      2
#define SHT_DYNSYM      11
#define SHT_NOBITS      8
#define SHT_PROGBITS    1

// Section flags
#define SHF_WRITE       (1 << 0)
#define SHF_ALLOC       (1 << 1)
#define SHF_EXECINSTR   (1 << 4)

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

char get_symbol_type(char *secname, int bind, int type, unsigned long addr, int sectype, unsigned long sh_flags) {
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
        ".rodata\0r\0"
        ".eh_frame\0r\0"
        ".eh_frame_hdr\0r\0"
        ".gcc_except_table\0r\0"
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

    // Specific sections
    if (strcmp(secname, ".text") == 0 || strcmp(secname, ".init") == 0 || strcmp(secname, ".fini") == 0) {
        return (addr != 0) ? 'T' : 't';
    } else if (strcmp(secname, ".abi_tag") == 0) {
         return (addr != 0) ? 'U' : 'u';
    }

    // Additional checks
    if (sectype == SHT_NOBITS && (sh_flags & SHF_ALLOC) && (sh_flags & SHF_WRITE)) {
        symbol_type = 'b';
    } else if (sectype == SHT_PROGBITS) {
        if (sh_flags & SHF_ALLOC) {
            if (sh_flags & SHF_WRITE) {
                symbol_type = 'd';
            } else if (sh_flags & SHF_EXECINSTR) {
                symbol_type = 't';
            } else {
                symbol_type = 'r';
            }
        }
    }

    // Handle global and local bindings
    if (bind == STB_GLOBAL) {
        return toupper(symbol_type);
    } else if (bind == STB_LOCAL) {
        return symbol_type;
    }

    return '?';
}


//int	main()
//{
//	printf("%c\n",
//		get_symbol_type(".interp", STB_GLOBAL, STT_OBJECT, 123));
//
//}
// TODO: Parse all tables all at once
void parse_table(t_lst *root, t_Elf64_Sym *symbol_table, char *stringtable, size_t size, char *shstrtab, t_secinfo64 *sections)
{
    for (size_t i = 0; i < size; i++)
    {
        if ((size_t)symbol_table[i].st_shndx >= size)
            continue;  // Skip invalid entries

        t_name_table *t = malloc(sizeof(t_name_table));
        if (!t)
            assert(0 && "TODO: free all and exit");
        t->name = &stringtable[symbol_table[i].st_name];
        sprintf(t->value, "%016llx", symbol_table[i].st_value);

        t->type = get_symbol_type(
                shstrtab + sections[symbol_table[i].st_shndx].sh_name,
                ELF64_ST_BIND(symbol_table[i].st_info),
                ELF64_ST_TYPE(symbol_table[i].st_info),
                symbol_table[i].st_value,
                sections[symbol_table[i].st_shndx].sh_type,
                sections[symbol_table[i].st_shndx].sh_flags);
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

int	diff_str(t_lst *a, t_lst *b) {
    if (!a || !a->data)
		return 0;
	if (!b || !b->data)
		return 0;
    char *data_a = (char *)a->data->name;
    char *data_b = (char *)b->data->name;

    return strcmp(data_a, data_b);
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

void print_nm(t_lst name_tables) {
	name_tables = *name_tables.next;
    if (name_tables.next) {
        t_lst **lst = &(name_tables.next);
        ft_lst_sort(lst, diff_str);
        for (t_lst *l = name_tables.next; l != NULL; l = l->next) {
			if (l->data->type == 'w' || l->data->type == 'U')
				printf("                 %c %s\n", l->data->type, l->data->name);
			else
			    printf("%s %c %s\n", l->data->value, l->data->type, l->data->name);
		}
	}
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
	print_nm(name_tables);
	lst_clear(name_tables.next);
}
