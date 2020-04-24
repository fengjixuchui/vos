//
// Created by x7cc on 2020/4/24.
//

#ifndef VOS_SVM_H
#define VOS_SVM_H

#include "vos/types.h"

extern void __vmrun (uint64 vmcb);

extern void __vmsave ();

extern void __vmload ();

#endif //VOS_SVM_H
