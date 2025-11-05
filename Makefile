.PHONY: all
all:
	$(MAKE) -C src/ai-lib
	$(MAKE) -C src/rubiks

clean:
	$(MAKE) -C src/rubiks $@
	$(MAKE) -C src/ai-lib $@
