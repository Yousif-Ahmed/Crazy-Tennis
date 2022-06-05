#pragma once

#include <application.hpp>
#include <mesh/mesh.hpp>
#include <shader/shader.hpp>
#include <texture/texture-utils.hpp>
#include <texture/texture2d.hpp>

// This state tests and shows how to use the Texture2D class.
class WinMenuState : public our::State {
  our::ShaderProgram* shader;
  our::Mesh* mesh;
  our::Texture2D* texture1;
  our::Texture2D* texture2;
  void onInitialize() override {
    // First of all, we get the scene configuration from the app config
    auto& config = getApp()->getConfig()["scene"];
    // Then we load the shader that will be used for this scene
    shader = new our::ShaderProgram();
    shader->attach("assets/shaders/texture-test.vert", GL_VERTEX_SHADER);
    shader->attach("assets/shaders/texture-test.frag", GL_FRAGMENT_SHADER);
    shader->link();

    // We create a simple 2D plane to use for viewing the plane
    std::vector<our::Vertex> vertices = {
        {{-1, -1, 0}, {255, 255, 255, 255}, {0.00, 0.00}, {0, 0, 1}},
        {{1, -1, 0}, {255, 255, 255, 255}, {1.00, 0.00}, {0, 0, 1}},
        {{1, 1, 0}, {255, 255, 255, 255}, {1.00, 1.00}, {0, 0, 1}},
        {{-1, 1, 0}, {255, 255, 255, 255}, {0.00, 1.00}, {0, 0, 1}},
    };
    std::vector<unsigned int> elements = {
        0, 1, 2, 2, 3, 0,
    };
    mesh = new our::Mesh(vertices, elements);

    // Then we create a texture and load an image into it
    texture1 = our::texture_utils::loadImage(
        config["assets"]["textures"]["player1wins"]);
    texture2 = our::texture_utils::loadImage(
        config["assets"]["textures"]["player2wins"]);
  }

  void onDraw(double deltaTime) override {
    glClear(GL_COLOR_BUFFER_BIT);
    shader->use();
    // Here we set the active texture unit to 0 then bind the texture to it
    glActiveTexture(GL_TEXTURE0);
    if (getApp()->winner == 1)
      texture1->bind();
    else if (getApp()->winner == 2)
      texture2->bind();
    // Then we send 0 (the index of the texture unit we used above) to the "tex"
    // uniform
    shader->set("tex", 0);
    mesh->draw();

    //
    if (getApp()->getKeyboard().isPressed(GLFW_KEY_ENTER)) {
      getApp()->registerState<MainMenuState>("menu");
      getApp()->changeState("menu");
    }
  }

  void onDestroy() override {
    delete shader;
    delete mesh;
    delete texture2;
    delete texture1;
  }
};