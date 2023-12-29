#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <chrono>

#include "MeshSim.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: inputMesh outputMesh reduceRatio" << std::endl;
        return -1;
    }

    std::string inputFN(argv[1]);
    std::string outputFN(argv[2]);
    double ratio = atof(argv[3]);

    std::ifstream in(argv[1]);
    if (!in.is_open()) {
        std::cerr << "Failed open input mesh file: " << inputFN << std::endl;
        return -1;
    }
    std::cout << "Start parsing mesh..." << std::endl;

    mmm::Mesh mesh(in);

    in.close();
    std::cout << "Finished." << std::endl;

    // Simplify the mesh.
    std::cout << "Start simplifying mesh..." << std::endl;
    size_t origin = mesh.getOldNumFaces();
    size_t remain = (size_t)(origin * ratio);

    clock_t start = clock();
    mesh.simplify(remain, 0.1);
    double duration = (clock() - start) / CLOCKS_PER_SEC;

    std::cout << "Finished: Simplified / Original = " << std::setw(6) <<
        (double)mesh.getNumFaces()  / (double)mesh.getOldNumFaces() << std::endl;
    std::cout << "Total time: " << std::setw(6) << duration << " sec." << std::endl;

    // Dump the output mesh.
    std::ofstream out(outputFN);
    if (!out.is_open()) {
        std::cerr << "Failed opening output file: " << outputFN << std::endl;
        return -1;
    }

    mesh.dumpObj(out);
    std::cout << "Saved to > " << outputFN << std::endl;
    out.close();

    return 0;
}
