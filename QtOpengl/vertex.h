#ifndef VERTEX_H
#define VERTEX_H
#include <QVector2D>
#include <QVector3D>

class Vertex{
/*
public:
    Vertex();
    Vertex(const QVector3D &position): position(position){}
    Vertex(const QVector3D &position, const QVector3D &color): position(position), color(color){}

    // 先頭から attribute へのバイト数
    static int positionOffset(){return offsetof(Vertex, position);}
    static int colorOffset(){return offsetof(Vertex, color);}
    // Vertex 1つ分のサイズ
    static int stride(){return sizeof(Vertex);}

private:
    QVector3D position;
    QVector3D color;
*/

// 2
public:
    Vertex(const QVector3D& position, const QVector2D& texCoord){
        this->position = position;
        this->texCoord = texCoord;
    }

    QVector3D* GetPosition() { return &position; }
    QVector2D* GetTexCoord() { return &texCoord; }

    // 先頭から attribute へのバイト数
    static int positionOffset(){return offsetof(Vertex, position);}
    static int texCoordOffset(){return offsetof(Vertex, texCoord);}
    // Vertex 1つ分のサイズ
    static int stride(){return sizeof(Vertex);}

private:
    QVector3D position;
    QVector2D texCoord;

// 3
/*
public:
    Vertex(const QVector3D& position, const QVector2D& texCoord, const QVector3D& normal){
        this->position = position;
        this->texCoord = texCoord;
        this->normal = normal;
    }

    QVector3D* GetPosition() { return &position; }
    QVector2D* GetTexCoord() { return &texCoord; }
    QVector3D* GetNormal() { return &normal; }

    // 先頭から attribute へのバイト数
    static int positionOffset(){return offsetof(Vertex, position);}
    static int texCoordOffset(){return offsetof(Vertex, texCoord);}
    static int normalOffset(){return offsetof(Vertex, normal);}
    // Vertex 1つ分のサイズ
    static int stride(){return sizeof(Vertex);}

private:
    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
*/
};

#endif // VERTEX_H
