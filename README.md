copy Custom folder into LLVM_PATH/lib
run make

to use:
    /path/to/opt -load /path/to/LLVMDOPModule.so -dop-module /path/to/LLVM_IR_FILE 

optional arguments:
    -dyn-dop <file>:
        dynamic assist takes in a file of lines containing
        <function name> <line no>
        it will check if line number in the function is contained within a loop,
        if yes, it will search for gadgets within it.

    -function-list <file>:
        takes in a path to the file containing functions to track

    -details 
        will print the detailed information about the gadgets
