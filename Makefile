CC=gcc

all:
	$(CC) -g -o vm -fsanitize=address,undefined vm.c
	$(CC) -g -o serialize -fsanitize=address,undefined serialize.c

clean:
	rm -rf vm serialize pan* *.trail

spin:
	spin -a $(PWD)/spin-coins.pml
	gcc -DVECTORSZ=2048 -o pan pan.c
	#echo "-------------------------------------"
	#spin -t $(PWD)/spin-coins.pml
	#echo "-------------------------------------"
	./pan && ./pan -r
