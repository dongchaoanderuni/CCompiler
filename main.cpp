#include "llvm/IR/LLVMContext.h"    /* 公共数据结构 */
#include "llvm/IR/Module.h"         /* 一个源文件的抽象(全局变量、函数、基本块组成) */
#include "llvm/IR//IRBuilder.h"     /* 指令生成器、加法、减法等。还可以获取类型 */
#include "llvm/IR/Verifier.h"       /* 校验模块、校验函数胡 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <typeinfo>
#include <vector>
#include <string>
#include <list>
#include <deque>
#include <forward_list>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <functional>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <new>
#include <memory>
#include <initializer_list>
#include <vector>
#include <string>
#include <stdexcept>
#include <utility>

using namespace llvm;

int main()
{
    auto llvmContxt= std::make_unique<LLVMContext>();
    auto module = std::make_unique<Module>("ir_global", *llvmContxt);
    auto irBuilder = std::make_unique<IRBuilder<>>(*llvmContxt);

    // int a = 10;
    // const int a = 10;

    module->getOrInsertGlobal("a", irBuilder->getInt32Ty());
    GlobalVariable *a = module->getNamedGlobal("a");
    a->setInitializer(irBuilder->getInt32(10));
    a->setConstant(true);

    module->print(errs(), nullptr);

    // int *p = &a;
    GlobalVariable *p = static_cast<GlobalVariable *>(module->getOrInsertGlobal("p", PointerType::getInt32PtrTy(*llvmContxt)));
    p->setInitializer(a);

    // double dArr[2] = {1.1, 2.2};
    ArrayType *arrayType = ArrayType::get(irBuilder->getDoubleTy(),2);
    GlobalVariable *dArr = module->getNamedGlobal("dArr");
    dArr->setInitializer((ConstantArray::get(arrayType,
                                             {
        ConstantFP::get(irBuilder->getDoubleTy(), 1.1),
        ConstantFP::get(irBuilder->getDoubleTy(), 2.2),
                                             })));

    // struct A {int a, float b, double d,, int *p};
    StructType *structType = StructType::create(*llvmContxt, "A");
    structType->setBody({
        irBuilder->getInt32Ty(),
        irBuilder->getFloatTy(),
        irBuilder->getDoubleTy(),
        PointerType::getInt32Ty(*llvmContxt)
    });

    // struct A s= {1, 1.1, 2.2, p};
    module->getOrInsertGlobal("stkt", structType);
    GlobalVariable *stkt = module->getNamedGlobal("stkt");
    stkt->setInitializer(ConstantStruct::get(structType,{
        irBuilder->getInt32(1),
        ConstantFP::get(irBuilder->getFloatTy(), 1.1),
        ConstantFP::get(irBuilder->getDoubleTy(), 2.2),
        ConstantPointerNull::get(PointerType::getInt32PtrTy(*llvmContxt))
    }));


    return 0;
};