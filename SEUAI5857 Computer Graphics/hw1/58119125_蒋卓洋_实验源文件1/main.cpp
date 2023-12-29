#include "Triangle.hpp"
#include "rasterizer.hpp"
#include <eigen3/Eigen/Eigen>
#include <iostream>
#include <opencv2/opencv.hpp>

constexpr double MY_PI = 3.1415926;

Eigen::Matrix4f get_view_matrix(Eigen::Vector3f eye_pos)
{
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();

    Eigen::Matrix4f translate;
    translate << 1, 0, 0, -eye_pos[0], 
                 0, 1, 0, -eye_pos[1], 
                 0, 0, 1, -eye_pos[2], 
                 0, 0, 0, 1;

    view = translate * view;

    return view;
}

Eigen::Matrix4f get_model_matrix(float rotation_angle)
{
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the model matrix for rotating the triangle around the Z axis.
    // Then return it.
    //////////////////Solution////////////////////
    ////Name:JiangZhuoyang
    ////StudentID:58119125
    ////FinishDate:21/9/30

    //1.Translation to origin: no movement
    Eigen::Matrix4f translate_pos;
    translate_pos << 1,0,0,0,
                     0,1,0,0,
                     0,0,1,0,
                     0,0,0,1;

    //2.Scale: no need
    Eigen::Matrix4f scale;
    scale << 1,0,0,0,
             0,1,0,0,
             0,0,1,0,
             0,0,0,1;

    //3.Rotation: main work WITH Z
    Eigen::Matrix4f rotation;
    float rotation_radian = rotation_angle * MY_PI / 180.0;//angle to radian
    rotation << cos(rotation_radian),-sin(rotation_radian),0,0,
                sin(rotation_radian),cos(rotation_radian),0,0,
                0,0,1,0,
                0,0,0,1;

    
    //4.Translation back: no movement
    Eigen::Matrix4f translate_neg;
    translate_neg << 1,0,0,0,
                     0,1,0,0,
                     0,0,1,0,
                     0,0,0,1;
    
    //5.Return model
    model = translate_neg * rotation * scale * translate_pos;
    /////////////////////////////////////////////
    
    return model;
}

Eigen::Matrix4f get_projection_matrix(float eye_fov, float aspect_ratio,
                                      float zNear, float zFar)
{
    // Students will implement this function

    Eigen::Matrix4f projection = Eigen::Matrix4f::Identity();

    // TODO: Implement this function
    // Create the projection matrix for the given parameters.
    // Then return it.
    //////////////////Solution////////////////////
    ////Name:JiangZhuoyang
    ////StudentID:58119125
    ////FinishDate:21/9/30
    //1.Definition
    Eigen::Matrix4f perspective = Eigen::Matrix4f::Identity();//perspective projection
    Eigen::Matrix4f persp_to_ortho = Eigen::Matrix4f::Identity();//turn the perspective projection to orthographic projection
    Eigen::Matrix4f orthographic = Eigen::Matrix4f::Identity();//orthographic projection

    //2.Construction
    //2.1.P -> O
    persp_to_ortho << zNear,0,0,0,
                      0,zNear,0,0,
                      0,0,zNear+zFar,-zNear*zFar,
                      0,0,1,0;

    //2.2.Orthographic
    //(1)Get edges: implicitly, do the first translation with the use of eye fov and aspect ration directly.
    float yTop = zNear * tan( (eye_fov/2) * MY_PI / 180 );
    float yBottom = (-1) * yTop;
    float xLeft = yTop * aspect_ratio;
    float xRigth = (-1) * xLeft;
    //(2)Orthographic translate:
    Eigen::Matrix4f ortho_trans = Eigen::Matrix4f::Identity();
    ortho_trans << 1, 0, 0, -(xRigth + xLeft) / 2, //(1,0,0,0)
                   0, 1, 0, -(yTop + yBottom) / 2, //(0,1,0,0)
                   0, 0, 1, -(zNear + zFar) / 2,
                   0, 0, 0, 1;
    //(3)Orthographic scale:
    Eigen::Matrix4f ortho_scale = Eigen::Matrix4f::Identity();
    ortho_scale << 2/(xRigth - xLeft), 0, 0, 0, //(1,0,0,0)
                   0, 2/(yTop - yBottom), 0, 0, //(0,1,0,0)
                   0, 0, 2/(zNear - zFar), 0,
                   0, 0, 0, 1;
    //(3)Orthographic Matrix:
    orthographic = ortho_scale * ortho_trans;

    //2.3.Perspective:
    perspective = orthographic * persp_to_ortho;

    //3.projection:
    projection = perspective;
    ///////////////////////////////////////////// 
    return projection;
}

int main(int argc, const char** argv)
{
    float angle = 0;
    bool command_line = false;
    std::string filename = "output.png";

    if (argc >= 3) {
        command_line = true;
        angle = std::stof(argv[2]); // -r by default
        if (argc == 4) {
            filename = std::string(argv[3]);
        }
        else
            return 0;
    }

    rst::rasterizer r(700, 700);

    Eigen::Vector3f eye_pos = {0, 0, 5};

    std::vector<Eigen::Vector3f> pos{{2, 0, -2}, {0, 2, -2}, {-2, 0, -2}};

    std::vector<Eigen::Vector3i> ind{{0, 1, 2}};

    auto pos_id = r.load_positions(pos);
    auto ind_id = r.load_indices(ind);

    int key = 0;
    int frame_count = 0;

    if (command_line) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);
        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);

        cv::imwrite(filename, image);

        return 0;
    }

    while (key != 27) {
        r.clear(rst::Buffers::Color | rst::Buffers::Depth);

        r.set_model(get_model_matrix(angle));
        r.set_view(get_view_matrix(eye_pos));
        r.set_projection(get_projection_matrix(45, 1, 0.1, 50));

        r.draw(pos_id, ind_id, rst::Primitive::Triangle);

        cv::Mat image(700, 700, CV_32FC3, r.frame_buffer().data());
        image.convertTo(image, CV_8UC3, 1.0f);
        cv::imshow("image", image);
        key = cv::waitKey(10);

        std::cout << "frame count: " << frame_count++ << '\n';

        if (key == 'a') {
            angle += 10;
        }
        else if (key == 'd') {
            angle -= 10;
        }
    }

    return 0;
}
