cd ./build
make
cp ./lib/libLocalOpts.so ../
cd ..

#opt -load-pass-plugin=./libLocalOpts.so -passes=function-info ./test/Loop.bc -o ./test/LoopFunctionInfo.bc
para=$1

if [ "$para" = "all" ];then
    echo "test all"
    #opt -load-pass-plugin=./libLocalOpts.so -passes=algebraic-identity,strength-reduction,multi-inst-opt ./test/TestCase1.ll -disable-output | $(llvm-config --bindir)/FileCheck --match-full-lines --check-prefix=CHECK ./test/TestCase1.ll
    opt -S -load-pass-plugin=./libLocalOpts.so -passes=algebraic-identity,strength-reduction,multi-inst-opt ./test/TestCase1.ll -o ./test/TestCase1t.ll
    opt -S -load-pass-plugin=./libLocalOpts.so -passes=algebraic-identity,strength-reduction,multi-inst-opt ./test/TestCase2.ll -o ./test/TestCase2t.ll
    opt -S -load-pass-plugin=./libLocalOpts.so -passes=algebraic-identity,strength-reduction,multi-inst-opt ./test/TestCaseBasic.ll -o ./test/TestCaseBasict.ll
elif [ "$para" = "1" ];then
    echo "test Algebraic Identity"
    opt -S -load-pass-plugin=./libLocalOpts.so -passes=algebraic-identity ./test/TestCase1.ll -o ./test/TestCase1a.ll
elif [ "$para" = "2" ];then
    echo "test Strength Reduction"
    opt -S -load-pass-plugin=./libLocalOpts.so -passes=strength-reduction ./test/TestCase1.ll -o ./test/TestCase1s.ll
elif [ "$para" = "3" ];then
    echo "test Multi-Instruction Optimization"
    opt -S -load-pass-plugin=./libLocalOpts.so -passes=multi-inst-opt ./test/TestCase1.ll -o ./test/TestCase1s.ll
fi
