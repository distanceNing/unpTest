//
// Created by yangning on 17-12-3.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//

#include "socket/epoller.h"

int main()
{
    std::string str("ad\r\nb\r\nc\r\nd\r\n");
    std::cout << str.length() << str.size() << std::endl;
    size_t pos = 0;
    std::string key("");
    size_t dis = 0;
    for (int i = 0; i < 4; ++i) {
        dis = key.size()  == 0 ? 0 : key.size() + 2 +dis;
        pos = str.find("\r\n", pos);
        key = str.substr(dis, pos -dis);
        pos += 2;
        std::cout << key << std::endl;
    }
    return 0;
}