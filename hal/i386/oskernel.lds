/* linux ld script. */
OUTPUT_FORMAT("elf32-i386", "elf32-i386", "elf32-i386")
OUTPUT_ARCH(i386)

ENTRY(_yike_start)

SECTIONS
{
  . = 0x80000000 + 0x101000;
  yi_g_nLdAllStart = .;
  yi_g_nLdTextStart = .;
  .text : {
	*(.text)
	}

  . = ALIGN(16);
  yi_g_nLdTextEnd = .;

  . = . + 1;
  . = ALIGN(16);

  yi_g_nLdDataStart = .;
  .data : {
	*(.data)
	}
  . = ALIGN(16);
  yi_g_nLdDataEnd = .;

   .rodata : {
	*(.rodata)
	}

  . = ALIGN(16);
   .stab : {
    *(.stab)
    }

   . = ALIGN(16);
   .stabstr : {
    *(.stabstr)
    }

  . = ALIGN(16);
   .comment : {
    *(.comment)
    }

/*  . = ALIGN(16);*/
   .shstrtab : {
    *(.shstrtab)
    }

/*  . = ALIGN(16);*/
   .symstab : {
    *(.symstab)
    }

/*  . = ALIGN(16);*/
   .strstab : {
    *(.strstab)
    }

  . = ALIGN(16);

  yi_g_nLdBssStart = .;
  .bss : {
	*(.bss)
	}

  . = ALIGN(16);
  yi_g_nLdBssEnd = .;

  yi_g_nLdAllEnd = .;
}

