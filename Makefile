
all:

	-mkdir bin
	
	$(MAKE) -C src/c-game/
	$(MAKE) -C src/game/

clean:

	-$(MAKE) clean -C src/c-game
	
	-$(MAKE) clean -C src/game
	-rmdir bin

run:

	./bin/grafcube -t 

web:

	./.tx2html README

test:

	$(MAKE) clean
	$(MAKE)
	$(MAKE) run
