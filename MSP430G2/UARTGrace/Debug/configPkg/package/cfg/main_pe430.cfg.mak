# invoke SourceDir generated makefile for main.pe430
main.pe430: .libraries,main.pe430
.libraries,main.pe430: package/cfg/main_pe430.xdl
	$(MAKE) -f C:\Users\PittyBishop\workspace_v5_4\UARTGrace/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\PittyBishop\workspace_v5_4\UARTGrace/src/makefile.libs clean

