CPP      = clang++
CPPFLAGS = -std=c++14 -Wall -O3

DIR  = svm nn
TDIR = ../test/
FILE = svm/multi_svm.cpp nn/activation.cpp nn/nn.cpp

all:
	$(CPP) $(CPPFLAGS) -c nn/activation.cpp nn/nn.cpp

test:
	$(CPP) $(CPPFLAGS) -c nn/nn.cpp
	$(CPP) $(CPPFLAGS) -o nn_test activation.o nn.o $(TDIR)nn_test.cpp

# $(CPP) $(CPPFLAGS) -o activation_test activation.o $(TDIR)activation_test.cpp
# $(CPP) $(CPPFLAGS) -o svm_test svm/multi_svm.o $(TDIR)svm_test.cpp
# $(CPP) $(CPPFLAGS) -o matrix_test $(TDIR)matrix_test.cpp
# $(CPP) $(CPPFLAGS) -o kernal_test $(TDIR)kernal_test.cpp
# $(CPP) $(CPPFLAGS) -o vector_test $(TDIR)vector_test.cpp

tag:
	cd .. && ctags -R --sort=yes -f .tags

clean:
	for dir in $(DIR); do \
		(cd $$dir && rm *.o) || continue ; \
	done
	rm *.o ../.tags vector_test matrix_test kernal_test svm_test activation_test nn_test
