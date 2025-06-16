#include "SandboxLayer.h"

SandboxLayer::SandboxLayer()
	:m_camera(16.0f / 9.0f, 60.0f)
{
}

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
    m_basicShader.LoadShader("src/res/shaders/basic.vert", "src/res/shaders/basic.frag");

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

    backpack = Model("src/res/objects/backpack/backpack.obj");
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

    m_basicShader.Use();
    m_basicShader.SetMat4f("modelViewProjection", m_camera.GetCamera().GetViewProjectionMatrix());
    backpack.Draw(m_basicShader);
}

void SandboxLayer::OnImGuiRender()
{
}

void SandboxLayer::OnDetach()
{
    p_vbo->Delete();
    m_vao.Delete();
    m_skybox.Delete();
    m_skyboxShader.Delete();
}
