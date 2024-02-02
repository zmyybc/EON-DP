# Variables
OBJECTS = Deepmd.o
CXXFLAGS += -I../.. -I${dp_root}/include -I${tf_root}/tensorflow/tf/include -Wall -fPIC  # Add -fPIC for position independent code
LDFLAGS += -L${dp_root}/lib -L${tf_root}/tensorflow/tf/lib
LDLIBS += -ldeepmd_cc -ltensorflow_cc -Wl,-rpath=${dp_root}/lib -Wl,-rpath=${tf_root}/tensorflow/tf/lib
LIB_STATIC := libDeepmd.a
LIB_SHARED := libDeepmd.so  # The name of the shared library
AR = ar

# Targets
$(LIB_STATIC) : $(OBJECTS)
	$(AR) rcs $@ $^

$(LIB_SHARED) : $(OBJECTS)
	$(CXX) -shared $(LDFLAGS) $^ $(LDLIBS) -o $@

clean :
	rm -f $(OBJECTS) $(LIB_STATIC) $(LIB_SHARED)

.PHONY : clean
