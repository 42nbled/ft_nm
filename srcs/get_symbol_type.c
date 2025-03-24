#include "ft_nm.h"

char get_symbol_type(char *secname, int bind, int type, unsigned long addr, short st_shndx) {
	//printf("secname %s\n", secname);
	const char *names =
		".bss\0b\0"
		".text\0t\0"
		".init\0t\0"
		".fini\0t\0"
		".plt\0t\0"
		".data\0d\0"
		".jcr\0d\0"
		".got.plt\0d\0"
		".plt.got\0t\0"
		".init_array\0d\0"
		".dynamic\0d\0"
		".fini_array\0d\0"
		".got\0d\0"
		".comment\0n\0"
		".dynstr\0r\0"
		".dynsym\0r\0"
		".rodata\0r\0"
		".gnu.hash\0r\0"
		".gnu.version\0r\0"
		".gnu.version_r\0r\0"
		".note.gnu.build-id\0r\0"
		".eh_frame\0r\0"
		".eh_frame_hdr\0r\0"
		".gcc_except_table\0r\0"
		".note.ABI-tag\0r\0"
		".interp\0r\0"
		".rela.plt\0r\0"
		".rela.dyn\0r\0";

	if (bind == STB_WEAK) {
		if (type == STT_FUNC || type == STT_NOTYPE) {
			return (addr != 0) ? 'W' : 'w';
		} else if (type == STT_OBJECT) {
			return (addr != 0) ? 'V' : 'v';
		}
	}
	char symbol_type = 'U';

	//printf("  s\n");
	if (secname && *secname) {
		if (strncmp(secname, ".debug_", 7) == 0)
			return 'N';
		for (const char *p = names; *p != '\0'; p += strlen(p) + 1) {
			//printf("\t%s\n", p);
			if (strcmp(secname, p) == 0) {
				symbol_type = *(p + strlen(p) + 1);
				break;
			}
		}
	}
	if (st_shndx < 0)
		symbol_type = 'a';

	if (bind == STB_GLOBAL) {
		return toupper(symbol_type);
	} else if (bind == STB_LOCAL) {
		return symbol_type;
	}

	return '?';
}
