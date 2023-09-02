# Output executable
OUTPUT = server

all: $(OUTPUT)

$(OUTPUT): server.c
	gcc server.c filter/*.c -o $(OUTPUT) 

clean:
	rm -f $(OUTPUT)
