#include <iostream>
#include <vector>

#pragma once

class BasicBlock{
public:
    size_t id_;
    std::vector<size_t> predcessors_;
    std::vector<size_t> successors_;
    std::vector<size_t> definitions_;
    std::vector<size_t> kills_;

    BasicBlock(size_t id): id_(id) {}

    void AddPred(size_t pred){
        predcessors_.push_back(pred);
    }

    void AddSucc(size_t succ){
        successors_.push_back(succ);
    }

    void AddDef(size_t def){
        definitions_.push_back(def);
    }

    void AddKill(size_t kill){
        kills_.push_back(kill);
    }
};