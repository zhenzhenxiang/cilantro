#include <cilantro/convex_hull.hpp>
#include <cilantro/ply_io.hpp>
#include <cilantro/visualizer.hpp>
#include <cilantro/pca.hpp>
#include <iostream>

int main(int argc, char ** argv) {

//    std::vector<Eigen::Vector3f> points;
//    points.push_back(Eigen::Vector3f(0,0,0));
//    points.push_back(Eigen::Vector3f(1,0,0));
//    points.push_back(Eigen::Vector3f(0,1,0));
//    points.push_back(Eigen::Vector3f(0,0,1));
//    points.push_back(Eigen::Vector3f(0,1,1));
//    points.push_back(Eigen::Vector3f(1,0,1));
//    points.push_back(Eigen::Vector3f(1,1,0));
//    points.push_back(Eigen::Vector3f(1,1,1));
//    points.push_back(Eigen::Vector3f(0.5,0.5,0.5));
//
//    ConvexHull3D ch1(points);
//    std::vector<Eigen::Vector4f> hs1 = ch1.getFacetHyperplanes();
//
//    std::vector<Eigen::Vector3f> points2(points.size());
//    for (size_t i = 0; i < points.size(); i++) {
//        points2[i] = points[i] + Eigen::Vector3f(2,0,0);
//    }
//
//    ConvexHull3D ch2(points2);
//    std::vector<Eigen::Vector4f> hs2 = ch2.getFacetHyperplanes();
//
//    std::vector<Eigen::Vector4f> hs;
//    for (size_t i = 0; i < hs1.size(); i++) hs.push_back(hs1[i]);
//    for (size_t i = 0; i < hs2.size(); i++) hs.push_back(hs2[i]);
//
//    ConvexHull3D ch(hs);
//
//    std::cout << ch1.isEmpty() << std::endl;
//    std::cout << ch2.isEmpty() << std::endl;
//    std::cout << ch.isEmpty() << std::endl;

//    ConvexHull<float,float,4> chd(ch.getFacetHyperplanes());

//    PointCloud cloud;
//    readPointCloudFromPLYFile(argv[1], cloud);
//
//    CloudHull ch(cloud);
//
//    Visualizer viz("win", "disp");
//
//    viz.addPointCloud("cloud", cloud, Visualizer::RenderingProperties().setOpacity(1.0));
//    viz.addTriangleMesh("mesh", ch.getVertices(), ch.getFacetVertexIndices(), Visualizer::RenderingProperties().setDrawingColor(1,0,0).setUseFaceNormals(true).setOpacity(0.8));
//
//    while (!viz.wasStopped()) {
//        viz.spinOnce();
//    }

    PointCloud cloud;
    readPointCloudFromPLYFile(argv[1], cloud);

    PCA3D proj(cloud.points);

    std::vector<Eigen::Vector2f> pts = proj.project<2>(cloud.points);
    cloud.points = proj.reconstruct<2>(pts);

    CloudHullFlat ch2d(cloud);

    Visualizer viz("win", "disp");

    std::vector<std::vector<size_t> > face_v_ind = ch2d.getFacetVertexIndices();
    std::vector<Eigen::Vector3f> p_src(face_v_ind.size()), p_dst(face_v_ind.size());
    for (size_t i = 0; i < face_v_ind.size(); i++) {
        p_src[i] = ch2d.getVertices3D()[face_v_ind[i][0]];
        p_dst[i] = ch2d.getVertices3D()[face_v_ind[i][1]];
    }

    viz.addPointCloud("cloud", cloud, Visualizer::RenderingProperties().setOpacity(0.5));
    viz.addPointCloud("hull_cloud", ch2d.getVertices3D(), Visualizer::RenderingProperties().setDrawingColor(1,0,0).setPointSize(20.0));
    viz.addPointCorrespondences("hull_lines", p_src, p_dst, Visualizer::RenderingProperties().setDrawingColor(1,0,0).setLineWidth(10.0).setCorrespondencesFraction(1.0));

    while (!viz.wasStopped()) {
        viz.spinOnce();
    }

    return 0;
}
