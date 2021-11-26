
# C Project: Assembler

_**Grade**: 100_

The program takes an assembly file(s) as input, and outputs 3 files: 
* **object file**: contains the memory image of the machine code in two parts: the code segment appears first, and when it ends, the data segment of the machine code will be shown.
* **externals file**: contains the memory addresses of the code segment where an extern label was declared (using the 'extern' keyword). 
* **entries file**: contains the memory addresses of the code segment where an entry label was declared (using the 'entry' keyword). 

If an error was found in the given machine code, it won't output any files and will throw an error log.

Submitted by: 
* [Tomer Rubinstein](https://github.com/Tomer-Rubinstein)
* [Anat Paliy](https://github.com/AnatPaliy)
