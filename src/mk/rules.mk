.PHONY: all clean

# Make the directory
$(DEPDIR):
	mkdir -p $@

# Make the directory
$(BIN_DIR):
	mkdir -p $@

# Cause directory to be built
$(DEPDIR)/%.d: $(DEPDIR);

# Rule to compile .cpp files into .o files and generate .d dependency files
%.o: %.cpp
%.o: %.cpp $(DEPDIR)/%.d
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@

clean:
	-rm -f $(OBJECTS)

$(TARGET): $(OBJECTS) $(AI_LIB) $(BIN_DIR)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)
