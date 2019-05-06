CXXFLAGS = -std=c++11 -g -Wall -Wextra -Werror

bloom: bloom.o BloomFilter.o HashSet.o IntegerHashes.o StringHashes.o
	${CXX} $(CXXFLAGS) -o $@ $^

bloom.o: bloom.cpp | BloomFilter.h HashSet.h
	${CXX} $(CXXFLAGS) -c -o $@ $^

BloomFilter.o: BloomFilter.cpp | BloomFilter.h IntegerHashes.h StringHashes.h
	${CXX} $(CXXFLAGS) -c -o $@ $^

HashSet.o: HashSet.cpp | HashSet.h IntegerHashes.h StringHashes.h
	${CXX} $(CXXFLAGS) -c -o $@ $^

IntegerHashes.o: IntegerHashes.cpp | IntegerHashes.h
	${CXX} $(CXXFLAGS) -c -o $@ $^

StringHashes.o: StringHashes.cpp | StringHashes.h
	${CXX} $(CXXFLAGS) -c -o $@ $^

clean:
	rm -rf bloom *.o
