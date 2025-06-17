#include "SandboxLayer.h"

void SandboxLayer::OnAttach()
{
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    m_skyboxShader.LoadShader("src/res/shaders/skybox.vert", "src/res/shaders/skybox.frag");
    m_lightingShader.LoadShader("src/res/shaders/lighting.vert", "src/res/shaders/lighting.frag");

    m_camera.SetProjection((16.0f / 9.0f), true);

    p_vbo = std::make_shared<VertexBuffer>();

    m_vao.GenBuffer();
    m_vao.Bind();
    p_vbo->GenBuffer(skyboxVertices, sizeof(skyboxVertices));
    p_vbo->Bind();
    VertexBufferLayout layout;
    // position attribute
    layout.Push<float>(3);
    m_vao.AddVertexBuffer(p_vbo, layout);

    m_skybox.GenTexture("src/res/skyboxes/ocean");
    m_skybox.Unbind();

    m_backpack = Model("src/res/objects/backpack/backpack.obj");
    std::vector<Vertex> vertices = {
        Vertex {
            glm::vec3(-8.0f, -5.0f, 8.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        },
        Vertex {
            glm::vec3(-8.0f, -5.0f, -8.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        },
        Vertex {
            glm::vec3(8.0f, -5.0f, -8.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        },
        Vertex {
            glm::vec3(8.0f, -5.0f, 8.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        },
    };
    std::vector<unsigned int> indices = {
        0, 1, 2,
        2, 3, 0
    };
    m_floorTexture.GenTexture("src/res/images/wall.jpg");
    std::vector<TextureData> textures = {
        TextureData {
            m_floorTexture.GetRendererID(),
            "texture_diffuse",
            "src/res/images/wall.jpg"
        }
    };
    m_floor = Mesh(vertices, indices, textures);

    m_skyboxShader.Use();
    m_skyboxShader.SetFloat3("globalLight.ambient", globalAmbient.x, globalAmbient.y, globalAmbient.z);
    m_skyboxShader.SetFloat("globalLight.intensity", globalIntensity);
    m_lightingShader.Use();
    m_lightingShader.SetFloat3("globalLight.ambient", globalAmbient.x, globalAmbient.y, globalAmbient.z);
    m_lightingShader.SetFloat("globalLight.intensity", globalIntensity);
    m_lightingShader.SetFloat3("directionalLight.ambient", dirAmbient.x, dirAmbient.y, dirAmbient.z);
    m_lightingShader.SetFloat3("directionalLight.diffuse", dirDiffuse.x, dirDiffuse.y, dirDiffuse.z);
    m_lightingShader.SetFloat3("directionalLight.specular", dirSpecular.x, dirSpecular.y, dirSpecular.z);
    m_lightingShader.SetFloat("directionalLight.intensity", dirIntensity);
}

void SandboxLayer::OnEvent(Event& event)
{
    m_camera.OnEvent(event);
}

void SandboxLayer::OnUpdate(float timestep)
{
    m_camera.OnUpdate(timestep);
}

void SandboxLayer::OnRender()
{
    m_skyboxShader.Use();
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_camera.GetCamera().GetPosition());
    m_skyboxShader.SetMat4f("modelViewProjection", m_camera.GetCamera().GetViewProjectionMatrix() * model);
    GLCall(glActiveTexture(GL_TEXTURE0));
    m_skybox.Bind();
    m_skyboxShader.SetInt("skybox", 0);
    m_vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glClear(GL_DEPTH_BUFFER_BIT);

    m_lightingShader.Use();
    m_lightingShader.SetMat4f("modelViewProjection", m_camera.GetCamera().GetViewProjectionMatrix());
    glm::vec3 camPos = m_camera.GetCamera().GetPosition();
    m_lightingShader.SetFloat3("cameraPosition", camPos.x, camPos.y, camPos.z);
    m_backpack.Draw(m_lightingShader);
    m_floor.Draw(m_lightingShader);
}

void SandboxLayer::OnImGuiRender()
{
    ImGui::Begin("Parameters");
    ImGui::Text("Global");
    if (ImGui::ColorEdit3("Global Ambient", (float*)(&globalAmbient))) {
        m_skyboxShader.Use();
        m_skyboxShader.SetFloat3("globalLight.ambient", globalAmbient.x, globalAmbient.y, globalAmbient.z);
        m_lightingShader.Use();
        m_lightingShader.SetFloat3("globalLight.ambient", globalAmbient.x, globalAmbient.y, globalAmbient.z);
    }
    if (ImGui::DragFloat("Global Intensity", &globalIntensity, 0.02f, 0.0f, 5.0f)) {
        m_skyboxShader.Use();
        m_skyboxShader.SetFloat("globalLight.intensity", globalIntensity);
        m_lightingShader.Use();
        m_lightingShader.SetFloat("globalLight.intensity", globalIntensity);
    }
    m_lightingShader.Use();
    ImGui::Text("Directional Light");
    if (ImGui::DragFloat3("Directional Rotation", (float*)(&dirDirection), 1.0f, -4960.0f, 4960.0f)) {
        float yaw = glm::radians(dirDirection.x);
        float pitch = glm::radians(dirDirection.y);

        glm::vec3 direction;
        direction.x = cos(pitch) * cos(yaw);
        direction.y = sin(pitch);
        direction.z = cos(pitch) * sin(yaw);

        direction = glm::normalize(direction);

        m_lightingShader.SetFloat3("directionalLight.direction", direction.x, direction.y, direction.z);
    }
    if (ImGui::ColorEdit3("Directional Ambient", (float*)(&dirAmbient))) {
        m_lightingShader.SetFloat3("directionalLight.ambient", dirAmbient.x, dirAmbient.y, dirAmbient.z);
    }
    if (ImGui::DragFloat3("Directional Diffuse", (float*)(&dirDiffuse), 0.001f, 0.0f, 1.0f)) {
        m_lightingShader.SetFloat3("directionalLight.diffuse", dirDiffuse.x, dirDiffuse.y, dirDiffuse.z);
    }
    if (ImGui::DragFloat3("Directional Specular", (float*)(&dirSpecular), 0.001f, 0.0f, 1.0f)) {
        m_lightingShader.SetFloat3("directionalLight.specular", dirSpecular.x, dirSpecular.y, dirSpecular.z);
    }
    if (ImGui::DragFloat("Directional Intensity", &dirIntensity, 0.001f, 0.0f, 5.0f)) {
        m_lightingShader.SetFloat("directionalLight.intensity", dirIntensity);
    }
    ImGui::End();
}

void SandboxLayer::OnDetach()
{
    p_vbo->Delete();
    m_vao.Delete();
    m_skybox.Delete();
    m_skyboxShader.Delete();
}
