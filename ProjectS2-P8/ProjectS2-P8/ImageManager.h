#pragma once
#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QPixmap>
#include <unordered_map>
#include <QString>
#include <QPainter>


enum ImageType {
    PLANE,
    JET,
    CHOPPER,
    GAZ,
    WIND,
    BIRD,
    TREE,
	RUNWAY,
    TREE,
    LOOSE
};

class ImageManager {
public:
    static ImageManager& getInstance();  // Singleton
    QPixmap getImage(ImageType type) const;  // Récupérer une image

private:
    ImageManager();  // Constructeur privé
    void loadImages();  // Charger toutes les images

    std::unordered_map<ImageType, QPixmap> images;  // Stockage des images
};

#endif
