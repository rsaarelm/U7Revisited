///////////////////////////////////////////////////////////////////////////
//
// Name:     RESOURCEMANAGER.H
// Author:   Anthony Salter
// Date:     2/03/05
// Purpose:  The ResourceManager subsystem.
//
///////////////////////////////////////////////////////////////////////////

#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <map>
#include <memory>
#include "Object.h"
#include "raylib.h"

class Config;

class ResourceManager : public Object {
 public:
    ResourceManager() {};

    virtual void Init(const std::string& configfile);
    virtual void Shutdown();
    virtual void Update();
    virtual void Draw() {};

    void ClearTextures();

    std::map<std::string, std::unique_ptr<Texture>> m_TextureList;
    std::map<std::string, std::unique_ptr<Model>> m_ModelList;
    std::map<std::string, std::unique_ptr<std::tuple<ModelAnimation*, int>>> m_ModelAnimList;
    std::map<std::string, std::unique_ptr<Wave>> m_SoundList;
    std::map<std::string, std::unique_ptr<Music>> m_MusicList;
    std::map<std::string, std::unique_ptr<Config>> m_configList;

    void AddTexture(const std::string& textureName, bool mipmaps = true);
    void AddModel(const std::string& meshName);
    void AddModel(const Model& model, const std::string& meshName);
    void AddSound(const std::string& soundName);
    void AddMusic(const std::string& musicName);
    void AddConfig(const std::string& configName);

    //  The pointers that these functions hand out are for access ONLY.  Do not
    //  delete them.  You didn't make these resources, you have no business
    //  deleting them.
    Texture* GetTexture(const std::string& textureName, bool mipmaps = true);
    Model* GetModel(const std::string& meshName);
    Wave* GetSound(const std::string& soundName);
    Music* GetMusic(const std::string& musicName);
    Config* GetConfig(const std::string& configName);

    /// Set the animation frame of a given mesh model. Yes, this modifies the
    /// actual model resource. You'll need to do this before every draw call
    /// for animated models.
    void AnimateModel(const std::string& meshName, const std::string& animName, unsigned int frame);

    //  Utilities
    bool DoesFileExist(const std::string& filename);
};

#endif
