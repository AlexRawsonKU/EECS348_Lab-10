calculator.exe: calculator.cpp calculator.hpp main.cpp
	$(CXX) -o $@ $(CXXFLAGS) $^