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
    m_shadowShader.LoadShader("src/res/shaders/shadow.vert", "src/res/shaders/shadow.frag");
    m_dirShadowMap.GenBuffer({
        1600, 900,
        FrameBufferAttachments::DEPTH 
        });
    m_spotShadowMap.GenBuffer({
        1600, 900,
        FrameBufferAttachments::DEPTH
        });

    float aspect = 16.0f / 9.0f;
    m_camera.SetProjection((16.0f / 9.0f), 60.0f);
    m_camera.GetCamera().SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    m_spotCamera.SetProjection(spotOuterAngle * 2, aspect, 0.1f, 100.0f);
    m_shadowOrtho.SetProjection(-aspect * 10.0f, aspect * 10.0f, -1.0f * 10.0f, 1.0f * 10.0f, -30.0f, 30.0f);

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
    m_lightingShader.SetFloat3("spotLight.position", spotPosition.x, spotPosition.y, spotPosition.z);
    m_lightingShader.SetFloat3("spotLight.ambient", spotAmbient.x, spotAmbient.y, spotAmbient.z);
    m_lightingShader.SetFloat3("spotLight.diffuse", spotDiffuse.x, spotDiffuse.y, spotDiffuse.z);
    m_lightingShader.SetFloat3("spotLight.specular", spotSpecular.x, spotSpecular.y, spotSpecular.z);
    m_lightingShader.SetFloat("spotLight.intensity", spotIntensity);
    m_lightingShader.SetFloat("spotLight.cutoffAngle", glm::cos(glm::radians(spotAngle)));
    m_lightingShader.SetFloat("spotLight.outerAngle", glm::cos(glm::radians(spotOuterAngle)));

    float yaw = glm::radians(dirDirection.y);
    float pitch = glm::radians(dirDirection.x);

    glm::vec3 direction;
    direction.x = cos(pitch) * cos(yaw);
    direction.y = sin(pitch);
    direction.z = cos(pitch) * sin(yaw);

    direction = glm::normalize(direction);

    m_lightingShader.SetFloat3("directionalLight.direction", direction.x, direction.y, direction.z);

    pitch = glm::radians(spotDirection.x);
    yaw = glm::radians(spotDirection.y);

    direction.x = cos(pitch) * cos(yaw);
    direction.y = sin(pitch);
    direction.z = cos(pitch) * sin(yaw);

    direction = glm::normalize(direction);

    m_lightingShader.SetFloat3("spotLight.direction", direction.x, direction.y, direction.z);
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
    GLCall(glDepthMask(GL_FALSE));
    m_skyboxShader.Use();
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_camera.GetCamera().GetPosition());
    m_skyboxShader.SetMat4f("modelViewProjection", m_camera.GetCamera().GetViewProjectionMatrix() * model);
    GLCall(glActiveTexture(GL_TEXTURE0));
    m_skybox.Bind();
    m_skyboxShader.SetInt("skybox", 0);
    m_vao.Bind();
    GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
    GLCall(glDepthMask(GL_TRUE));

    GLCall(glClear(GL_DEPTH_BUFFER_BIT));

    m_dirShadowMap.Bind();
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));
    glDepthMask(GL_TRUE);
    m_shadowShader.Use();
    m_shadowOrtho.SetPosition(glm::vec3(0, 0.0f, 0));
    m_shadowOrtho.SetRotation(dirDirection);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -5.0f, 0.0f));
    m_shadowShader.SetMat4f("modelViewProjection", m_shadowOrtho.GetViewProjectionMatrix() * model);
    m_floor.Draw(m_shadowShader);
    model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    m_shadowShader.SetMat4f("modelViewProjection", m_shadowOrtho.GetViewProjectionMatrix() * model);
    m_backpack.Draw(m_shadowShader);
    m_dirShadowMap.Unbind();

    m_spotShadowMap.Bind();
    GLCall(glClear(GL_DEPTH_BUFFER_BIT));
    glDepthMask(GL_TRUE);
    m_shadowShader.Use();
    m_spotCamera.SetRotation(glm::vec3(spotDirection.x - 180.0f, spotDirection.y, 0.0f));
    m_spotCamera.SetPosition(spotPosition);
    m_shadowShader.SetMat4f("modelViewProjection", m_spotCamera.GetViewProjectionMatrix());
    m_floor.Draw(m_shadowShader);
    m_backpack.Draw(m_shadowShader);
    m_spotShadowMap.Unbind();

    m_lightingShader.Use();
    GLCall(glActiveTexture(GL_TEXTURE8));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_dirShadowMap.GetDepthAttachment()));
    m_lightingShader.SetInt("directionalLight_shadowMap", 8);
    GLCall(glActiveTexture(GL_TEXTURE7));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_spotShadowMap.GetDepthAttachment()));
    m_lightingShader.SetInt("spotLight_shadowMap", 7);
    m_lightingShader.SetMat4f("modelViewProjection", m_camera.GetCamera().GetViewProjectionMatrix());
    m_lightingShader.SetMat4f("dirShadowViewProjection", m_shadowOrtho.GetViewProjectionMatrix());
    m_lightingShader.SetMat4f("spotShadowViewProjection", m_spotCamera.GetViewProjectionMatrix());
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
        float yaw = glm::radians(dirDirection.y);
        float pitch = glm::radians(dirDirection.x);

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
    ImGui::Text("Spot Light");
    if (ImGui::DragFloat3("SpotLight Position", (float*)(&spotPosition), 0.05f, -100.0f, 100.0f)) {
        m_lightingShader.SetFloat3("spotLight.position", spotPosition.x, spotPosition.y, spotPosition.z);
    }
    if (ImGui::DragFloat3("SpotLight Rotation", (float*)(&spotDirection), 1.0f, -4960.0f, 4960.0f)) {
        float pitch = glm::radians(spotDirection.x);
        float yaw = glm::radians(spotDirection.y);

        glm::vec3 direction;
        direction.x = cos(pitch) * cos(yaw);
        direction.y = sin(pitch);
        direction.z = cos(pitch) * sin(yaw);

        direction = glm::normalize(direction);

        m_lightingShader.SetFloat3("spotLight.direction", direction.x, direction.y, direction.z);
    }
    if (ImGui::ColorEdit3("SpotLight Ambient", (float*)(&spotAmbient))) {
        m_lightingShader.SetFloat3("spotLight.ambient", spotAmbient.x, spotAmbient.y, spotAmbient.z);
    }
    if (ImGui::DragFloat3("SpotLight Diffuse", (float*)(&spotDiffuse), 0.001f, 0.0f, 1.0f)) {
        m_lightingShader.SetFloat3("spotLight.diffuse", spotDiffuse.x, spotDiffuse.y, spotDiffuse.z);
    }
    if (ImGui::DragFloat3("SpotLight Specular", (float*)(&spotSpecular), 0.001f, 0.0f, 1.0f)) {
        m_lightingShader.SetFloat3("spotLight.specular", spotSpecular.x, spotSpecular.y, spotSpecular.z);
    }
    if (ImGui::DragFloat("SpotLight Intensity", &spotIntensity, 0.001f, 0.0f, 5.0f)) {
        m_lightingShader.SetFloat("spotLight.intensity", spotIntensity);
    }
    if (ImGui::DragFloat("SpotLight Angle", &spotAngle, 1.0f, 0.0f, 180.0f)) {
        m_lightingShader.SetFloat("spotLight.cutoffAngle", glm::cos(glm::radians(spotAngle)));
    }
    if (ImGui::DragFloat("SpotLight Outer Angle", &spotOuterAngle, 1.0f, 0.0f, 180.0f)) {
        m_lightingShader.SetFloat("spotLight.outerAngle", glm::cos(glm::radians(spotOuterAngle)));
        m_spotCamera.SetProjection(spotOuterAngle * 2, (16.0f / 9.0f), 0.1f, 100.0f);
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
