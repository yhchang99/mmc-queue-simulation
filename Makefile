run: mmn.c
	gcc mmn.c -lm -o mmn && ./mmn
compile:
	gcc mmn.c -lm -g -o mmn
exe:
	./mmn
debug:
	gdb ./mmn
plot:
	gnuplot -p script/wait_time.gp
clean:
	rm -f *.o
	rm -f main data/wait_time.txt img/wait_time.png