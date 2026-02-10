#include "../Header/HUD.h"
#include "../Header/Log.h"
#include "../Shader.h"
#include "../Header/stb_image.h"
#include <GL/glew.h>

HUD::HUD()
    : m_VAO(0)
    , m_VBO(0)
    , m_texture(0)
    , m_shader(nullptr)
    , m_screenWidth(0)
    , m_screenHeight(0)
    , m_initialized(false)
{
}

HUD::~HUD()
{
    shutdown();
}

void HUD::init(int screenWidth, int screenHeight)
{
    LOG_INFO("[HUD] Initializing HUD overlay...");
    
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    
    
    m_shader = new Shader("Assets/Shaders/hud.vert", "Assets/Shaders/hud.frag");
    if (m_shader->ID == 0)
    {
        LOG_ERROR("[HUD] Failed to create HUD shader!");
        return;
    }
    
    LOG_INFO("[HUD] Shader created successfully (ID: " + std::to_string(m_shader->ID) + ")");
    
    
    loadTexture("Assets/Textures/hud_nametag.png");
    
    
    createQuad();
    
    m_initialized = true;
    LOG_INFO("[HUD] HUD initialized successfully");
}

void HUD::loadTexture(const char* path)
{
    LOG_INFO("[HUD] Loading texture: " + std::string(path));
    
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* data = stbi_load(path, &width, &height, &channels, 4); 
    
    if (!data)
    {
        LOG_ERROR("[HUD] Failed to load texture: " + std::string(path));
        return;
    }
    
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    
    LOG_INFO("[HUD] Texture loaded successfully (ID: " + std::to_string(m_texture) + 
             ", Size: " + std::to_string(width) + "x" + std::to_string(height) + ")");
}

void HUD::createQuad()
{
    
    
    float hudWidth = 300.0f;
    float hudHeight = 100.0f;
    
    
    float ndcWidth = (hudWidth / m_screenWidth) * 2.0f;
    float ndcHeight = (hudHeight / m_screenHeight) * 2.0f;
    
    
    float paddingX = 0.02f; 
    float paddingY = 0.02f; 
    
    float right = 1.0f - paddingX;
    float left = right - ndcWidth;
    float bottom = -1.0f + paddingY;
    float top = bottom + ndcHeight;
    
    
    float vertices[] = {
        
        left,  bottom,      0.0f, 0.0f,  
        right, bottom,      1.0f, 0.0f,  
        right, top,         1.0f, 1.0f,  
        
        left,  bottom,      0.0f, 0.0f,  
        right, top,         1.0f, 1.0f,  
        left,  top,         0.0f, 1.0f   
    };
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
    
    LOG_INFO("[HUD] Quad created (VAO: " + std::to_string(m_VAO) + ", VBO: " + std::to_string(m_VBO) + ")");
}

void HUD::draw()
{
    if (!m_initialized || !m_shader || m_VAO == 0 || m_texture == 0)
        return;
    
    
    GLboolean depthTestWasEnabled = glIsEnabled(GL_DEPTH_TEST);
    GLboolean blendWasEnabled = glIsEnabled(GL_BLEND);
    GLint blendSrcRGB, blendDstRGB, blendSrcAlpha, blendDstAlpha;
    glGetIntegerv(GL_BLEND_SRC_RGB, &blendSrcRGB);
    glGetIntegerv(GL_BLEND_DST_RGB, &blendDstRGB);
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &blendSrcAlpha);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &blendDstAlpha);
    
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    m_shader->use();
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    m_shader->setInt("hudTexture", 0);
    
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    
    if (depthTestWasEnabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);
    
    if (blendWasEnabled)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
    
    glBlendFuncSeparate(blendSrcRGB, blendDstRGB, blendSrcAlpha, blendDstAlpha);
}

void HUD::shutdown()
{
    if (m_VAO != 0)
    {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
    
    if (m_VBO != 0)
    {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    
    if (m_texture != 0)
    {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
    
    if (m_shader)
    {
        delete m_shader;
        m_shader = nullptr;
    }
    
    m_initialized = false;
    LOG_INFO("[HUD] HUD shutdown complete");
}
