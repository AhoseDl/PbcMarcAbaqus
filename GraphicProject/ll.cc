#include<QGuiApplication>
#include<QQmlApplicationEngine>

int main(int argc, char** argv){

QApplication app(argc, argv);
QQmlApplicationEngine engine;

//engine.load(QUrl());
return app.exec();

}
/*
import QtQuick 2.7
Rectangle { // définition d’un object Rectangle qui sera ici la fenêtre
width: 200; height: 200 // 200x200 pixels
color: "white" // couleur de fond en blanc
	Text {
	text: "Hello World" // définition d’un texte à l’intérieur du rectangle
	color: "#ff0000" // couleur du texte ici en rouge
	font.pointSize: 24 // taille de la police
	font.bold: true // en gras
	anchors.centerIn: parent // le texte sera centré dans la fenêtre
	}
}*/
