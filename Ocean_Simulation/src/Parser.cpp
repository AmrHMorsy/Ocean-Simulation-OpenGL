#include "Parser.h"


Parser::Parser( progressbar * bar ):num_progress_bars(0)
{
    this->bar = bar ;
}

void Parser::parse( std::string filepath )
{
    max.clear() ;
    min.clear() ;
    indices.clear() ;
    vertex_faces.clear() ;
    texture_coordinates_faces.clear() ;
    vertex_normals_faces.clear() ;
    vertices.clear() ;
    vertices_wtextures.clear() ;
    vertex_normals.clear() ;
    texture_coordinates.clear() ;
    f.open(filepath) ;
    if( !f )
        generate_error("Failed to open the 3D model obj file") ;
    parse() ;
    f.close() ;
}

void Parser::parse()
{
    std::string line ;
    while( std::getline(f,line) ){
        if( (line[0]=='v') && (line[1]==' ') )
            parse_vertex(line) ;
        else if( (line[0]=='v') && (line[1]=='n') )
            parse_vertex_normal(line) ;
        else if( (line[0]=='v') && (line[1]=='t') )
            parse_texture_coordinate(line) ;
        else if( line[0]=='f' )
            parse_face(line) ;
    }
    update_progress_bar(15) ;
    merge() ;
}

void Parser::merge()
{
    for( int i = 0 ; i < vertex_faces.size() ; i++ ){
        for( int j = 0 ; j < 3 ; j++ )
            vertices_wtextures.push_back(vertices[(3*vertex_faces[i])+j]) ;
        for( int j = 0 ; j < 2 ; j++ )
            vertices_wtextures.push_back((texture_coordinates[(texture_coordinates_faces[i]*2)+j])) ;
        for( int j = 0 ; j < 3 ; j++ )
            vertices_wtextures.push_back(vertex_normals[(3*vertex_normals_faces[i])+j]) ;
        indices.push_back(i) ;
        if( i == vertex_faces.size()/2 )
            update_progress_bar(8) ;
    }
    update_progress_bar(7) ;
}

void Parser::parse_vertex( std::string vertex )
{
    char ignore ;
    float x, y, z ;
    std::stringstream s(vertex) ;
    s >> ignore ;
    s >> x ;
    s >> y ;
    s >> z ;
    vertices.push_back(x) ;
    vertices.push_back(y) ;
    vertices.push_back(z) ;
}

void Parser::parse_vertex_normal( std::string vertex_normal )
{
    std::string ignore ;
    float x, y, z ;
    std::stringstream s(vertex_normal) ;
    s >> ignore ;
    s >> x ;
    s >> y ;
    s >> z ;
    vertex_normals.push_back(x) ;
    vertex_normals.push_back(y) ;
    vertex_normals.push_back(z) ;
}

void Parser::parse_texture_coordinate( std::string texture_coordinate )
{
    std::string ignore ;
    float x, y ;
    std::stringstream s(texture_coordinate) ;
    s >> ignore ;
    s >> x ;
    s >> y ;
    texture_coordinates.push_back(x) ;
    texture_coordinates.push_back(y) ;
}

void Parser::parse_indices( std::string f, glm::vec4& v, glm::vec4& t, glm::vec4& n, int count )
{
    std::string tmp ;
    std::stringstream s(f) ;
    
    std::getline(s, tmp, '/') ;
    v[count] = std::stof(tmp) - 1 ;
    std::getline(s, tmp, '/') ;
    t[count] = std::stof(tmp) - 1 ;
    std::getline(s, tmp, '/') ;
    n[count] = std::stof(tmp) - 1 ;
}

void Parser::store_indices( glm::vec4 data, std::vector<unsigned int> &buffer )
{
    glm::vec3 v ;
    for( int i = 0 ; i < 3 ; i++ )
        v[i] = data[i] ;
    buffer.push_back((unsigned int)v.x) ;
    buffer.push_back((unsigned int)v.y) ;
    buffer.push_back((unsigned int)v.z) ;
    if( data.w == -1 )
        return ;
    for( int i = 1 ; i < 3 ; i++ )
        v[i] = data[i+1] ;
    buffer.push_back((unsigned int)v.x) ;
    buffer.push_back((unsigned int)v.y) ;
    buffer.push_back((unsigned int)v.z) ;
}

void Parser::parse_face( std::string face )
{
    int count = 0 ;
    char ignore ;
    glm::vec4 v, t, n ;
    glm::vec3 vertex, texture, normal ;
    std::string f ;
    std::stringstream s(face) ;
    v.w = -1 ;
    t.w = -1 ;
    n.w = -1 ;
    s >> ignore ;
    while( s >> f ){
        parse_indices( f, v, t, n, count ) ;
        count++ ;
    }
    store_indices(v,vertex_faces) ;
    store_indices(t,texture_coordinates_faces) ;
    store_indices(n,vertex_normals_faces) ;
}

void Parser::update_progress_bar( int num_bars )
{
    if( num_bars > num_progress_bars )
        return ;
    for( int i = 0 ; i < num_bars  ; i++ )
        bar->update() ;
    num_progress_bars -= num_bars ;
}

std::vector<float> Parser::get_vertices()
{
    return vertices_wtextures ;
}

std::vector<unsigned int> Parser::get_vertices_faces()
{
    return indices ;
}

std::vector<float> Parser::get_texture_coordinates()
{
    return texture_coordinates ;
}

int Parser::get_vertices_faces_size()
{
    return vertex_faces.size() ;
}

void Parser::generate_error( std::string msg )
{
    std::cout << msg << std::endl ;
    exit(1) ;
}

Parser::~Parser(){}
