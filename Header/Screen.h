#pragma once

#include <glm/glm.hpp>
#include <vector>

class Shader;

class Screen
{
public:
    Screen();
    ~Screen();
    
    void init();
    void startPlayback();
    void stopAndResetToWhite();
    void update(float deltaTime);
    void draw(const glm::mat4& view, const glm::mat4& projection);
    
    bool isPlaying() const { return m_playing; }
    
private:
    std::vector<unsigned int> m_filmTextures;
    unsigned int m_whiteTexture;
    
    bool m_playing;
    float m_timer;
    int m_currentFrame;
    bool m_debugUV;
    bool m_forceSolidColor;
    bool m_debugOverlay;
    
    static constexpr float FILM_DURATION = 20.0f;
    
    unsigned int m_VAO;
    unsigned int m_VBO;
    unsigned int m_overlayVAO;
    unsigned int m_overlayVBO;
    
    Shader* m_shader;
    
    glm::vec3 m_position;
    glm::vec2 m_size;
    
    void loadFilmTextures();
    void createWhiteTexture();
    void setupScreenQuad();
    unsigned int loadTexture(const char* path);
};
