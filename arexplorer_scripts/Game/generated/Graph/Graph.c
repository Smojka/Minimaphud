 /*
 ===========================================
 Do not modify, this script is generated
 ===========================================
 */

 class Graph: Managed
 {
     proto external bool HasNode(GraphNodeId nodeId);
     proto external GraphNode GetNode(GraphNodeId nodeId);
     proto external GraphNodeId AddNode(GraphNode graphNode);
     proto external bool RemoveNode(GraphNodeId nodeId);
     proto external bool RemoveNodes(notnull array<GraphNodeId> nodeIds);
     proto external bool AddEdge(GraphNodeId node1, GraphEdge edge);
     proto external GraphEdge GetEdge(GraphNodeId node1, GraphNodeId node2);
     proto external int GetEdges(GraphNodeId node, out notnull array<ref GraphEdge> edges);
     proto external bool RemoveEdge(GraphNodeId node1, GraphNodeId node2);
     proto external bool HasEdge(GraphNodeId node1, GraphNodeId node2);
     proto external int GetAdjacentNodes(GraphNodeId node, out notnull array<GraphNodeId> adjacentNodes);
     proto void BFS(GraphNodeId startNode, func callback);
     proto void DFS(GraphNodeId startNode, func callback);

     // callbacks

     event bool CanAddNode(GraphNode graphNode) { return true; };
     event void OnNodeAdded(GraphNode graphNode);
     event void OnNodeRemoved(GraphNode graphNode);
     event bool CanAddEdge(GraphNode node1, GraphEdge edge) { return true; };
     event void OnEdgeAdded(GraphNode node1, GraphEdge edge);
     event void OnEdgeRemoved(GraphNode node1, GraphEdge edge);
 }
