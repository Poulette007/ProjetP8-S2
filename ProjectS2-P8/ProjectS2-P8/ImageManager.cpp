#include "ImageManager.h"
#include <iostream>

#define PLANE_PATH "sprites/avion/avion1.png"
#define JET_PATH "sprites/avion/jet.png"
#define CHOPPER_PATH "sprites/avion/chopper.png"
#define GAZ_PATH "sprites/objet/gas.png"
#define WIND_PATH "sprites/objet/vent.png"
#define BIRD_PATH "sprites/objet/bird.png"
#define TREE_PATH "sprites/objet/arbre2.png"
#define LOOSE_PATH "sprites/background/GameOver.png"
#define RUNWAY_PATH "sprites/background/piste.png"
#define AIGUILLE_PATH "sprites/Stat/Baton.png"
#define CADRAN_PATH "sprites/Stat/Cadran.png"
#define BACKGROUND_GAME_PATH "sprites/background/BackgroundGame.png"
#define BACKGROUND_ATTERRISSAGE_DECOLAGE_PATH "sprites/background/BackgroundDecoAtte.png"
#define AIRPORT_PATH "sprites/background/Airport.png"
#define TOUR_PATH "sprites/background/ControlTower.png"
#define BARRIERE_PATH "sprites/objet/barriere.png"


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
        {JET, JET_PATH},
        {CHOPPER, CHOPPER_PATH},
        {GAZ, GAZ_PATH},
        {WIND, WIND_PATH},
        {BIRD, BIRD_PATH},
		{TREE, TREE_PATH},
		{RUNWAY, RUNWAY_PATH},
        {LOOSE, LOOSE_PATH},
		{CADRAN, CADRAN_PATH},
        {AIGUILLE, AIGUILLE_PATH},
		{BACKGROUND_GAME, BACKGROUND_GAME_PATH},
		{BACKGROUND_ATTERRISSAGE_DECOLAGE, BACKGROUND_ATTERRISSAGE_DECOLAGE_PATH},
		{BARRIERE, BARRIERE_PATH},
		{TOUR, TOUR_PATH},
		{AIRPORT, AIRPORT_PATH}
    };
    //for (const auto& [type, path] : paths) {
    //    QPixmap pixmap(path);
    //    if (pixmap.isNull()) {
    //        std::cerr << "Erreur: Impossible de charger l'image " << path.toStdString() << std::endl;
    //    }
    //    else {
    //        // Redimensionner tout en gardant le ratio, sans ajouter de zone transparente
    //        QSize newSize(300, 300); // ou une taille qui te convient
    //        QPixmap scaledPixmap = pixmap.scaled(
    //            newSize,
    //            Qt::KeepAspectRatio,
    //            Qt::SmoothTransformation
    //        );

    //        images[type] = scaledPixmap;
    //    }
    //}
    for (const auto& [type, path] : paths) {
        QPixmap pixmap(path);
        if (pixmap.isNull()) {
            std::cerr << "Erreur: Impossible de charger l'image " << path.toStdString() << std::endl;
		}
		else if (type == BACKGROUND_GAME || type == BACKGROUND_ATTERRISSAGE_DECOLAGE) {
			QSize newSize(1920, 1080);
			QPixmap scaledPixmap = pixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			QPixmap finalPixmap(newSize);
			QPainter painter(&finalPixmap);
			QPoint center((newSize.width() - scaledPixmap.width()) / 2,
				(newSize.height() - scaledPixmap.height()) / 2);
			painter.drawPixmap(center, scaledPixmap);
			images[type] = finalPixmap;
        }
        else if (type == BIRD)
        {
			QSize newSize(200, 200);
			QPixmap scaledPixmap = pixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			QPixmap finalPixmap(newSize);
			finalPixmap.fill(Qt::transparent);
			QPainter painter(&finalPixmap);
			QPoint center((newSize.width() - scaledPixmap.width()) / 2,
				(newSize.height() - scaledPixmap.height()) / 2);
			painter.drawPixmap(center, scaledPixmap);
			images[type] = finalPixmap;
        }
        else {
            QSize newSize(300, 300);
            QPixmap scaledPixmap = pixmap.scaled(newSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            QPixmap finalPixmap(newSize);
            finalPixmap.fill(Qt::transparent);

            QPainter painter(&finalPixmap);
            QPoint center((newSize.width() - scaledPixmap.width()) / 2,
                (newSize.height() - scaledPixmap.height()) / 2);
            painter.drawPixmap(center, scaledPixmap);

            images[type] = finalPixmap;
        }
    }
}

QPixmap ImageManager::getImage(ImageType type) const{
    std::cout << "getImage() appelé avec type: " << type << std::endl;
    if (type >= 0 && type < images.size() + 1) {
        std::cout << "type: " << type << std::endl;
        return images.at(type);
    }
    std::cerr << "Erreur : accès hors limites dans ImageManager::getImage(" << type << "). Taille: " << images.size() << std::endl;
    return QPixmap();
}