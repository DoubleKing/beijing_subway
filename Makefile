INC:= 
SRCS:=$(wildcard ./*.cpp)
OBJS:=$(SRCS:%.cpp=%.o)
LIBS:=  -lcurl -lglog
CXX:= g++
CXXFLAGS:=  -std=c++11 -g  $(INC) $(LIBS)

EXE:=beijing_subway

$(EXE):$(OBJS)
	$(CXX)  -o $@ $(OBJS) $(CXXFLAGS)
%.o:%.cpp
	$(CXX)  -c $< -o $@ $(INC) $(CXXFLAGS)
clean:
	rm -rf $(EXE) $(OBJS)