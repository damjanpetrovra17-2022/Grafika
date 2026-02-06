#include "../Header/Screen.h"
#include "../Header/Log.h"
#include "../Shader.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "../Header/stb_image.h"

#include <string>
#include <sstream>
#include <iomanip>

Screen::Screen()
    : m_whiteTexture(0)
    , m_playing(false)
    , m_timer(0.0f)
    , m_currentFrame(0)
    , m_debugUV(false)
    , m_forceSolidColor(false)
    , m_debugOverlay(false)
    , m_VAO(0)
    , m_VBO(0)
    , m_overlayVAO(0)
    , m_overlayVBO(0)
    , m_shader(nullptr)
    , m_position(0.0f, 3.0f, -8.8f)  // Front wall at -9.0, screen slightly in front
    , m_size(10.0f, 5.0f)
{
}

Screen::~Screen()
{
    if (m_whiteTexture)
        glDeleteTextures(1, &m_whiteTexture);
    
    if (!m_filmTextures.empty())
        glDeleteTextures(static_cast<GLsizei>(m_filmTextures.size()), m_filmTextures.data());
    
    if (m_VAO)
        glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO)
        glDeleteBuffers(1, &m_VBO);
    if (m_overlayVAO)
        glDeleteVertexArrays(1, &m_overlayVAO);
    if (m_overlayVBO)
        glDeleteBuffers(1, &m_overlayVBO);
    
    delete m_shader;
}

void Screen::init()
{
    loadFilmTextures();
    createWhiteTexture();
    setupScreenQuad();
    
    m_shader = new Shader("Assets/Shaders/screen.vert", "Assets/Shaders/screen.frag");
    
    if (!m_shader || m_shader->ID == 0)
    {
        LOG_ERROR("[SCREEN] Failed to create shader!");
        return;
    }
    
    LOG_INFO("[SCREEN] Initialized with " + std::to_string(m_filmTextures.size()) + " film frames");
}

void Screen::loadFilmTextures()
{
    m_filmTextures.clear();
    
    for (int i = 1; i <= 100; ++i)
    {
        std::ostringstream oss;
        oss << "Assets/Textures/" << std::setfill('0') << std::setw(3) << i << ".png";
        std::string filename = oss.str();
        
        unsigned int texture = loadTexture(filename.c_str());
        if (texture != 0)
        {
            m_filmTextures.push_back(texture);
        }
    }
    
    if (m_filmTextures.empty())
    {
        LOG_INFO("[SCREEN] No film textures found - using white screen");
    }
}

void Screen::createWhiteTexture()
{
    unsigned char whitePixel[4] = { 255, 255, 255, 255 };
    
    glGenTextures(1, &m_whiteTexture);
    glBindTexture(GL_TEXTURE_2D, m_whiteTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, whitePixel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Screen::setupScreenQuad()
{
    // Unit quad - scaled via model matrix
    // UV coords flipped horizontally (1.0 - u) to compensate for 180° Y-rotation
    float vertices[] = {
        // Position          // UV
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f,
         0.5f, -0.5f, 0.0f,  0.0f, 0.0f,
         0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);
}

unsigned int Screen::loadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);  // Fix flipped screen texture
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    
    if (data)
    {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return textureID;
    }
    else
    {
        glDeleteTextures(1, &textureID);
        return 0;
    }
}

void Screen::startPlayback()
{
    if (m_filmTextures.empty())
        return;
    
    m_playing = true;
    m_timer = 0.0f;
    m_currentFrame = 0;
    LOG_INFO("[SCREEN] Playback started");
}

void Screen::stopAndResetToWhite()
{
    m_playing = false;
    m_timer = 0.0f;
    m_currentFrame = 0;
}

void Screen::update(float deltaTime)
{
    if (!m_playing || m_filmTextures.empty())
        return;
    
    m_timer += deltaTime;
    
    if (m_timer >= FILM_DURATION)
    {
        stopAndResetToWhite();
        return;
    }
    
    float frameDuration = FILM_DURATION / m_filmTextures.size();
    m_currentFrame = std::min(static_cast<int>(m_timer / frameDuration), 
                              static_cast<int>(m_filmTextures.size()) - 1);
}

void Screen::draw(const glm::mat4& view, const glm::mat4& projection)
{
    if (!m_shader || m_VAO == 0)
        return;
    
    m_shader->use();
    
    // Build model matrix: translate, rotate to face camera, scale
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_position);
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(m_size.x, m_size.y, 1.0f));
    
    m_shader->setMat4("model", model);
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);
    
    // Select texture
    unsigned int tex = m_whiteTexture;
    if (m_playing && !m_filmTextures.empty())
    {
        tex = m_filmTextures[m_currentFrame];
    }
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex);
    m_shader->setInt("uTex", 0);
    
    // Set shader uniforms (debug modes off by default)
    m_shader->setInt("uForceSolid", 0);
    m_shader->setInt("uDebugUV", 0);
    
    // Disable culling for screen quad
    glDisable(GL_CULL_FACE);
    
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    
    glEnable(GL_CULL_FACE);
}
