all:
    	gcc -o mot a3.c -lpthread
run:
    	./mot
clean:
      	@rm mot
