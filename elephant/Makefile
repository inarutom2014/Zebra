CPP      = g++
CPPFLAGS = -std=c++14 -Wall -O

DIR  = svm
TDIR = ../test/
OBJ  = svm/multi_svm.o

all:$(OBJ)

svm_test:svm/multi_svm.o
	$(CPP) $(CPPFLAGS) -o svm_test svm/multi_svm.o $(TDIR)svm_test.cpp
	# $(CPP) $(CPPFLAGS) -o matrix_test $(TDIR)matrix_test.cpp
	# $(CPP) $(CPPFLAGS) -o kernal_test $(TDIR)kernal_test.cpp
	# $(CPP) $(CPPFLAGS) -o vector_test $(TDIR)vector_test.cpp

tag:
	cd .. && ctags -R --sort=yes -f .tags

clean:
	for dir in $(DIR); do \
		(cd $$dir && rm *.o) || continue ; \
	done
	rm ../.tags vector_test matrix_test kernal_test svm_test