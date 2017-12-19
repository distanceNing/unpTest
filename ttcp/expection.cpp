//
// Created by yangning on 17-12-10.
//
// Descriprion :
//
// Copyright (c) yangning All rights reserved.
//
#include <iostream>

void  func()
{
    throw std::exception();
}

int main()
{
    try {
        func();
    }
    catch (std::exception &e){
        std::cout<<e.what()<<std::endl;
    }

    return 0;
}