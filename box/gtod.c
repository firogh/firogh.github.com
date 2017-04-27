#include <sys/auxv.h>
#include <elf.h> /* man 5 elf */
#include <stdio.h>
#include <sys/time.h>

int parse_vdso()
{
	unsigned char *vdso, *s_names, *dyn_strtab;
	Elf32_Ehdr *e;
	Elf32_Shdr *s, s_names_hdr, *dyn_sym_hdr;
	Elf32_Sym *dyn_sym;
	unsigned int i;
	struct timeval tmp, start, end;
	int (*gtod)(struct timeval *tv, struct timezone *tz);

	vdso = getauxval(AT_SYSINFO_EHDR);
	if (!vdso)
		return 1;

	e = (Elf32_Ehdr *) vdso;
	if (e->e_ident[EI_CLASS] != ELFCLASS32) 
		return 2;
	s = (Elf32_Shdr *) (vdso + e->e_shoff);	
	if (e->e_shstrndx == SHN_UNDEF)
		return 3;
	s_names_hdr = s[e->e_shstrndx];
	s_names = vdso + s_names_hdr.sh_offset;
	printf("section names index %d, sections start %p vdso %p\n", e->e_shstrndx, s, vdso);

	for (i = 0; i < e->e_shnum; i++)
	{
		printf("section name %s\n", s_names + s[i].sh_name);
		printf("%d %d %d %d\n", i, s[i].sh_link, s[i].sh_offset, s[s[i].sh_link].sh_offset);
		if (!strcmp(s_names + s[i].sh_name, ".dynsym"))
		{
			dyn_sym_hdr = &s[i];
			dyn_sym = (Elf32_Sym *)(vdso + s[i].sh_offset);
			dyn_strtab = vdso + s[s[i].sh_link].sh_offset;
			//printf("%d %d %d %d\n", i, s[i].sh_link, s[i].sh_offset, s[s[i].sh_link].sh_offset);
			/* not brake for printing all section name. */
		}
	}

	for (i = 0; i * sizeof(Elf32_Sym) < dyn_sym_hdr->sh_size; i++)
	{
		printf("%s %p %p\n", dyn_strtab + dyn_sym[i].st_name, dyn_sym[i].st_value);
		if (!strcmp(dyn_strtab + dyn_sym[i].st_name, "__vdso_gettimeofday"))
		{
			gtod = vdso +  dyn_sym[i].st_value;
			gettimeofday(&tmp, NULL);
			gtod(&start, NULL);
			gettimeofday(&end, NULL);
			printf("Firo %ld %ld %ld\n",(end.tv_sec * 1000000 + end.tv_usec), (end.tv_sec * 1000000 + end.tv_usec), (start.tv_sec * 1000000 + start.tv_usec));
		}
	}
	
	return 0;
}

int main(int argc, char **argv)
{
	return parse_vdso();
}


