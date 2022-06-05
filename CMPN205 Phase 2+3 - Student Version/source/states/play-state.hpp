#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <systems/player-controller.hpp>
#include <systems/collision-controller.hpp>
#include <imgui.h>

// This state shows how to use the ECS framework and deserialization.
class Playstate: public our::State {

    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::PlayerControllerSystem playerController;
    our::CollisionControllerSystem collisionController;

    void onInitialize() override {
        // First of all, we get the scene configuration from the app config
        auto& config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if(config.contains("assets")){
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if(config.contains("world")){
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        playerController.enter(getApp());
        collisionController.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override {
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        playerController.update(&world, (float)deltaTime);
        collisionController.update(&world, (float)deltaTime);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);
    }

    void onDestroy() override {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
    void onImmediateGui() override
    {
        ImGui::Begin("score" , 0 ,  ImGuiWindowFlags_NoMove);
        // setting window position 
        ImGui::SetWindowPos(ImVec2(950,50));
        // setting window size
        ImGui::SetWindowSize(ImVec2(150,80));

        // writing text to window 
        ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f) , "Player 1   : %d" , getApp()->player1_score);
        ImGui::End();

        ImGui::Begin("score" , 0 ,   ImGuiWindowFlags_NoMove);
        // setting window position 
        ImGui::SetWindowPos(ImVec2(950,50));
        // setting window size
        ImGui::SetWindowSize(ImVec2(175,80));

        // writing text to window 
        ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f) , "Player 2   : %d" , getApp()->player2_score);
        
        ImGui::End();
    }
};