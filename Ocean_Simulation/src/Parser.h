#include "Includes.h"




class Parser{
    
private:
    
    std::ifstream f ;
    progressbar * bar ;
    int num_progress_bars ;
    std::vector<float> max, min ;
    std::vector<unsigned int> indices ;
    std::vector<unsigned int> vertex_faces ;
    std::vector<unsigned int> texture_coordinates_faces ;
    std::vector<unsigned int> vertex_normals_faces ;
    std::vector<float> vertices ;
    std::vector<float> vertices_wtextures ;
    std::vector<float> vertex_normals ;
    std::vector<float> texture_coordinates ;
    
public:
    
    Parser( progressbar * bar ) ;
    void parse( std::string filepath ) ;
    void parse() ;
    void merge() ;
    void parse_vertex( std::string vertex ) ;
    void parse_vertex_normal( std::string vertex_normal ) ;
    void parse_texture_coordinate( std::string texture_coordinate ) ;
    void parse_face( std::string face ) ;
    void parse_indices( std::string f, glm::vec4& v, glm::vec4& t, glm::vec4& n, int count ) ;
    void store_indices( glm::vec4 data, std::vector<unsigned int> &buffer ) ;
    int get_vertices_faces_size() ;
    void update_progress_bar( int num_bars ) ;
    void generate_error( std::string msg ) ;
    std::vector<float> get_vertices() ;
    std::vector<unsigned int> get_vertices_faces() ;
    std::vector<float> get_texture_coordinates() ;
    ~Parser() ;
} ;

