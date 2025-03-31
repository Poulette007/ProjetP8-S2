#include "ImageManager.h"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
std::vector<QPixmap> ImageManager::images;

ImageManager::ImageManager() {
    images.resize(IMAGE_COUNT);
    loadImages();
}

void ImageManager::loadImages() {
    fs::path p = fs::absolute(PLANE_PATH);
    std::cout << "Chemin absolu : " << p << std::endl;
    // Mettre tout les image necessaire ici et dans le ImageManager.h
    images[PLANE] = QPixmap(PLANE_PATH);
	images[GAZ] = QPixmap(PLANE_PATH);
	images[WIND] = QPixmap(PLANE_PATH);
	images[BIRD] = QPixmap(PLANE_PATH);
	images[TREE] = QPixmap(PLANE_PATH);
    setPropertie(PLANE);
    setPropertie(GAZ);
    setPropertie(WIND);
    setPropertie(BIRD);
    setPropertie(TREE);
}

QPixmap ImageManager::getImage(ImageType type) {
    if (type >= 0 && type < IMAGE_COUNT) {
        return images[type];
    }
    return QPixmap();
}

void ImageManager::setPropertie(ImageType type)
{
    QSize newSize(100, 100);
    QPixmap scaledPixmap = images[type].scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    images[type] = scaledPixmap;
}
