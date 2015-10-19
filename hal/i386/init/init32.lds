
OUTPUT_FORMAT("binary", "binary", "binary")
OUTPUT_ARCH(i386)

ENTRY(init32)


SECTIONS
{
  .text : {
	*(.text)
	}

  . = ALIGN(16);

  .data : {
	*(.data)
	}

  . = ALIGN(16);

  .comment : {
	*(.comment)
	}

  . = ALIGN(16);

  yi_g_nLdInit32BssStart = .;
  .bss : {
	*(.bss)
	}

  . = ALIGN(16);
  yi_g_nLdInit32BssEnd = .;
}

