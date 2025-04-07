// each mesh should be generated with std::shared_ptr<hhMesh> mesh = std::make_shared<hhMesh>();
#include <vector>
#include <string>
#include <functional>

#include <C:/OpenGL/glm/glm/glm.hpp>


struct attribData {
    int elementLength;
    std::shared_ptr<std::vector<float>> dataVec;
    std::shared_ptr<std::vector<unsigned int>> indices;
};

class hhMesh {
    public:

        std::unordered_map<std::string, attribData> data;

        unsigned int dimensionality;
        bool hasIndices = false;

        hhMesh(int dimension, 
            const std::function<std::unordered_map<std::string, attribData>(float A, float B, float C)>& generatorFunc, 
            float paramA, float paramB, float paramC);
        hhMesh(const std::string& filename);
        ~hhMesh() {}


        void computeNormals();
        void boundingBox(); // can just adapt some code i wrote for dataAnnotation 

        std::unordered_map<std::string, attribData>* getData();
};


hhMesh::hhMesh(int dimension, 
    const std::function<std::unordered_map<std::string, attribData>(float A, float B, float C)>& generatorFunc,
    float A, float B, float C) {
    
    if (dimension == 2 || dimension == 3) {
        dimensionality = dimension;

        data = generatorFunc(A, B, C);
        if (data.count("indices") > 0) {
            hasIndices = true;
        }
    } else {
        throw std::invalid_argument("invalid dimensionality provided");
    }
}
hhMesh::hhMesh(const std::string& filename) {
    std::cout << "will add loading from files soon" << std::endl;
}

std::unordered_map<std::string, attribData>* hhMesh::getData() {
    return &(data);
}

