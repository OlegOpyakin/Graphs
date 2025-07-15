#include <iostream>
#include <map>
#include "BitVector.h"
#include "BasicBlock.h"

#pragma once

class ControlFlowGraph{
public:
    size_t number_of_variables_;
    std::map<size_t, BitVector> RIN_;
    std::map<size_t, BitVector> ROUT_;
    std::map<size_t, BitVector> GEN_;
    std::map<size_t, BitVector> KILL_;
    std::vector<BasicBlock> blocks_;

    ControlFlowGraph(size_t number_of_variables, size_t graph_size): 
        number_of_variables_(number_of_variables) {
        for(size_t i = 0; i < graph_size; ++i){
            blocks_.push_back(BasicBlock(i)); // creatre all basic blocks
        }
    }

    void AddEdge(size_t edge_start, size_t edge_end){
        blocks_[edge_start].AddSucc(edge_end);
        blocks_[edge_end].AddPred(edge_start);
    }

    void InitBitVectors();
    void ShowGraph();
    size_t ReachingDefinitions();
};

void ControlFlowGraph::InitBitVectors() {
    for(size_t i = 0; i < blocks_.size(); ++i){
        RIN_[blocks_[i].id_] = BitVector(number_of_variables_);  // all in are 0
        ROUT_[blocks_[i].id_] = BitVector(number_of_variables_); // all out are 0

        BitVector gen(number_of_variables_);  // default = 0
        BitVector kill(number_of_variables_); // default = 0

        for(size_t def: blocks_[i].definitions_){
            gen.Set(def, true);
        }
        GEN_[blocks_[i].id_] = gen;

        for(size_t use: blocks_[i].kills_){
            kill.Set(use, true);
        }
        KILL_[blocks_[i].id_] = kill;
    }
}

void ControlFlowGraph::ShowGraph() {
    std::cout << "Number of variables " << number_of_variables_ << "\n";
    for(size_t i = 0; i < blocks_.size(); ++i){
        std::cout << "\nBlock " << i << "\nPredcessors: ";
        for(auto it: blocks_[i].predcessors_) {
            std::cout << it << "; ";
        }

        std::cout << "\nSuccessors: ";
        for(auto it: blocks_[i].successors_) {
            std::cout << it << "; ";
        }

        std::cout << "\nRIN_: ";
        RIN_[blocks_[i].id_].Print();
        
        std::cout << "\nROUT_: ";
        ROUT_[blocks_[i].id_].Print();

        std::cout << "\nGEN_: ";
        GEN_[blocks_[i].id_].Print();

        std::cout << "\nKILL_: ";
        KILL_[blocks_[i].id_].Print();
        std::cout << "\n";
    }
}

size_t ControlFlowGraph::ReachingDefinitions(){
    bool changed = true;
    int iterations = 0;

    while(changed){
        changed = false;
        iterations++;
        for(auto& bb: blocks_){
            BitVector NR(number_of_variables_);
            for(auto pred: bb.predcessors_){
                NR = NR | ROUT_[pred];
            }

            BitVector RO = GEN_[bb.id_] | (NR.Substract(KILL_[bb.id_]));

            if(RIN_[bb.id_] != NR){
                RIN_[bb.id_] = NR;
                changed = true;
            }

            if(ROUT_[bb.id_] != RO){
                ROUT_[bb.id_] = RO;
                changed = true;
            }
        }
    }

    return iterations;
}