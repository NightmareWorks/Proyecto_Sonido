#pragma once
#include "Componente.h"
#include <iterator>
#include "Mensaje.h"

class Estado;

class Entidad{

public:
	Entidad(Estado* pEstado);
	Entidad(Estado* pEstado, std::string prefab);
	~Entidad();
	void Update(float deltaTime, Mensaje  & msg);
	void Awake();
	void Sleep();
	Estado* getPEstado() { return pEstado; };

	bool aņadeComponenteGrafico(std::string mesh);
	bool aņadeComponenteFisico();
	bool aņadeAnimacion(std::string mesh, std::ifstream & fe);
	bool aņadeAnimacion(std::string name,bool enabled = true, bool loop = true);
	bool aņadeComponenteLogico(std::string component);
	

private:
	std::map<std::string, Componente*> componentes;
	Estado* pEstado;
	bool activo;
	Entidad* target; //Usar esta entidad para escuchar los mensajes dirigidos a ella
	
};