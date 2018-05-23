#pragma once
#include "EstadoJuego.h"
#include "FactoryBalas.h"
#include <iostream>
#include "OgreParticleSystem.h"
#include "Juego.h"
#include "EstadoMenu.h"

static std::vector<std::string> colisiones;
static std::string anteriorRec = " ", anteriorEmi =" ";

bool collisionCallback(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1) {
	std::string emisor = "vacio", receptor = "vacio";
	

	receptor = static_cast<Ogre::SceneNode*>(colObj0Wrap->getCollisionObject()->getUserPointer())->getName();

	std::string s = receptor.substr(5, 5);

		int i = 0;
	if (s != "Candy" && s != "Muffi") {
		while (i < colisiones.size() && colisiones[i] != receptor)i++;
		if (i >= colisiones.size()) colisiones.push_back(receptor);
	}
	

	emisor = static_cast<Ogre::SceneNode*>(colObj1Wrap->getCollisionObject()->getUserPointer())->getName();
	if (emisor != "vacio") {
		std::string s = emisor.substr(5, 9);
		if (s == "triangulo") {
			i = 0;
			while (i < colisiones.size() && colisiones[i] != emisor)i++;
			if (i >= colisiones.size()) colisiones.push_back(emisor);
			
		}

	}
	return false;
}
EstadoJuego::EstadoJuego(Ogre::SceneManager * mng, Ogre::RenderWindow* mWindow, FMOD::System* sys, Juego* pJuego): Estado(mng, mWindow, sys, pJuego)
{
	
	noInput = true; contInput = contDescartes=0;
	gContactAddedCallback = collisionCallback;
	anteriorEmi = anteriorRec = " ";
	cargaGui();
	
}

void EstadoJuego::init() {
#pragma region InitOgre 
	factoria = new FactoryBalas();
	scnMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
	scnMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);


	scnMgr->setShadowFarDistance(200);


	entidades.insert(std::make_pair("Alaia", new Entidad(this, "Alaia")));
	entidades.insert(std::make_pair("camera", new Entidad(this, "camera")));


	Mensaje msg(Tipo::Fisica, "80/30/0", SubTipo::Reposicionar,8);
	msg.setMsgInfo(entidades.at("Alaia"), entidades.at("Alaia"));
	mensajes.push(msg);

	Ogre::ParticleSystem* particleSystem = scnMgr->createParticleSystem("Bubbles", "Bubbles");
	scnMgr->getSceneNode("Alaia")->attachObject(particleSystem);
	particleSystem->setEmitting(true);

	Entidad *aux = new Entidad(this); aux->aņadeComponenteGrafico("arena","arena");
	aux->aņadeComponenteFisico(0, 0, 0, true);
	entidades.insert(std::make_pair("arena", aux));


	Entidad *aux2 = new Entidad(this);
	aux2->aņadeComponenteGrafico("compcube","compcube");
	aux2->aņadeComponenteFisico(0 ,0 ,0, false, tipoFisica::Estatico, 100);
	entidades.insert(std::make_pair("compcube", aux2));
	Mensaje ms1(Tipo::Fisica, "25/5/10", SubTipo::Reposicionar,8);
	ms1.setMsgInfo(entidades.at("compcube"), entidades.at("compcube"));
	mensajes.push(ms1);



	Entidad* aux1 = new Entidad(this);
	aux1->aņadeComponenteGrafico("stone","stone");
	aux1->aņadeComponenteFisico(0, 0, 0, false, tipoFisica::Kinematico, 1);
	aux1->aņadeComponenteLogico("IABola");
	entidades.insert(std::make_pair("stone", aux1));
	Mensaje ms(Tipo::Fisica, "20/150/0", SubTipo::Reposicionar,8);
	ms.setMsgInfo(entidades.at("stone"), entidades.at("stone"));
	mensajes.push(ms);


	Entidad* aux3 = new Entidad(this); aux3->aņadeComponenteSM("SoundManager", system);
	entidades.insert(std::make_pair("SoundManager", aux3));
	Mensaje playM(Tipo::Audio, "Play/wii.mp3", SubTipo::Musica);
	mensajes.push(playM);


	// Luz por defecto
	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	light = scnMgr->createLight("MainLight");
	light->setPosition(20, 50, 50);
	



#pragma endregion InitOgre
	destroy();
	initCEGUI();

	Mensaje ini(Tipo::Fisica, " ", SubTipo::Inicializado, -10);
	//mensajes.push(ini);
}
void EstadoJuego::cargaGui()
{
	Estado::initCEGUI();
	guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("loadScreen.layout");
	m_gui.getRoot()->addChild(guiRoot);

}
void EstadoJuego::restaPower()
{
	Mensaje ms(Tipo::Gui, "-0.25", SubTipo::CambiaMana);
	ms.setMsgInfo(entidades.at("Alaia"), entidades.at("Alaia"));
	mensajes.push(ms);
}
void EstadoJuego::resuelveCol()
{
	while (colisiones.size() > 0) {
		std::string aux = colisiones[colisiones.size()-1].substr(5,colisiones[colisiones.size() - 1].size());
		colisiones.pop_back();
		if (aux != "arena" && aux != "Alaia") { 
			try {
				Mensaje ms(Tipo::Fisica, " ", SubTipo::Colision);
				ms.setMsgInfo(entidades.at(aux), entidades.at(aux));
				mensajes.push(ms);
			}catch(const std::out_of_range& ot){}
		}

	}
}
void EstadoJuego::cambiaEstado(std::string const & estado)
{

	if (estado == "GameOver") {
		EstadoMenu* go = new EstadoMenu (scnMgr, mWin, system, game_, "GameOver");
		game_->cambiaEstado(go, true);
	}
}
void EstadoJuego::changeHabilidad()
{
	if (habilidad == 0) { // Simple
		habilidad = 1;
		lluviaW->setAlpha(1);
		simpleW->setAlpha(0);
	}
	else if (habilidad == 1) { //Lluvia
		habilidad = 0;
		lluviaW->setAlpha(0);
		simpleW->setAlpha(1);
	}
}
bool EstadoJuego::initCEGUI() {
	//Estado::initCEGUI();

	guiRoot = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("Hud.layout");
	m_gui.getRoot()->addChild(guiRoot);

	lluviaW = static_cast<CEGUI::DefaultWindow*>(guiRoot->getChild("Lluvia"));
	simpleW = static_cast<CEGUI::DefaultWindow*>(guiRoot->getChild("Simple"));
	
	lluviaW->setAlpha(0);
	simpleW->setAlpha(1);
	habilidad = 0;

	Mensaje ms(Tipo::Gui, " ", SubTipo::InitGui);
	ms.setMsgInfo(entidades.at("Alaia"), entidades.at("Alaia"));
	mensajes.push(ms);
	
	return true;
}
//===========================================================================================================
bool EstadoJuego::update(float delta) {
	CEGUI::System::getSingleton().injectTimePulse(1.0f / delta);
	//CEGUI::System::getSingleton().injectTimePulse(0.016f);
	this->getFisicManager()->getDynamicsWorld()->stepSimulation(1.0f / delta);
	
	for (size_t i = 0; i < borrar.size(); i++)
	{
		Entidad * ent = entidades.at(borrar[i]);
		delete ent;
		entidades.erase(borrar[i]);
	
	}
	borrar.clear();

	while (mensajes.size() > 0) {
		Mensaje aux = mensajes.top();
		mensajes.pop();
		
		for (std::pair<std::string, Entidad*> ent : entidades) {
			ent.second->Update(delta, aux);
		}
	}
	resuelveCol();
	if (contInput >= 30) {
		entidades.at("Alaia")->setAnim("Idle", true, true, true);
		//entidades.at("Alaia")->setAnim("IdleBase", true, true, true);
		contInput = 0;
	}

	for (std::pair<std::string, Entidad*> ent : entidades) {
		ent.second->Update(delta, Mensaje(Tipo::AnimationM, " ", SubTipo::Nulo));
		ent.second->Update(delta, Mensaje(Tipo::IA, " ", SubTipo::Nulo));
	}
	int random = rand() % 10000;
	if ( random > 9995) {
		int x, z;
		x = (rand() % 200 )-100;
		z = (rand() % 200) - 100;

		if (random % 2 == 0) {
			Mensaje msg(Tipo::Consumible, to_string(x)+"/3/"+to_string(z), SubTipo::Candy);
			mensajes.push(msg);
		}
		else {
			Mensaje msg(Tipo::Consumible, to_string(x) + "/5/" + to_string(z), SubTipo::Muffin);
		mensajes.push(msg);

		}
	}

	contInput++;
	m_gui.draw();
	swapMsgBufer();

	return true;
}

void EstadoJuego::joystickMoved(float x, float y, int js) {

	std::string sx, sy; sx = std::to_string(x); sy = std::to_string(y);
	std::string s = sx + "/" + "0/" + sy;


	if (js == 0) {
		Mensaje msgI(Tipo::Input, s, SubTipo::Mover);
		mensajes.push(msgI);
		Mensaje msgR(Tipo::Render, s, SubTipo::Orientar); //Look at de la camara
		msgR.setMsgInfo(entidades.at("Alaia"), entidades.at("Alaia"));
		mensajes.push(msgR);
		entidades.at("Alaia")->setAnim("Run", true);
		//entidades.at("Alaia")->setAnim("RunBase", true);
		contInput = 0;

	}
	else if (js == 1) {
		Mensaje msgI(Tipo::Input, s, SubTipo::OrientaCamara);
		msgI.setMsgInfo(entidades.at("camera"), entidades.at("camera"));
		mensajes.push(msgI);
	}
	else {
		Mensaje msgF(Tipo::Fisica, s, SubTipo::Nulo);
		mensajes.push(msgF);
	}

}

void EstadoJuego::keyPressed(std::string s) {
	if (s == "0" || s == "salto") {
		Mensaje msg(Tipo::Fisica, "", SubTipo::Salto);
		msg.setMsgInfo(entidades.at("Alaia"), entidades.at("Alaia"));
		mensajes.push(msg);
		entidades.at("Alaia")->setAnim("Jump");
		Mensaje msg1(Tipo::Fisica, "", SubTipo::Musica);
		//msg.setMsgInfo(entidades.at("Alaia"), entidades.at("Alaia"));
		mensajes.push(msg1);
	}
	else if (s == "1") {
		entidades.at("Alaia")->setAnim("AirUp");

	}
	else if (s == "2") {

	entidades.at("Alaia")->setAnim("AirDown");
		Mensaje msg(Tipo::Mana, "carga", SubTipo::GetPower);
		msg.setMsgInfo(entidades.at("Alaia"), entidades.at("Alaia"));
		mensajes.push(msg);
	}
	else if (s == "3") {
		
			for (std::pair<std::string, Entidad*> ent : entidades)
				ent.second->Update(0.12, Mensaje(Tipo::IA, " ", SubTipo::Musica));
	}

	else if (s == "5") {
		if (habilidad == 0) { //Simple
			Mensaje msg(Tipo::Mana, "simple", SubTipo::DoPower);
			msg.setMsgInfo(entidades.at("Alaia"), entidades.at("Alaia"));
			mensajes.push(msg);
		}
		else if (habilidad == 1) { //lluvia
			Mensaje msg(Tipo::Mana, "lluvia", SubTipo::DoPower);
			msg.setMsgInfo(entidades.at("Alaia"), entidades.at("Alaia"));
			mensajes.push(msg);
		}

	}
	else if (s == "7") {
			EstadoMenu * pEstado;
			pEstado = new EstadoMenu(scnMgr, mWin, system, game_, "pause");
			game_->cambiaEstado(pEstado);
	}
	
	

}
void EstadoJuego::keyReleased(std::string s) {
	if (s == "2") {
		
		Mensaje msg(Tipo::Mana, " ", SubTipo::GetPower);
		msg.setMsgInfo(entidades.at("Alaia"), entidades.at("Alaia"));
		mensajes.push(msg);
	}
	else if (s == "4") {
		changeHabilidad();
	}

}