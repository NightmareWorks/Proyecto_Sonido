#pragma once 
#include "Transform.h"
#include <iostream>

Transform::Transform(Entidad * pEnt, int x, int y, int z) : Componente(pEnt)
{
	this->x = x; this->y = y; this->z = z;
	camera = pEntidad->getPEstado()->getEntidad("camera");
	Awake();
}
Transform::~Transform()
{
}
void Transform::Update(float deltaTime, Mensaje const & msj) {
	Componente::Update(deltaTime, msj);
	Mensaje msg = msj;
	//Play/jddjdj/x/y/z
	if (msg.getTipo() == Tipo::Input ) {

		if (msg.getSubTipo() == SubTipo::Mover) {
			int pos = msg.getMsg().find("/");
			std::string xS = msg.getMsg().substr(0, pos);
			std::string subcad = msg.getMsg().substr(pos + 1);
			pos = subcad.find("/");
			std::string yS = subcad.substr(0, pos);
			std::string zS = subcad.substr(pos + 1);

			float x = std::stof(xS)*deltaTime / 4;
			float y = std::stof(yS)*deltaTime / 4;
			float z = std::stof(zS)*deltaTime / 4;

			this->x += x; this->z += z; this->y += y;

			Mensaje  m(Tipo::Fisica, msg.getMsg(), SubTipo::Mover);
			//Si no se especifica receptor se considera broadcast
			m.setMsgInfo(pEntidad, pEntidad);
			pEntidad->getPEstado()->addMsg(m);
		}
	
	}
}

void Transform::Awake() {
	
}