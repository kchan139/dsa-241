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
        typename AbstractGraph<T>::VertexNode* fromNode = this->getVertexNode(from);
        typename AbstractGraph<T>::VertexNode* toNode = this->getVertexNode(to);

        if (!fromNode)
            throw VertexNotFoundException(string(1, from));
        if (!toNode)
            throw VertexNotFoundException(string(1, to));

        fromNode->connect(toNode, weight);
    }
    void disconnect(T from, T to){
        //TODO
    }
    void remove(T vertex){
        //TODO
    }
    
    static DGraphModel<T>* create(
            T* vertices, int nvertices, Edge<T>* edges, int nedges,
            bool (*vertexEQ)(T&, T&),
            string (*vertex2str)(T&)){
        //TODO
    }
};

#endif /* DGRAPHMODEL_H */

