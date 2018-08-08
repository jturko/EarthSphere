
CXX = g++
CXXFLAGS = -Wall -fPIC
MC_TARGET = MonteCarlo
INT_TARGET = Integration

Integration3D: $(INT_TARGET)3D.cc
	$(CXX) $(CXXFLAGS) $(INT_TARGET)3D.cc -o $(INT_TARGET)3D 

Integration2DX: $(INT_TARGET)2DX.cc
	$(CXX) $(CXXFLAGS) $(INT_TARGET)2DX.cc -o $(INT_TARGET)2DX 

Integration2DZ: $(INT_TARGET)2DZ.cc
	$(CXX) $(CXXFLAGS) $(INT_TARGET)2DZ.cc -o $(INT_TARGET)2DZ 

MonteCarlo: $(MC_TARGET).cc
	$(CXX) $(CXXFLAGS) $(MC_TARGET).cc -o $(MC_TARGET) 

clean:
	rm -f $(INT_TARGET)3D $(INT_TARGET)2DX $(INT_TARGET)2DZ $(MC_TARGET)

