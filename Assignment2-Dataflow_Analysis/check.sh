cd ./build
make
cp ./lib/libDFA.so ../
cd ..

#opt -load-pass-plugin=./libLocalOpts.so -passes=function-info ./test/Loop.bc -o ./test/LoopFunctionInfo.bc
para=$1

if [ "$para" = "all" ];then
    echo "test all"
    #opt -load-pass-plugin=./libLocalOpts.so -passes=algebraic-identity,strength-reduction,multi-inst-opt ./test/TestCase1.ll -disable-output | $(llvm-config --bindir)/FileCheck --match-full-lines --check-prefix=CHECK ./test/TestCase1.ll
   
elif [ "$para" = "1" ];then
    echo "test avail-expr"
    opt -S -load-pass-plugin=./build/lib/libDFA.so -passes=avail-expr ./test/1-AvailExpr.ll -o ./test/1-AvailExprout.ll
elif [ "$para" = "2" ];then
    echo "test liveness"
    opt -S -load-pass-plugin=./build/lib/libDFA.so -passes=liveness ./test/2-Liveness.ll -o ./test/2-Livenessout.ll
elif [ "$para" = "3" ];then
    echo "test const-prop"
    opt -S -load-pass-plugin=./build/lib/libDFA.so -passes=const-prop ./test/3-SCCP.ll -o ./test/3-SCCPout.ll
elif [ "$para" = "4" ];then
    echo "test lcm"
    opt -S -load-pass-plugin=./build/lib/libDFA.so -passes=lcm ./test/4-LCM.ll -o ./test/4-LCMout.ll
fi
