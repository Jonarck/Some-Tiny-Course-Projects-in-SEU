/////////////////Solution////////////////////
////Name:JiangZhuoyang
////StudentID:58119125
////FinishDate:21/12/05
////****Reference:https://github.com/787624675/MSimplificationAndReM****
#include "MeshSim.h"
#include "Matrix.h"
#include <math.h>

namespace mmm {

    //1.MESH Load
    Mesh::Mesh(std::istream &in) {
        char c;
        while (in.get(c)) {
            if (c == '#') {
                char buf[1024];
                in.getline(buf, sizeof(buf));
                continue;
            }
            else if (c == '\n') {

            }
            else if (c == 'v') {
                double x;
                double y;
                double z;
                in >> x >> y >> z;

                verts.emplace_back(x, y, z);
            }
            else if (c == 'f') {
                int id[3];
                in >> id[0] >> id[1] >> id[2];

                if (vfTable.size() != verts.size()) {
                    faces.reserve(verts.size());
                    vfTable.resize(verts.size());
                    vRemoved.resize(verts.size(), false);
                    edges = new CrossLink((int)verts.size());
                }

                id[0]--; id[1]--; id[2]--;

                faces.emplace_back(id[0], id[1], id[2]);
                vfTable[id[0]].insert((int)faces.size() - 1);
                vfTable[id[1]].insert((int)faces.size() - 1);
                vfTable[id[2]].insert((int)faces.size() - 1);

                std::sort(id, id + 3);
                assert(0 <= id[0] && id[0] < id[1] && id[1] < id[2] && id[2] < verts.size());
                edges->insert(id[0], id[1]);
                edges->insert(id[1], id[2]);
                edges->insert(id[0], id[2]);

            }
            else {
                std::cerr << "Invalid Command: " << c << std::endl;
                continue;
            }
        }
        num_faces = faces.size();
        old_num_edges = edges->size();
        old_num_faces = faces.size();
        old_num_verts = verts.size();
    }

    Mesh::~Mesh() {
        verts.clear();
        vRemoved.clear();
        faces.clear();
    }

    //2.Dunmp
    void Mesh::dumpObj(std::ostream &out) const {
        size_t vertexN = verts.size();
        std::vector<int> vertexID(vertexN, 0);
        int vertexReal = 0;

        for (int i = 0; i < vertexN; i++) {
            if (vRemoved[i]) continue;
            vertexID[i] = ++vertexReal;
            out << "v " << verts[i][0] << " " << verts[i][1] << " " << verts[i][2] << std::endl;
        }

        for (int i = 0; i < vertexN; i++) {
            if (vRemoved[i]) continue;
            for (const auto &fId : vfTable[i]) {
                Edge edge = faces[fId].against(i);
                int v1 = edge.v1;
                int v2 = edge.v2;
                assert(!vRemoved[v1] && !vRemoved[v2]);
                assert(vertexID[v1] && vertexID[v2] && vertexID[i]);
                if (i < v1 && i < v2) {
                    out << "f " << vertexID[i] << ' ' << vertexID[v1] << ' ' << vertexID[v2] << std::endl;
                }
            }
        }
    }

    /* Get the split position of the edge. */
    std::pair<Vector, double> Mesh::getPosition(const Edge &e) {

        /* Get the Q matrix for all the faces. */
        Matrix q(0.0);

        auto calculate = [&](int vId) -> void {
            for (const auto &fId : vfTable[vId]) {
                Edge edge = faces[fId].against(vId);
                int v1 = edge.v1, v2 = edge.v2;
                auto n = crossProduct(verts[v1] - verts[vId], verts[v2] - verts[vId]);
                n = n / norm(n);
                n.push_back(-innerProduct(verts[vId], n));
                outerProductAcc(n, n, q);
            }
        };

        calculate(e.v1);
        calculate(e.v2);

        Vector v = (verts[e.v1] + verts[e.v2]) / 2;

        // Change this to vec4 and calculate the cost.
        v.push_back(1);
        double cost = innerProduct(innerProduct(v, q), v);
        assert(cost > -EPS);
        v.pop_back();
        return make_pair(v, cost);
    }


    /* Select an edge to collapse. */
    std::pair<Edge, Vector> Mesh::selectEdge(double threshold) {
        Edge idx(-1, -1);
        Vector pos;
        std::pair<double, Edge> tmp;
        while (!heap.empty()) {
            tmp = heap.top();
            heap.pop();

            // This edge is already deleted.
            if (!edges->find(tmp.second.v1, tmp.second.v2)) continue;

            // The verts of the edge has been deleted.
            if (vRemoved[tmp.second.v1] || vRemoved[tmp.second.v2]) continue;

            // The edge is too long.
            if (edgeLen(tmp.second) > threshold) continue;

            // Again we get the split position.
            auto act = getPosition(tmp.second);

            // Compare the new cost with the original one.
            if (fabs(act.second + tmp.first) > EPS) continue;

            // This is indeed a valid edge.
            idx = tmp.second;
            pos = act.first;
            break;
        }
        return std::make_pair(idx, pos);
    }

    /**
     * Check if after replacing v1 with v2, the normal of the triangle will be on the opposite direction.
     * The triangle looks like:
     *
     *      v1                  v2
     *     /  \                /  \
     *    /    \  ------>     /    \
     *   x ---- y            x ---- y
     *
     * @param e: the opposite edge.
     * @param v1: the old vert.
     * @param v2: the new vert.
     * @return bool: true if the face's normal changed, false otherwise.
     *
     */
    bool Mesh::faceReverse(const Edge &e, const Vector &v1, const Vector &v2) {
        const auto &x = verts[e.v1];
        const auto &y = verts[e.v2];
        return innerProduct(crossProduct(x - v1, y - v1), crossProduct(x - v2, y - v2)) < 0.0;
    }

    /* Add an edge to the heap. */
    void Mesh::addToHeap(const Edge &e, double threshold) {
        if (edgeLen(e) > threshold) return;

        // Get the split position.
        auto pos = getPosition(e);

        heap.emplace(-pos.second, e);
    }

    /* Update the neighbor edge and add them to the heap. */
    void Mesh::updateNeighborEdge(int v, double threshold) {
        std::set<int> neighbor;
        for (const auto &fId : vfTable[v]) {
            Edge edge = faces[fId].against(v);
            neighbor.insert(edge.v1);
            neighbor.insert(edge.v2);
        }
        for (auto x : neighbor) {
            addToHeap(Edge(min(x, v), max(x, v)), threshold);
        }
    }

    /**
     * Remove an edge from the mesh.
     * @param e: edge to be removed.
     * @param v: new position for v1.
     * @param threshold: maximum length for a removable edge.
     * @return void.
     */
    void Mesh::removeEdge(const Edge &e, const Vector &v, double threshold) {

        // First check if any face of v1 will be reversed after this removal.
        for (const auto &fId : vfTable[e.v1]) {
            Edge edge = faces[fId].against(e.v1);

            // If this is the face we will remove, simply ignore it.
            if (edge.v1 == e.v2 || edge.v2 == e.v2) continue;

            bool reverse = faceReverse(edge, verts[e.v1], v);
            if (!reverse) continue;

            // The face will be reversed after we replacing v1 with v.
            // Fix the orientation of the faces at face[f.v2] and face[f.v1].
            assert(vfTable[edge.v2].find(fId) != vfTable[edge.v2].end());
            assert(vfTable[edge.v1].find(fId) != vfTable[edge.v1].end());
            faces[fId].reverse();
        }

        /* Process the faces of v2. 
         *          v2
         *         /  \
         *        /    \
         *  edge.v1 - edge.v2
         */
        for (const auto &fId : vfTable[e.v2]) {
            
            /* Check if the face will be reverse. */
            Edge edge = faces[fId].against(e.v2);

            auto reverse = faceReverse(edge, verts[e.v2], v);
            assert(vfTable[edge.v2].find(fId) != vfTable[edge.v2].end());
            assert(vfTable[edge.v1].find(fId) != vfTable[edge.v1].end());
            if (edge.v1 != e.v1 && edge.v2 != e.v1) {
                // This isn't the face we will collapse.
                faces[fId].replace(e.v2, e.v1);
                vfTable[e.v1].insert(fId);
                if (reverse) {
                    faces[fId].reverse();
                }
            }
            else {
                vfTable[edge.v1].erase(fId);
                vfTable[edge.v2].erase(fId);
                num_faces--;
            }

            // Update the edge.
            if (edges->find(min(e.v2, edge.v1), max(e.v2, edge.v1)))
                edges->erase(min(e.v2, edge.v1), max(e.v2, edge.v1));
            if (edges->find(min(e.v2, edge.v2), max(e.v2, edge.v2)))
                edges->erase(min(e.v2, edge.v2), max(e.v2, edge.v2));
            if (edge.v1 != e.v1 && edge.v2 != e.v1) {
                edges->insert(min(e.v1, edge.v1), max(e.v1, edge.v1));
                edges->insert(min(e.v1, edge.v2), max(e.v1, edge.v2));
            }
        }

        edges->erase(e.v1, e.v2);
        verts[e.v1] = v;
        verts[e.v2].clear();
        vRemoved[e.v2] = true;
        vfTable[e.v2].clear();

        std::set<int> neighbor;     /* All the neighboring vert. */
        for (const auto &fId : vfTable[e.v1]) {
            Edge edge = faces[fId].against(e.v1);
            neighbor.insert(edge.v1);
            neighbor.insert(edge.v2);
        }
        for (auto nb : neighbor) {
            updateNeighborEdge(nb, threshold);
        }
    }

    /* Push all the valid edges into the heap. */
    void Mesh::buildHeap(double threshold) {

        // First clear the heap.
        while (!heap.empty()) heap.pop();

        for (int i = 0; i < edges->size(); ++i) {
            CrossLink::Node *node = edges->edgeX[i];
            while (node != nullptr) {
                addToHeap(Edge(node->x, node->y), threshold);
                node = node->dwn;
            }
        }
    }

    void Mesh::simplify(size_t target, double threshold) {

        // Build the heap.
        buildHeap(threshold);
        while (num_faces > target) {
            //  Select an edge to collapse.
            auto e = selectEdge(threshold);
            if (e.first.v1 != -1)
                removeEdge(e.first, e.second, threshold);
            else {
                // Failed getting an edge to deleted.
                // Double the threshold.
                threshold *= 2.0;
                std::cout << "Not enough edges, double the threshold.\n";
                buildHeap(threshold);
            }
        }
    }
};