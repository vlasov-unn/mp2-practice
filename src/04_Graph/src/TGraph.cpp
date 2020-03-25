#include "TGraph.h"

bool TGraph::connected(const TAdjacencyMatrix& adjMatrix) const
{
    TAdjacencyList adjList(vertexCount);
    for (int i = 0; i < vertexCount; i++)
        for (int j = i + 1; j < vertexCount; j++)
            if (adjMatrix[i][j] == true)
            {
                adjList.add(i, j);
                adjList.add(j, i);
            }
    std::vector<bool> vertexUsed;
    vertexUsed.resize(vertexCount, false);
    std::queue<TVertexId> vertexQueue;
    vertexQueue.push(0);
    vertexUsed[0] = true;
    while (!vertexQueue.empty())
    {
        int vertex = vertexQueue.front();
        vertexQueue.pop();
        for (size_t i = 0; i < adjList[vertex].size(); ++i)
        {
            int adjacentVertex = adjList[vertex][i];
            if (!vertexUsed[adjacentVertex])
            {
                vertexUsed[adjacentVertex] = true;
                vertexQueue.push(adjacentVertex);
            }
        }
    }
    bool isGraphConnected = true;
    for (size_t i = 0; i < vertexUsed.size(); i++)
        if (!vertexUsed[i])
        {
            isGraphConnected = false;
            break;
        }
    return isGraphConnected;
}

TAdjacencyMatrix TGraph::generateAdjacencyMatrix() const
{
    TAdjacencyMatrix adjMatrix(vertexCount);
    for (int i = 0; i < edgesCount; i++)
    {
        if ((adjMatrix[edges[i].x][edges[i].y] == false) && (adjMatrix[edges[i].y][edges[i].x] == false))
        {
            adjMatrix[edges[i].x][edges[i].y] = true;
            adjMatrix[edges[i].y][edges[i].x] = true;
        }
    }
    return adjMatrix;
}

void TGraph::freeEdges()
{
    if (edges)
        delete[] edges;
    edgesCount = 0;
    edges = nullptr;
}

TGraph::TGraph() : edges(nullptr), vertexCount(0), edgesCount(0)
{
}

TGraph::TGraph(const TGraph& other)
{
    vertexCount = other.vertexCount;
    edgesCount = other.edgesCount;
    edges = new TEdge[edgesCount];
    for (int i = 0; i < edgesCount; i++)
        edges[i] = other.edges[i];
}

TGraph::TGraph(TGraph&& other)
{
    vertexCount = other.vertexCount;
    edgesCount = other.edgesCount;
    edges = other.edges;
    other.vertexCount = 0;
    other.edgesCount = 0;
    other.edges = nullptr;
}

TGraph::TGraph(std::initializer_list<TEdge> edgesList, int vertexCount_)
{
    if (vertexCount_ < 0)
        throw InvalidParameterError();
    vertexCount = vertexCount_;
    edgesCount = static_cast<int>(edgesList.size());
    edges = new TEdge[edgesCount];
    int i = 0;
    for (const TEdge& edge : edgesList)
    {
        edges[i++] = edge;
    }
    TAdjacencyMatrix adjMatrix = generateAdjacencyMatrix();
    for (int i = 0; i < edgesCount; i++)
    {
        if (edges[i].x == edges[i].y)
        {
            freeEdges();
            throw SelfLoopedGraphError();
        }
        if (adjMatrix[edges[i].x][edges[i].y] != adjMatrix[edges[i].y][edges[i].x])
        {
            freeEdges();
            throw DirectedGraphError();
        }
    }
    if (!connected(adjMatrix))
    {
        freeEdges();
        throw MulticonnectedGraphError();
    }
}

TGraph::TGraph(TEdge* edges_, int edgesCount_, int vertexCount_)
{
    if ((vertexCount_ < 0) || (edgesCount_ < 0))
        throw InvalidParameterError();
    vertexCount = vertexCount_;
    edgesCount = edgesCount_;
    edges = new TEdge[edgesCount];
    for (int i = 0; i > edgesCount; i++)
    {
        edges[i] = edges_[i];
    }
    TAdjacencyMatrix adjMatrix = generateAdjacencyMatrix();
    for (int i = 0; i < edgesCount; i++)
    {
        if (edges[i].x == edges[i].y)
        {
            freeEdges();
            throw SelfLoopedGraphError();
        }
        if (adjMatrix[edges[i].x][edges[i].y] != adjMatrix[edges[i].y][edges[i].x])
        {
            freeEdges();
            throw DirectedGraphError();
        }
    }
    if (!connected(adjMatrix))
    {
        freeEdges();
        throw MulticonnectedGraphError();
    }
}

TGraph::~TGraph()
{
    freeEdges();
}

TGraph& TGraph::operator=(const TGraph& other)
{
    freeEdges();
    vertexCount = other.vertexCount;
    edgesCount = other.edgesCount;
    edges = new TEdge[edgesCount];
    for (int i = 0; i < edgesCount; i++)
        edges[i] = other.edges[i];
    return *this;
}

TGraph& TGraph::operator=(TGraph&& other)
{
    freeEdges();
    vertexCount = other.vertexCount;
    edgesCount = other.edgesCount;
    edges = other.edges;
    other.vertexCount = 0;
    other.edgesCount = 0;
    other.edges = nullptr;
    return *this;
}

TGraph TGraph::kruskalAlgorithm() const
{
    TDisjointSet vertex(vertexCount);
    TGraph result;
    result.vertexCount = vertexCount;
    result.edges = new TEdge[vertexCount - 1];
    for (int i = 0; i < vertexCount; i++)
    {
        vertex.createSingleton(i);
    }
    THeap<TEdge> heapEdges(edges, edgesCount);
    int tEdgesCount = 0;
    while ((tEdgesCount != vertexCount - 1) && (!heapEdges.empty()))
    {
        TEdge edge = heapEdges.topMin();
        heapEdges.popMin();
        int setX = vertex.findSet(edge.x), setY = vertex.findSet(edge.y);
        if (setX != setY)
        {
            vertex.unite(setX, setY);
            result.edges[tEdgesCount] = edge;
            tEdgesCount++;
        }
    }
    result.edgesCount = tEdgesCount;
    return result;
}

TPathData TGraph::dijkstraAlgorithm(TVertexId startVertex) const
{
    float* dist = new float[vertexCount];
    TVertexId* up = new TVertexId[vertexCount];
    TPair* distId = new TPair[vertexCount];
    for (int i = 0; i < vertexCount; i++)
    {
        distId[i].distance = std::numeric_limits<float>::infinity();
        distId[i].vertex = i;
        up[i] = 0;
    }
    distId[startVertex].distance = 0.0f;
    THeap<TPair> vertexIds(distId, vertexCount);
    while (!vertexIds.empty())
    {
        TPair minId = vertexIds.topMin();
        TVertexId index = 0, indexX = 0, indexY = 0;
        for (int i = 0; i < edgesCount; i++)
        {
            if (edges[i].incidental(minId.vertex))
            {
                for (int j = 0; j < vertexIds.getSize(); j++)
                {
                    if (distId[j].vertex == edges[i].x)
                        indexX = j;
                    if (distId[j].vertex == edges[i].y)
                        indexY = j;
                }
                if (minId.distance + edges[i].weight < distId[indexX].distance)
                {
                    distId[indexX].distance = minId.distance + edges[i].weight;
                    up[edges[i].x] = minId.vertex;
                }
                if (minId.distance + edges[i].weight < distId[indexY].distance)
                {
                    distId[indexY].distance = minId.distance + edges[i].weight;
                    up[edges[i].y] = minId.vertex;
                }
            }
        }
        dist[minId.vertex] = minId.distance;
        vertexIds.popMin();
    }
    TPathData pathList(dist, up, vertexCount, startVertex);
    delete[] dist, delete[] up, delete[] distId;
    return pathList;
}

inline int TGraph::getVertexCount() const
{
    return vertexCount;
}

inline int TGraph::getEdgesCount() const
{
    return edgesCount;
}

bool TGraph::hasEdge(TVertexId x, TVertexId y) const
{
    for (int i = 0; i < edgesCount; i++)
        if (edges[i].incidental(x) && edges[i].incidental(y))
            return true;
    return false;
}

TEdgeList TGraph::getEdges() const
{
    TEdgeList list(edgesCount);
    for (int i = 0; i < edgesCount; i++)
        list[i] = edges[i];
    return list;
}

TEdgeList TGraph::getEdges(TEdgeFilterFunc filter) const
{
    TEdgeList list;
    for (int i = 0; i < edgesCount; i++)
        if((*filter)(edges[i]))
            list.push_back(edges[i]);
    return list;
}

float TGraph::getWeight() const
{
    float weight = 0.0f;
    for (int i = 0; i < edgesCount; i++)
        weight += edges[i].weight;
    return weight;
}

float TGraph::getWeight(TEdgeFilterFunc filter) const
{
    float weight = 0.0f;
    for (int i = 0; i < edgesCount; i++)
        if((*filter)(edges[i]))
            weight += edges[i].weight;
    return weight;
}

void TGraph::print() const
{
    std::cout << "[\n";
    for (int i = 0; i < edgesCount; i++)
        std::cout << "    (" << edges[i].x << ", " << edges[i].y << ") {" << edges[i].weight << "}\n";
    std::cout << "]\n";
}

std::istream& operator>>(std::istream& input, TGraph& graph)
{
    int vertexCount_ = 0, edgesCount_ = 0;
    input >> vertexCount_ >> edgesCount_;
    if ((vertexCount_ < 0) || (edgesCount_ < 0))
        throw TGraph::InvalidParameterError();
    TEdge* edges_ = new TEdge[edgesCount_];
    for (int i = 0; i < edgesCount_; i++)
        input >> edges_[i].x >> edges_[i].y >> edges_[i].weight;
    graph = TGraph(edges_, edgesCount_, vertexCount_);
    return input;
}
