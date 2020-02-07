//
// Created by mathieu on 07/02/2020.
//

#ifndef INC_3A_ECC_CPP_P256_H
#define INC_3A_ECC_CPP_P256_H

#include "UnsignedBigInteger.h"
#include "ModularBigInteger.h"
#include "Point.h"

namespace ecc {
    class P256 {
    public:
        static UnsignedBigInteger m;
        static ModularBigInteger a;
        static ModularBigInteger b;
        static Point generator;
    };

    std::string mod = "115792089210356248762697446949407573530086143415290314195533631308867097853951";
    UnsignedBigInteger P256::m(mod);
    ModularBigInteger P256::a("3", mod);
    ModularBigInteger P256::b("41058363725152142129326129780047268409114441015993725554835256314039467401291", mod);
    Point P256::generator(
            UnsignedBigInteger("48439561293906451759052585252797914202762949526041747995844080717082404635286"),
            UnsignedBigInteger("2258390684796862237411570494974242622288194167061563441992324890848025900319"),
            UnsignedBigInteger("1"),
            P256::a.value,
            P256::b.value,
            mod
    );
}


#endif //INC_3A_ECC_CPP_P256_H
