tree : BuildTree.cpp MLopr.cpp MLremake.h
	g++ -O -std=c++14 MLopr.cpp BuildTree.cpp -o tree


clean:
	rm -rf tree






