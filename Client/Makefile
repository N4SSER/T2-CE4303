# Output executable
OUTPUT = client

all: $(OUTPUT)

$(OUTPUT): client.c
	gcc client.c -o $(OUTPUT) -lm

clean:
	rm -f $(OUTPUT)