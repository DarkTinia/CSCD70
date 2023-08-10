clang -O2 -emit-llvm -c ./test/ -o ./test/Loop.bc
llvm-dis ./test/Loop.bc -o=./test/Loop.ll

#opt -load-pass-plugin=./libFunctionInfo.so -passes=function-info ./test/Loop.bc -o ./test/LoopFunctionInfo.bc

opt -load-pass-plugin=./libFunctionInfo.so -passes=function-info ./test/Loop.bc -disable-output | $(llvm-config --bindir)/FileCheck --check-prefix=CHECK ./test/Loop.c