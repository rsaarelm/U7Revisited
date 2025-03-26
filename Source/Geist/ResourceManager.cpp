#include "ResourceManager.h"
#include <cassert>
#include "Config.h"
#include "Globals.h"
#include "Logging.h"

#include <fstream>
#include <sstream>

using namespace std;

void ResourceManager::Init(const std::string& configfile) {}

void ResourceManager::Shutdown() {
    for (auto& node : m_TextureList) {
        UnloadTexture(*node.second);
    }

    for (auto& node : m_ModelList) {
        UnloadModel(*node.second);
    }

    for (auto& node : m_ModelAnimList) {
        auto [anims, count] = *node.second;
        UnloadModelAnimations(anims, count);
    }

    for (auto& node : m_SoundList) {
        UnloadWave(*node.second);
    }

    for (auto& node : m_MusicList) {
        UnloadMusicStream(*node.second);
    }
}

void ResourceManager::Update() {}

bool ResourceManager::DoesFileExist(const std::string& fileName) {
    map<std::string, unique_ptr<Texture>>::iterator node;
    node = m_TextureList.find(fileName);

    if (node == m_TextureList.end()) {
        ifstream file(fileName.c_str());
        return file.good();
    } else {
        return true;
    }
}

void ResourceManager::AddTexture(const std::string& textureName, bool mipmaps) {
    Log("Loading texture " + textureName);
    m_TextureList[textureName] =
        std::make_unique<Texture>(LoadTexture(textureName.c_str()));
    Log("Load successful.");
}

void ResourceManager::AddModel(const std::string& modelName) {
    Log("Loading model " + modelName);
    m_ModelList[modelName] =
        std::make_unique<Model>(LoadModel(modelName.c_str()));

    int animCount = 0;
    ModelAnimation* anims = LoadModelAnimations(modelName.c_str(), &animCount);

    if (anims && animCount > 0) {
        m_ModelAnimList[modelName] =
            std::make_unique<std::tuple<ModelAnimation*, int>>(anims,
                                                               animCount);
    }

    Log("Load successful.");
}

void ResourceManager::AddSound(const std::string& soundName) {
    Log("Loading sound " + soundName);
    m_SoundList[soundName] =
        std::make_unique<Wave>(LoadWave(soundName.c_str()));
    Log("Load successful.");
}

void ResourceManager::AddMusic(const std::string& musicName) {
    Log("Loading music " + musicName);
    m_MusicList[musicName] =
        std::make_unique<Music>(LoadMusicStream(musicName.c_str()));
    Log("Load successful.");
}

void ResourceManager::AddConfig(const std::string& configName) {
    Log("Loading config " + configName);
    m_configList[configName] = std::make_unique<Config>();
    m_configList[configName]->Load(configName);
    Log("Load successful.");
}

Texture* ResourceManager::GetTexture(const std::string& Texturename,
                                     bool mipmaps) {
    map<std::string, unique_ptr<Texture>>::iterator node;
    node = m_TextureList.find(Texturename);

    if (node != m_TextureList.end()) {
        return (*node).second.get();
    } else {
        Log("Loading texture " + Texturename + " on the fly!");
        AddTexture(Texturename, mipmaps);
        return m_TextureList[Texturename].get();
    }
}

Model* ResourceManager::GetModel(const std::string& modelName) {
    map<std::string, unique_ptr<Model>>::iterator node;
    node = m_ModelList.find(modelName);

    if (node != m_ModelList.end()) {
        return (*node).second.get();
    } else {
        Log("Loading model " + modelName + " on the fly!");
        AddModel(modelName);
        return m_ModelList[modelName].get();
    }
}

void ResourceManager::AnimateModel(const std::string& modelName,
                                   const std::string& animName,
                                   unsigned int frame) {
    auto model = GetModel(modelName);

    map<std::string, unique_ptr<std::tuple<ModelAnimation*, int>>>::iterator
        node;
    node = m_ModelAnimList.find(modelName);

    if (node == m_ModelAnimList.end()) {
        Log("Model " + modelName + " has no animations!");
        return;
    }

    auto [anims, count] = *node->second;

    // If animations are present, there must be at least one.
    assert(count > 0);

    for (int i = 0; i < count; i++) {
        if (animName == anims[i].name) {
            UpdateModelAnimation(*model, anims[i],
                                 frame % (unsigned int)anims[i].frameCount);
            return;
        }
    }

    // The named animation wasn't found for this model. Animation calls need
    // to clear earlier animation states though, so at least we can set the
    // model into a possibly neutral state.
    Log("Animation " + animName + " not found for model " + modelName);
    UpdateModelAnimation(*model, anims[0], 0);
}

Wave* ResourceManager::GetSound(const std::string& soundName) {
    map<std::string, unique_ptr<Wave>>::iterator node;
    node = m_SoundList.find(soundName);

    if (node != m_SoundList.end()) {
        return (*node).second.get();
    } else {
        Log("Loading sound " + soundName + " on the fly!");
        AddSound(soundName);
        return m_SoundList[soundName].get();
    }
}

Music* ResourceManager::GetMusic(const std::string& musicName) {
    map<std::string, unique_ptr<Music>>::iterator node;
    node = m_MusicList.find(musicName);

    if (node != m_MusicList.end()) {
        return (*node).second.get();
    } else {
        Log("Loading music " + musicName + " on the fly!");
        AddMusic(musicName);
        return m_MusicList[musicName].get();
    }
}

Config* ResourceManager::GetConfig(const std::string& configName) {
    map<std::string, unique_ptr<Config>>::iterator node;
    node = m_configList.find(configName);
    if (node != m_configList.end()) {
        return (*node).second.get();
    } else {
        Log("Loading config " + configName + " on the fly!");
        AddConfig(configName);
        return m_configList[configName].get();
    }
}

//  Dumps the current texture list so it can be recreated (on res change or
//  whatever)
void ResourceManager::ClearTextures() { m_TextureList.clear(); }

void ResourceManager::AddModel(const Model& model,
                               const std::string& meshName) {
    m_ModelList[meshName] = std::make_unique<Model>(model);
}
