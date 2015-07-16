SRCS 		= add.cpp alu.cpp aluctrl.cpp and.cpp ctrl.cpp main.cpp \
		mux.cpp ram.cpp register.cpp registerfile.cpp rom.cpp \
		shift.cpp signextend.cpp decoder.cpp shiftjump.cpp  mask.cpp \
		bigalu.cpp combine.cpp decoderfpu.cpp fpuctrl.cpp fpushift.cpp incordec.cpp muxfpu.cpp roundhw.cpp smallalu.cpp 



OBJS 		= $(SRCS:%.cpp=%.o)
DEPS		= $(SRCS:%.cpp=%.d)

SYSTEMC_HOME 	= /usr/local/systemc230

CXX		= g++
CXXFLAGS	= -Wall -ansi -g -O0 -Di386 
INCLUDES 	= -I $(SYSTEMC_HOME)/include

LD		= $(CXX)
LDFLAGS 	= $(CXXFLAGS)

LIBS		= -L$(SYSTEMC_HOME)/lib-linux64 -lsystemc -Wl,-rpath,/usr/local/systemc230/lib-linux64

PROGRAM 	= mips

default:	$(PROGRAM)

$(PROGRAM):	$(OBJS) $(DEPS)
		$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o ./$@
		
test:		$(PROGRAM)
		./$<
		
clean:; 	rm -rf $(PROGRAM) *.o core* *.d *.vcd *.dump

# Cancel implicit rule
%.o:		%.cpp

%.o:		%.cpp %.d
		$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.d: 		%.cpp
		@ echo Making dependencies for $<
		@ g++ -MM $(INCLUDES) $< | sed 's#: # $*.d : #1' > $@

# Include dependency files .d
ifneq ("$(wildcard *.d)","")
  include $(wildcard *.d)
endif
