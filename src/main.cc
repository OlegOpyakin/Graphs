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
    std::cout << "\nIterations: " << cfg.ReachingDefinitions() << "\n";
    cfg.ShowGraph();
}

void TestBigGraph(size_t num_of_vars, size_t graph_size, size_t edges_number) {
    // Goal of this test is to check 
    // the convergence of the algorithm

    std::random_device r;
    std::default_random_engine e(r());
    std::uniform_int_distribution<int> dist1(0,num_of_vars-1);
    std::uniform_int_distribution<int> dist2(1,graph_size-1);
    std::uniform_int_distribution<int> dist3(1,2);

    ControlFlowGraph cfg(num_of_vars, graph_size); // cfg(num_of_vars, graph_size)

    for(int i = 0; i < edges_number; ++i){
        cfg.AddEdge(dist2(e), dist2(e)); // adding edges
    }

    for(int i = 0; i < graph_size;  ++i){
        for(int j = 0; j < num_of_vars; ++j){
            if(dist3(e) == 2)  cfg.blocks_[i].AddDef(j);
            if(dist3(e) == 2)  cfg.blocks_[i].AddKill(j);
        }
    }

    cfg.InitBitVectors();
    std::cout << "\nIterations: " << cfg.ReachingDefinitions() << "\n";
    cfg.ShowGraph();
}

int main() try{
    //TestBigGraph(30, 10, 20);
    TestSmallGraph();
}
catch (const std::exception& e){
    std::cerr << "Error: " << e.what() << "\n";
}
catch (...){
    std::cerr << "Unknown exception\n";
}
