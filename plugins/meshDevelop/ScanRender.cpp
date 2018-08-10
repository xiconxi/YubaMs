#include "ScanRender.h"
#include <functional>
#include <easylogging++.h>
#include <controller>
#include <mesh_proc>

#include <QStandardPaths>
#include <fstream>
#include <numeric>

ScanRender::ScanRender():RenderScript(std::bind(&ScanRender::scan_line_animation,this,std::placeholders::_1)){
    RenderScript([this](QTime& t){
        auto mesh = con<MeshCtrl>().mesh("scanbody");
//        orient = glm::normalize(glm::mat3(glm::inverse(mesh->model))*glm::vec3(0,1,0));

        this->dot_v = glm::vec2(glm::dot(orient, mesh->v[(*mesh->f.begin())[0]].mv), glm::dot(orient, mesh->v[(*std::prev(mesh->f.end()))[1]].mv));
        LOG(INFO) << "dot: " << this->dot_v[0] << ' ' << this->dot_v[1];

//        auto he = halfEdge(mesh, mesh->f.begin(), mesh->f.end());

//        std::fstream fileHandle;
//        fileHandle.open(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation).toStdString()+"/a.obj",std::ifstream::out);
////        for(float percent = 0.1; percent < 0.9; percent += 0.02)
//        float percent = 1-0.72;
//        for(auto line:he.get_slice(orient, this->dot_v[0]+(this->dot_v[1]-this->dot_v[0])*percent,(this->dot_v[1]-this->dot_v[0])*0.05)) {
//            fileHandle << "v " << line[0][0] << ' ' << line[0][1] << ' ' << line[0][2] << std::endl;
//            fileHandle << "v " << line[1][0] << ' ' << line[1][1] << ' ' << line[1][2] << std::endl;
//        }
//        fileHandle.close();
    });
}

void ScanRender::scan_line_animation(QTime& t) {
    gl.glEnable(GL_DEPTH_TEST);
    gl.glClearColor(0.2, 0.3, 0.3, 1.0);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto shader = con<ShaderCtrl>().shader("base");
    auto mesh = con<MeshCtrl>().mesh("scanbody");
    auto view = con<ViewCtrl>().view();

    shader->bind();
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("model", view->Model()*mesh->Model());
    auto v = glm::vec4(orient,dot_v[0]+(dot_v[1]-dot_v[0])*percent);
    shader->setUniformValue("orient", v[0],v[1],v[2],v[3]);
    auto p = std::lower_bound(mesh->f.begin(),mesh->f.end(), dot_v[0]+(dot_v[1]-dot_v[0])*percent,
            [=](glm::ivec3& e1,float v){
        return v < glm::dot(mesh->v[e1[0]].mv, orient);
    });

    mesh->drawElements(0, (p - mesh->f.begin())); //
    shader->release();

    percent += 1e-3;
    percent = percent > 1.0 ? 0: percent;
    con<RenderCtrl>().update(true);

}

void ScanRender::draw_model(QTime &t) {
    gl.glEnable(GL_DEPTH_TEST);
    gl.glClearColor(0.2, 0.3, 0.3, 1.0);
    gl.glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    auto shader = con<ShaderCtrl>().shader("base");
    auto mesh = con<MeshCtrl>().mesh("scanbody");
    auto view = con<ViewCtrl>().view();

    shader->bind();
    shader->setUniformValue("camera_vp", view->MatrixVP());
    shader->setUniformValue("model", view->Model()*mesh->Model());
    auto p1 = std::lower_bound(mesh->f.begin(),mesh->f.end(), dot_v[0]+(dot_v[1]-dot_v[0])*(percent-0.01),
            [=](glm::ivec3& e1,float v){
        return v < glm::dot(mesh->v[e1[0]].mv, glm::mat3(glm::inverse(mesh->model))*glm::vec3(0,1,0));
    });

    auto p2 = std::lower_bound(mesh->f.begin(),mesh->f.end(), dot_v[0]+(dot_v[1]-dot_v[0])*(percent+0.01),
            [=](glm::ivec3& e1,float v){
        return v < glm::dot(mesh->v[e1[0]].mv, glm::mat3(glm::inverse(mesh->model))*glm::vec3(0,1,0));
    });

    mesh->drawElements( (p1 - mesh->f.begin()), (p2 - p1));

    shader->release();
}