CS 3503 Assignment 1 - Number Base Conversion

## Author
Sara Waymen

## Description
My implementation of Owl Tech ’s number base conversion utility .
## Build Instructions
```
bash
gcc -o convert convert .c main .c
./ convert
```
## Test Results
100/100 tests passed
## Notes
Challenges:

When I first read the file I came across the binary not reading it correctly
and writing some mixed results.
There was an issue on how I originally coded the file to be parsed was not 
looking at how many inputs as well as not comparing the results to expected output 
appropriately. Once those were addressed the code ran as expected.

Discoveries:

"char *p = line;" allows you to point to the start of the current line without 
modifying the original buffer. It allows you to be able to skip characters and 
trim lines

strcmp() - allows you to compare two strings char by char
