
CXX = g++
CXXFLAGS = -Wall -fPIC
MC_TARGET = MonteCarlo
INT_TARGET = Integration

Integration: $(INT_TARGET).cc
	$(CXX) $(CXXFLAGS) $(INT_TARGET).cc -o $(INT_TARGET) 

MonteCarlo: $(MC_TARGET).cc
	$(CXX) $(CXXFLAGS) $(MC_TARGET).cc -o $(MC_TARGET) 

clean:
	rm -f $(INT_TARGET) $(MC_TARGET)

