/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   UGraphModel.h
 * Author: LTSACH
 *
 * Created on 24 August 2020, 15:16
 */

#ifndef UGRAPHMODEL_H
#define UGRAPHMODEL_H

#include "graph/AbstractGraph.h"

//////////////////////////////////////////////////////////////////////
///////////// UGraphModel: Undirected Graph Model ////////////////////
//////////////////////////////////////////////////////////////////////

template <class T>
class UGraphModel : public AbstractGraph<T>
{
private:
public:
    // class UGraphAlgorithm;
    // friend class UGraphAlgorithm;

    UGraphModel( bool (*vertexEQ)(T &, T &), string (*vertex2str)(T &))
         : AbstractGraph<T>(vertexEQ, vertex2str) {}

    void connect(T from, T to, float weight = 0)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode   = this->getVertexNode(to);

        if (!fromNode) throw VertexNotFoundException("");
        if (!toNode)   throw VertexNotFoundException("");

        fromNode->connect(toNode, weight);
        toNode->connect(fromNode, weight);
    }

    void disconnect(T from, T to)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode   = this->getVertexNode(to);
        typename AbstractGraph<T>::Edge *disconnectEdge = fromNode->getEdge(toNode);
        
        if (!fromNode)       throw VertexNotFoundException("");
        if (!toNode)         throw VertexNotFoundException("");
        if (!disconnectEdge) throw EdgeNotFoundException("");

        fromNode->removeTo(toNode);
        toNode->removeTo(fromNode);
    }

    void remove(T vertex)
    {
        // TODO
        typename AbstractGraph<T>::VertexNode *removeNode = this->getVertexNode(vertex);
        if (!removeNode)
            throw VertexNotFoundException("");

        DLinkedList<T> outEdges = this->getOutwardEdges(vertex);
        for (auto adjnode : outEdges)
        {
            typename AbstractGraph<T>::VertexNode *adjNode = this->getVertexNode(adjnode);
            removeNode->removeTo(adjNode);
            adjNode->removeTo(removeNode);
        }

        AbstractGraph<T>::nodeList.removeItem(
            removeNode, 
            [](typename AbstractGraph<T>::VertexNode *delVertex) { 
                delete delVertex; 
            }
        );
    }

    static UGraphModel<T> *create(
        T *vertices, int nvertices, Edge<T> *edges, int nedges,
        bool (*vertexEQ)(T &, T &),
        string (*vertex2str)(T &))
    {
        // TODO
        UGraphModel<T> *model = new UGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i < nvertices; i++)
            model->add(vertices[i]);

        for (int i = 0; i < nedges; i++)
            model->connect(edges[i].from, edges[i].to, edges[i].weight);

        return model;
    }
};

#endif /* UGRAPHMODEL_H */
