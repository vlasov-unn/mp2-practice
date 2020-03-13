#include <iostream>
#include "TGraph.h"

int main()
{
    TGraph graph1 = {
        TEdge(0,1,8),
        TEdge(1,5,2),
        TEdge(5,0,2),
        TEdge(0,4,3),
        TEdge(1,4,0),
        TEdge(1,3,1),
        TEdge(3,4,3),
        TEdge(3,2,5),
        TEdge(1,2,10),
        TEdge(4,2,2),
        TEdge(2,5,9)
    };
    graph1.vertexCount = 6;

    TGraph graph2 = {
        TEdge(0,1,5),
        TEdge(0,2,8),
        TEdge(0,3,11),
        TEdge(1,4,4),
        TEdge(2,3,3),
        TEdge(2,4,2),
        TEdge(3,5,8),
        TEdge(4,5,6),
        TEdge(4,7,1),
        TEdge(5,6,13),
        TEdge(5,7,24),
        TEdge(6,7,5),
        TEdge(2,5,9)
    };
    graph2.vertexCount = 8;

    TGraph graph3 = graph1.kruskalAlgorithm();
    graph3.output();

    int* dist;
    int* up;

    graph2.dijkstraAlgorithm(dist, up);
}
