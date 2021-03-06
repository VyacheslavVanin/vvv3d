#include "textureatlas.hpp"
#include "texturepacker.hpp"
#include <string.h>
#include <utils/helper.hpp>
#include <vvv3d/utils/log.hpp>
#include <vvv3d/utils/texture_io.hpp>

using namespace vvv3d;
using namespace std;

TextureAtlas::TextureAtlas(size_t width, size_t height)
    : atlas(make_shared<LowLevelTexture>(nullptr, width, height, GL_RGBA,
                                         GL_RGBA, GL_UNSIGNED_BYTE)),
      textures(), width(width), height(height)
{
}

static bool textureCompareHeightFirst(const pair<string, Texture*>& left,
                                      const pair<string, Texture*>& right)
{
    const auto& l = left.second;
    const auto& r = right.second;
    return l->getHeight() < r->getHeight()
               ? true
               : l->getHeight() > r->getHeight()
                     ? false
                     : l->getWidth() < r->getWidth();
}

static pair<GLuint, GLuint> textureGetSize(const pair<string, Texture*>& t)
{
    return make_pair(t.second->getWidth(), t.second->getHeight());
}

std::vector<std::unique_ptr<TextureAtlas>>
TextureAtlas::pack(size_t width, size_t height,
                   const std::vector<std::string>& filenames,
                   const std::vector<std::string>& names, unsigned int border)
{
    bench timings("make atlas from files");
    std::vector<LowLevelTexture*> lltexs;
    std::transform(filenames.begin(), filenames.end(), back_inserter(lltexs),
                   [](auto& name) { return readTexture(name.c_str()); });
    return pack(width, height, lltexs, names, border);
}

std::vector<std::unique_ptr<TextureAtlas>>
TextureAtlas::pack(size_t width, size_t height,
                   const std::vector<std::string>& filenames,
                   unsigned int border)
{
    return pack(width, height, filenames, filenames, border);
}

std::vector<std::unique_ptr<TextureAtlas>>
TextureAtlas::pack(size_t width, size_t height,
                   const std::vector<LowLevelTexture*>& texsList,
                   const vector<string>& names, unsigned int border)
{
    bench timings("make atlas");
    std::vector<std::unique_ptr<TextureAtlas>> ret;
    vector<std::pair<std::string, Texture*>> notPlaced;
    for (size_t i = 0; i < texsList.size(); ++i) {
        const auto& t = texsList[i];
        const auto& name = names[i];
        auto tex = new Texture(t);
        notPlaced.push_back(std::make_pair(name, tex));
    }

    auto setTextureOffset = [&](std::pair<string, Texture*>& t, int32_t xoff,
                                int32_t yoff, int32_t border) {
        t.second->texturePosition.x = float(xoff + border) / width;
        t.second->texturePosition.y = float(yoff + border) / height;
    };

    do {
        vector<pair<string, Texture*>> texsToPack;

        notPlaced =
            pack2d(notPlaced, static_cast<ssize_t>(width),
                   static_cast<ssize_t>(height), textureCompareHeightFirst,
                   textureGetSize, setTextureOffset, texsToPack, border);

        struct _PrivConstrWorkaround : public TextureAtlas {
        public:
            _PrivConstrWorkaround(size_t width, size_t height)
                : TextureAtlas(width, height)
            {
            }
        };
        std::unique_ptr<TextureAtlas> newAtlas =
            std::make_unique<_PrivConstrWorkaround>(width, height);
        for (const auto& t : texsToPack) {
            const auto& name = t.first;
            newAtlas->textures[name] = std::unique_ptr<Texture>(t.second);
        }

        auto& atlas = newAtlas->atlas;
        const size_t MAX_BYTES = width * height * 4;
        auto buff = make_unique<GLubyte[]>(MAX_BYTES);
        memset(buff.get(), 0, MAX_BYTES);

        // Clear atlass
        atlas->bind();
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA,
                        GL_UNSIGNED_BYTE, buff.get());

        // Draw all images on atlass
        for (auto& tpair : texsToPack) {
            auto& t = tpair.second;
            readImage(&t->getLowLevelTexture(), buff.get(), GL_RGBA,
                      GL_UNSIGNED_BYTE);
            atlas->bind();

            const GLuint texOffsetX = t->getTexturePosition().x * width;
            const GLuint texOffsetY = t->getTexturePosition().y * height;
            glTexSubImage2D(GL_TEXTURE_2D, 0, texOffsetX, texOffsetY,
                            t->getWidth(), t->getHeight(), GL_RGBA,
                            GL_UNSIGNED_BYTE, buff.get());
            t->tex = atlas;

            const float widthUnit = 1.0f / width;
            const float heightUnit = 1.0f / height;
            t->texturePosition.z = widthUnit * (t->getWidth() - 1);
            t->texturePosition.w = heightUnit * (t->getHeight() - 1);
            t->texturePosition.x += 0.5 * widthUnit;
            t->texturePosition.y += 0.5 * heightUnit;
        }
        ret.push_back(std::move(newAtlas));
    } while (!notPlaced.empty());

    return ret;
}

const Texture& TextureAtlas::get(const string& name) const
{
    return *textures.at(name);
}

std::vector<string> TextureAtlas::listNames() const
{
    std::vector<string> ret;
    for (const auto& i : textures)
        ret.push_back(i.first);
    return ret;
}

std::unique_ptr<TextureAtlas>
TextureAtlas::makeAtlas(std::shared_ptr<LowLevelTexture>&& atlas_texuture,
                        const std::vector<vvv::vector4f>& coords,
                        const std::vector<std::string>& names)
{
    if (coords.size() != names.size()) {
        LOG_ERROR("TextureAtlas::makeAtlas coords.size() != names.size()");
        throw std::invalid_argument("coords.size() != names.size()");
    }

    struct _PrivConstrWorkaround : public TextureAtlas {
    public:
        _PrivConstrWorkaround(size_t width, size_t height)
            : TextureAtlas(width, height)
        {
        }
    };

    const auto width = atlas_texuture->getWidth();
    const auto height = atlas_texuture->getHeight();
    const auto wx = 1.0f / width;
    const auto hx = 1.0f / height;
    const auto& px_multiplier = vvv::vector4f(wx, hx, wx, hx);
    std::unique_ptr<TextureAtlas> newAtlas =
        std::make_unique<_PrivConstrWorkaround>(width, height);
    newAtlas->atlas = std::move(atlas_texuture);
    for (size_t i = 0; i < names.size(); ++i) {
        const auto& name = names[i];
        const auto& coord = coords[i] * px_multiplier;
        newAtlas->textures[name] =
            std::make_unique<Texture>(newAtlas->atlas, coord);
    }

    return newAtlas;
}

std::unique_ptr<TextureAtlas>
TextureAtlas::makeAtlas(const std::string& texture_file_name,
                        const std::vector<vvv::vector4f>& coords,
                        const std::vector<std::string>& names)
{
    auto atlas_texture =
        std::shared_ptr<LowLevelTexture>(readTexture(texture_file_name));
    return makeAtlas(std::move(atlas_texture), coords, names);
}
