#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
public:
    Texture(const std::string& path);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    inline int getWidth() const {return m_width;};
    inline int getHeight() const {return m_height;};
private:
    std::string m_filePath;
    unsigned int m_rendererId;
    unsigned char* m_localBuffer;
    int m_width, m_height, m_BPP;
};



#endif //TEXTURE_H
