#pragma once
#include "Entidad.h"
#include "GComponent.h"
#include "Transform.h"
#include <iostream>
#include <fstream>

Entidad::Entidad(Estado* pEstado): pEstado(pEstado){


}
Entidad::Entidad(Estado* pEstado, std::string prefab) : pEstado(pEstado) {


	std::string path = "../Media/prefabs/";
	path += prefab + ".txt";
	std::ifstream fe(path);
	std::string type;
	fe >> type;
	while (type != "---") {
		if (type == "Grafico") {
			std::string mesh; fe >> mesh;
			aņadeComponenteGrafico(mesh);
		}
		else if (type == "Logico") {
			std::string component; fe >> component;
			aņadeComponenteLogico(component);
		}
		fe >> type;
	}

	fe.close();
	
}

bool Entidad::aņadeComponenteGrafico(std::string mesh) {
	componentes.insert(std::make_pair("Grafico", new GComponent(this, mesh)));
	return true;
}
bool Entidad::aņadeComponenteLogico(std::string component) {
	if (component == "Transform") {
		componentes.insert(std::make_pair("Transform", new Transform(this, 0, 0, 0)));
	}
	return true;
}
void Entidad::Update(float deltaTime,  Mensaje const & msg){
	if (activo){
		for (auto i = componentes.begin(); i != componentes.end(); i++)
		{
			i->second->Update(deltaTime, msg);
		}
	}
}
void Entidad::Awake(){ activo = true; }
void Entidad::Sleep(){ activo = false; }