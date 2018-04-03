#pragma once
#include "Componente.h"
#include <iterator>
#include "Mensaje.h"

class Estado;
//enum tipoFisica { Dinamico, Estatico, Kinematico };
class Entidad{

public:
	Entidad(Estado* pEstado);
	Entidad(Estado* pEstado, std::string prefab);
	void Update(float deltaTime, Mensaje const & msg);
	void Awake();
	void Sleep();
	Estado* getPEstado() { return pEstado; };

	bool aņadeComponenteGrafico(std::string mesh);
	bool aņadeComponenteFisico(float altoCaja, float anchoCaja, float profCaja, bool suelo, tipoFisica type = tipoFisica::Estatico, int masa = 0);
	bool aņadeAnimacion(std::string mesh, std::ifstream & fe);
	bool aņadeAnimacion(std::string name,bool enabled = true, bool loop = true);
	bool aņadeComponenteLogico(std::string component);
	

private:
	std::map<std::string, Componente*> componentes;
	Estado* pEstado;
	bool activo;
	Entidad* target; //Usar esta entidad para escuchar los mensajes dirigidos a ella
	
};