### Install

0. compile LLVM 3.5 following http://llvm.org/docs/GettingStarted.html
1. copy DOPModule in Custom folder into LLVM_PATH/lib
2. add folder DOPModule into the CMakeLists.txt in LLVM_PATH/lib 

    add_subdirectory(DOPModule)

3. run cmake in your build folder 

### Usage:
    
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
