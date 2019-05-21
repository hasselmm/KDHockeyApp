#pragma once

#include <stdint.h>

typedef uint16_t __u16; 
typedef uint32_t __u32; 
typedef uint64_t __u64; 

typedef int16_t __s16; 
typedef int32_t __s32; 
typedef int64_t __s64; 

typedef __u32 Elf32_Addr;
typedef __u16 Elf32_Half;
typedef __u32 Elf32_Off;
typedef __s32 Elf32_Sword;
typedef __u32 Elf32_Word;

typedef __u64 Elf64_Addr;
typedef __u16 Elf64_Half;
typedef __s16 Elf64_SHalf;
typedef __u64 Elf64_Off;
typedef __s32 Elf64_Sword;
typedef __u32 Elf64_Word;
typedef __u64 Elf64_Xword;
typedef __s64 Elf64_Sxword;

#define ELFCLASS32 1
#define ELFCLASS64 2

#define ELF32_ST_TYPE(val)           ((val) & 0xf)
#define ELF64_ST_TYPE(val)           ELF32_ST_TYPE (val)

#define EM_SPARC        2
#define EM_386          3               /* Intel 80386 */
#define EM_MIPS         8               /* MIPS R3000 Big-Endian only */
#define EM_ARM          40              /* Advanced RISC Machines ARM */
#define EM_AMD64        62              /* AMD64 architecture */
#define EM_X86_64       62
#define EM_PPC          20              /* PowerPC */
#define EM_S390         22
#define EM_PPC64        21
#define EM_SPARCV9      43

#define EI_CLASS        4               /* file class */
#define EI_DATA         5               /* data encoding */
#define EI_NIDENT       16

#define ELFDATA2LSB     1               /* Little-Endian */
#define ELFDATA2MSB     2               /* Big-Endian */

#define PT_LOAD         1               /* loadable segment */
#define PT_NOTE         4               /* auxiliary information */

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_NUM 12
#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_NUM 12
#define SHT_LOOS        0x60000000	
#define SHT_MIPS_DWARF  0x7000001e

#define      ELFMAG          "\177ELF"
#define      SELFMAG         4

#define STT_FUNC     2
#define SHN_UNDEF    0

typedef struct elf32_hdr{
 unsigned char e_ident[EI_NIDENT];
 Elf32_Half e_type;
 Elf32_Half e_machine;
 Elf32_Word e_version;
 Elf32_Addr e_entry;
 Elf32_Off e_phoff;
 Elf32_Off e_shoff;
 Elf32_Word e_flags;
 Elf32_Half e_ehsize;
 Elf32_Half e_phentsize;
 Elf32_Half e_phnum;
 Elf32_Half e_shentsize;
 Elf32_Half e_shnum;
 Elf32_Half e_shstrndx;
} Elf32_Ehdr;

typedef struct elf64_hdr {
 unsigned char e_ident[16];
 Elf64_Half e_type;
 Elf64_Half e_machine;
 Elf64_Word e_version;
 Elf64_Addr e_entry;
 Elf64_Off e_phoff;
 Elf64_Off e_shoff;
 Elf64_Word e_flags;
 Elf64_Half e_ehsize;
 Elf64_Half e_phentsize;
 Elf64_Half e_phnum;
 Elf64_Half e_shentsize;
 Elf64_Half e_shnum;
 Elf64_Half e_shstrndx;
} Elf64_Ehdr;

typedef struct elf32_phdr{
 Elf32_Word p_type;
 Elf32_Off p_offset;
 Elf32_Addr p_vaddr;
 Elf32_Addr p_paddr;
 Elf32_Word p_filesz;
 Elf32_Word p_memsz;
 Elf32_Word p_flags;
 Elf32_Word p_align;
} Elf32_Phdr;

typedef struct elf64_phdr {
 Elf64_Word p_type;
 Elf64_Word p_flags;
 Elf64_Off p_offset;
 Elf64_Addr p_vaddr;
 Elf64_Addr p_paddr;
 Elf64_Xword p_filesz;
 Elf64_Xword p_memsz;
 Elf64_Xword p_align;
} Elf64_Phdr;

typedef struct {
 Elf32_Word sh_name;
 Elf32_Word sh_type;
 Elf32_Word sh_flags;
 Elf32_Addr sh_addr;
 Elf32_Off sh_offset;
 Elf32_Word sh_size;
 Elf32_Word sh_link;
 Elf32_Word sh_info;
 Elf32_Word sh_addralign;
 Elf32_Word sh_entsize;
} Elf32_Shdr;

typedef struct elf64_shdr {
 Elf64_Word sh_name;
 Elf64_Word sh_type;
 Elf64_Xword sh_flags;
 Elf64_Addr sh_addr;
 Elf64_Off sh_offset;
 Elf64_Xword sh_size;
 Elf64_Word sh_link;
 Elf64_Word sh_info;
 Elf64_Xword sh_addralign;
 Elf64_Xword sh_entsize;
} Elf64_Shdr;

typedef struct elf32_note {
 Elf32_Word n_namesz;
 Elf32_Word n_descsz;
 Elf32_Word n_type;
} Elf32_Nhdr;

typedef struct elf64_note {
 Elf64_Word n_namesz;
 Elf64_Word n_descsz;
 Elf64_Word n_type;
} Elf64_Nhdr;

typedef struct elf32_sym{
 Elf32_Word st_name;
 Elf32_Addr st_value;
 Elf32_Word st_size;
 unsigned char st_info;
 unsigned char st_other;
 Elf32_Half st_shndx;
} Elf32_Sym;

typedef struct elf64_sym {
 Elf64_Word st_name;
 unsigned char st_info;
 unsigned char st_other;
 Elf64_Half st_shndx;
 Elf64_Addr st_value;
 Elf64_Xword st_size;
} Elf64_Sym;

