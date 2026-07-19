//
//  Memory.hpp
//
//  Created by Alexis ZBIK on 12/02/2020.
//  Copyright © 2020 Bleass. All rights reserved.
//

#ifndef Memory_hpp
#define Memory_hpp

#include <cstring>
#include <cstdlib>

class Memory {
public:
    //Copy
    static void copy(float* _in, float* _out, const size_t frameCount);
};

#endif /* Memory_hpp */
