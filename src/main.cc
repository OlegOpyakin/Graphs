#include <random>
#include "ControlFlowGraph.h"

void TestSmallGraph(){
    // ------------------------------------------
    // tested graph:
    // bb0: x = 10;
    //      y = x + 2;
    //      goto bb1;
    // 
    // bb1: y = x + y;
    //      x = x - 1;
    //      if(x > 2) goto bb1; else goto bb2;
    //
    // bb2: t = x + y
    //      return t
    // ------------------------------------------

    ControlFlowGraph cfg(6, 3); // cfg(num_of_vars, graph_size)
    cfg.AddEdge(0, 1);
    cfg.AddEdge(1, 2);
    cfg.AddEdge(1, 1);

    cfg.blocks_[0].AddDef(1);
    cfg.blocks_[0].AddDef(4);

    cfg.blocks_[1].AddDef(2);
    cfg.blocks_[1].AddDef(5);

    cfg.blocks_[0].AddKill(0);
    cfg.blocks_[0].AddKill(2);
    cfg.blocks_[0].AddKill(3);
    cfg.blocks_[0].AddKill(5);

    cfg.blocks_[1].AddKill(0);
    cfg.blocks_[1].AddKill(1);
    cfg.blocks_[1].AddKill(3);
    cfg.blocks_[1].AddKill(4);

    cfg.InitBitVectors();
    std::cout << "Iterations: " << cfg.ReachingDefinitions() << "\n";
    //cfg.ShowGraph();
}

void GenerateAndTest(size_t num_of_vars, size_t graph_size, size_t edges_number) {
    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> var_dist(0, num_of_vars - 1);
    std::uniform_int_distribution<int> block_dist(0, graph_size - 1);
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);

    ControlFlowGraph cfg(num_of_vars, graph_size);

    for(size_t i = 0; i < graph_size - 1; ++i) {
        cfg.AddEdge(i, i + 1);
    }

    size_t additional_edges = (edges_number > graph_size - 1) ? edges_number - (graph_size - 1) : 0;
    for(size_t i = 0; i < additional_edges; ++i) {
        size_t src = block_dist(e);
        size_t dst = block_dist(e);
        
        if(src != dst || (src > 0 && prob_dist(e) < 0.3)) {
            cfg.AddEdge(src, dst);
        }
    }

    for(size_t i = 0; i < graph_size; ++i) {
        std::vector<bool> defined_vars(num_of_vars, false);
        std::vector<bool> killed_vars(num_of_vars, false);
        
        size_t num_defs = std::min(num_of_vars, static_cast<size_t>(1 + (prob_dist(e) * 3)));
        for(size_t j = 0; j < num_defs; ++j) {
            size_t var = var_dist(e);
            if(!defined_vars[var]) {
                cfg.blocks_[i].AddDef(var);
                defined_vars[var] = true;
            }
        }

        for(size_t var = 0; var < num_of_vars; ++var) {
            if(defined_vars[var]) {
                cfg.blocks_[i].AddKill(var);
                killed_vars[var] = true;
            } else if(prob_dist(e) < 0.2) {
                cfg.blocks_[i].AddKill(var);
                killed_vars[var] = true;
            }
        }
        
        if(i < graph_size - 1) {
            bool has_def = false;
            for(size_t var = 0; var < num_of_vars; ++var) {
                if(defined_vars[var]) {
                    has_def = true;
                    break;
                }
            }
            if(!has_def) {
                size_t var = var_dist(e);
                cfg.blocks_[i].AddDef(var);
                cfg.blocks_[i].AddKill(var);
            }
        }
    }

    cfg.InitBitVectors();
    std::cout << "Iterations: " << cfg.ReachingDefinitions() << "\n\n";
    //cfg.ShowGraph();
}

int main() try{
    std::cout << "Testing Small Predefined Graph\n";
    TestSmallGraph();
    
    std::cout << "\nTest Generated Graphs\n";
    std::cout << "Small Graph (5 vars, 4 blocks, 6 edges)\n";
    GenerateAndTest(5, 4, 6);
    
    std::cout << "Medium Graph (10 vars, 8 blocks, 12 edges)\n";
    GenerateAndTest(10, 8, 12);
    
    std::cout << "Large Graph (15 vars, 12 blocks, 20 edges)\n";
    GenerateAndTest(20, 12, 30);
}
catch (const std::exception& e){
    std::cerr << "Error: " << e.what() << "\n";
}
catch (...){
    std::cerr << "Unknown exception\n";
}
