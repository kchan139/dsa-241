/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DGraphModel.h
 * Author: LTSACH
 *
 * Created on 23 August 2020, 19:36
 */

#ifndef DGRAPHMODEL_H
#define DGRAPHMODEL_H
#include "graph/AbstractGraph.h"

//////////////////////////////////////////////////////////////////////
///////////// GraphModel: Directed Graph Model    ////////////////////
//////////////////////////////////////////////////////////////////////


template<class T>
class DGraphModel: public AbstractGraph<T>{
private:
public:
    DGraphModel(
            bool (*vertexEQ)(T&, T&), 
            string (*vertex2str)(T&) ): 
        AbstractGraph<T>(vertexEQ, vertex2str){
    }
    
    void connect(T from, T to, float weight=0){
        //TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode   = this->getVertexNode(to);

        if (!fromNode) throw VertexNotFoundException("none");
        if (!toNode)   throw VertexNotFoundException("none");

        fromNode->connect(toNode, weight);
    }
    void disconnect(T from, T to){
        //TODO
        typename AbstractGraph<T>::VertexNode *fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode *toNode   = this->getVertexNode(to);
        typename AbstractGraph<T>::Edge *disconnectEdge = fromNode->getEdge(toNode);

        if (!fromNode)       throw VertexNotFoundException("");
        if (!toNode)         throw VertexNotFoundException("");
        if (!disconnectEdge) throw EdgeNotFoundException("");

        fromNode->removeTo(toNode);
    }
    void remove(T vertex){
        //TODO
        typename AbstractGraph<T>::VertexNode *removeNode = this->getVertexNode(vertex);
        if (!removeNode)
            throw VertexNotFoundException("");

        DLinkedList<T> outEdges = this->getOutwardEdges(vertex);
        DLinkedList<T> inEdges  = this->getInwardEdges(vertex);

        for (auto adjnode : outEdges)
        {
            typename AbstractGraph<T>::VertexNode *adjNode = this->getVertexNode(adjnode);
            removeNode->removeTo(adjNode);
        }

        for (auto adjnode : inEdges)
        {
            typename AbstractGraph<T>::VertexNode *adjNode = this->getVertexNode(adjnode);
            adjNode->removeTo(removeNode);
        }

        AbstractGraph<T>::nodeList.removeItem(
            removeVertex, 
            [](typename AbstractGraph<T>::VertexNode *delVertex) { 
                delete delVertex; 
            }
        );
    }
    
    static DGraphModel<T>* create(
            T* vertices, int nvertices, Edge<T>* edges, int nedges,
            bool (*vertexEQ)(T&, T&),
            string (*vertex2str)(T&)){
        //TODO
        DGraphModel<T> *model = new DGraphModel<T>(vertexEQ, vertex2str);

        for (int i = 0; i < nvertices; i++)
            model->add(vertices[i]);

        for (int i = 0; i < nedges; i++)
            model->connect(edges[i].from, edges[i].to, edges[i].weight);

        return model;
    }
};

#endif /* DGRAPHMODEL_H */

