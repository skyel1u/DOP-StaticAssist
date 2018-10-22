### Install

#### Before

```
Package	                  Version	    Notes
GNU Make	              3.79, 3.79.1	Makefile/build processor
GCC	                      >=4.7.0	    C/C++ compiler1
python	                  >=2.5	        Automated test suite2
GNU M4	                  1.4	        Macro processor for configuration3
GNU Autoconf	          2.60      	Configuration script builder3
GNU Automake	          1.9.6     	aclocal macro generator3
libtool                   1.5.22	    Shared library manager3
zlib	                  >=1.2.3.4	    Compression library4
```
Make sure your gcc and g++ version both is **4.8.5**.

#### Begin build llvm system

0. compile LLVM 3.5 following http://llvm.org/docs/GettingStarted.html

```markdown
* Checkout LLVM:
cd where-you-want-llvm-to-live
svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm

* Checkout Clang:
cd where-you-want-llvm-to-live
cd llvm/tools
svn co http://llvm.org/svn/llvm-project/cfe/trunk clang

* build

cd where you want to build llvm
mkdir build
cd build
cmake <path to llvm sources>
make -j12
```

1. copy DOPModule in Custom folder into LLVM_PATH/lib

2. add folder DOPModule into the CMakeLists.txt in LLVM_PATH/lib 

    add_subdirectory(DOPModule)

3. run cmake;make in your build folder 

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
