#include "ImageManager.h"
#include <iostream>

#define PLANE_PATH "sprites/avion/avion1.png"
#define GAZ_PATH "sprites/objet/gas.png"
#define WIND_PATH "sprites/objet/vent.png"
#define BIRD_PATH "sprites/objet/arbre1.png"
#define TREE_PATH "sprites/objet/arbre2.png"

ImageManager::ImageManager() {
    loadImages();
}

ImageManager& ImageManager::getInstance() {
    static ImageManager instance;  // Singleton : chargé une seule fois
    return instance;
}

void ImageManager::loadImages() {
    std::unordered_map<ImageType, QString> paths = {
        {PLANE, PLANE_PATH},
        {GAZ, GAZ_PATH},
        {WIND, WIND_PATH},
        {BIRD, BIRD_PATH},
        {TREE, TREE_PATH}
    };

    for (const auto& [type, path] : paths) {
        QPixmap pixmap(path);
        if (pixmap.isNull()) {
            std::cerr << "Erreur: Impossible de charger l'image " << path.toStdString() << std::endl;
        }
        else {
            QSize newSize(300, 300);
            pixmap = pixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            images[type] = pixmap;
        }
    }
}

QPixmap ImageManager::getImage(ImageType type) const{
    std::cout << "getImage() appelé avec type: " << type << std::endl;
    if (type >= 0 && type < images.size()) {
        std::cout << "type: " << type << std::endl;
        return images.at(type);
    }
    std::cerr << "Erreur : accès hors limites dans ImageManager::getImage(" << type << "). Taille: " << images.size() << std::endl;
    return QPixmap();
}
