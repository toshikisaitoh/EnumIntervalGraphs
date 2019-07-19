# CPPFLAGS := -O3 -std=c++14 -Wall -Wextra -Wl,-stack,268435456
# CC := "env OMPI_CC=g++ mpic++"
CXX := g++
CPPFLAGS := -O3 -std=c++14 -Wl,-stack,268435456 -D USE_MPI=0

COMMONDIR := ./../common
OBJECTS := main.o $(COMMONDIR)/trie.o interval.o mpq_tree.o mpq_tree_node.o ultimate.o

main.exe: $(OBJECTS)
# 	g++ -o main.exe $(OBJECTS) -I "C:\Program Files (x86)\OpenMPI_v1.5.5-x64\include" -L "C:\Program Files (x86)\OpenMPI_v1.5.5-x64\lib"  -l libmpi_cxx.lib -l libmpi.lib -l libopen-pal.lib -l libopen-rte.lib advapi32.lib Ws2_32.lib shlwapi.lib
	g++ -o main.exe $(OBJECTS)


$(COMMONDIR)/trie.o: $(COMMONDIR)/trie.h

interval.o: $(COMMONDIR)/abstruct_enumerator.h mpq_tree.o mpq_tree_node.o ultimate.o

interval.o main.o: $(COMMONDIR)/abstruct_enumerator.h

mpq_tree.o mpq_tree_node.o: mpq_tree.o mpq_tree.o

ultimate.o: ultimate.h

clean:
	rm $(OBJECTS)
