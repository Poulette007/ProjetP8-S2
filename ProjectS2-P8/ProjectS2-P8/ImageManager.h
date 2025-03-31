#pragma once
#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include <QPixmap>
#include <vector>
#include <QString>
#include "const.h"
/*
    Pour QT
    Chemin vers les image. Doivent etre dans le fichier image dans ProjectS2-P8
    Il faut mettre le nom de l'image dans l'enum avec un _PATH
    Voir ex : avion
*/
//C:\Users\yohan\Documents\git\ProjetP8 - S2/ProjectS2 - P8/ProjectS2 - P8/Sprite
#define PLANE_PATH "Sprite/Avion.png" // je pense
//#define GAZ_PATH "./ProjectS2-P8/Sprite/Gaz.png" // yen a pas
enum ImageType {
    PLANE,
    GAZ,
	WIND,
	BIRD,
    TREE,
    IMAGE_COUNT
};

class ImageManager {
public:
    ImageManager();
    static void loadImages();
    static QPixmap getImage(ImageType type);
	static void setPropertie(ImageType pixmap);

private:
    static std::vector<QPixmap> images;
};

#endif
