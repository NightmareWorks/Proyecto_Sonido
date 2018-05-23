#include "ManaComponent.h"  
#include "OgreParticleSystem.h"
#include "OgreParticleEmitter.h"

ManaComponent::ManaComponent(Entidad* pEnt) : Componente(pEnt) 
{

	particleSystem = pEntidad->getPEstado()->getScnManager()->createParticleSystem("cargaManaPFX", "Fairy");
	pEntidad->getPEstado()->getScnManager()->getSceneNode(pEntidad->getNombreNodo())->attachObject(particleSystem);
	particleSystem->setEmitting(false);
} 
ManaComponent::~ManaComponent() { 
} 
void ManaComponent::Update(float deltaTime, Mensaje const & msj)
{
	Mensaje msg = msj;
	if (msg.getTipo() == Tipo::Gui) {
		if (msg.getSubTipo() == SubTipo::InitGui) {
			mana = static_cast<CEGUI::ProgressBar*>(pEntidad->getPEstado()->getGUIRoot()->getChild("Power"));
			maxMana = mana->getWidth().d_offset;
		}
		else if (msg.getSubTipo() == SubTipo::CambiaMana && mana != nullptr) {
			if (msg.getMsg()[0] == '+') {
				std::string s;
				s = msg.getMsg().substr(1, msg.getMsg().size());
				CEGUI::UDim dim = mana->getWidth();
				if (dim.d_offset < maxMana) {
					dim += {0, std::stof(s)};
					mana->setWidth(dim);
				}
			}
			else if (msg.getMsg()[0] == '-') {
				std::string s;
				s = msg.getMsg().substr(1, msg.getMsg().size());
				CEGUI::UDim dim = mana->getWidth();
					dim -= {0, std::stof(s)};
					mana->setWidth(dim);
			}
		}
	}
	if (msg.getTipo() == Tipo::Mana && mana != nullptr) {
		if (msg.getSubTipo() == SubTipo::DoPower ) {

			if (msg.getMsg() == "simple") {
				if (mana->getWidth().d_offset >= 24) {
					pEntidad->getPEstado()->getEntidad("Alaia")->setAnim("Shoot");
					Entidad* aux1 = new Entidad(pEntidad->getPEstado());

					string auxBala = pEntidad->getPEstado()->getFactory()->create("triangulo");
					aux1->setNombreNodo(auxBala);

					aux1->a�adeComponenteGrafico("triangulo", auxBala);
				//	restaPower();
					// create a particle system named explosions using the explosionTemplate
					/*Ogre::ParticleSystem* particleSystem = pEntidad->getPEstado()->getScnManager()->createParticleSystem(auxBala + "PFX", "Smoke");
					pEntidad->getPEstado()->getScnManager()->getSceneNode(auxBala)->attachObject(particleSystem);
					particleSystem->setEmitting(true);*/
					aux1->a�adeComponenteFisico(2, 2, 2, false, tipoFisica::Dinamico, 1);
					aux1->a�adeComponenteLogico("BalaComponentSimple");
					pEntidad->getPEstado()->addEntidad(auxBala, aux1);
				}
			}
			else if (msg.getMsg() == "lluvia") {

				if (mana->getWidth().d_offset >= 24) {
					double rotation = 2 * 3.1416 / 6;
					for (int i = 0; i < 6; i++) {
						Entidad* aux1 = new Entidad(pEntidad->getPEstado());

						string auxBala = pEntidad->getPEstado()->getFactory()->create("triangulo");
						aux1->setNombreNodo(auxBala);

						aux1->a�adeComponenteGrafico("triangulo", auxBala);
						aux1->a�adeComponenteFisico(2, 1, 2, false, tipoFisica::Dinamico, 1);
						aux1->a�adeComponenteLogico("BalaComponent");
						pEntidad->getPEstado()->addEntidad(auxBala, aux1);
						double rot = i * rotation;
						double x = aux1->getPEstado()->getFisicManager()->getRigidBody("Alaia")->getWorldTransform().getOrigin().getX() + cos(rot) * 15;
						double z = aux1->getPEstado()->getFisicManager()->getRigidBody("Alaia")->getWorldTransform().getOrigin().getZ() + sin(rot) * 15;
						string posOgro = to_string(x) + "/" +
							to_string(aux1->getPEstado()->getFisicManager()->getRigidBody("Alaia")->getWorldTransform().getOrigin().getY() + 60) + "/" +
							to_string(z);
						Mensaje ms(Tipo::Fisica, posOgro, SubTipo::Reposicionar);
						Mensaje ms1(Tipo::Fisica, "5", SubTipo::Dispara);
						ms.setMsgInfo(aux1, aux1);
						ms1.setMsgInfo(aux1, aux1);
						aux1->getPEstado()->addMsg(ms);
						aux1->getPEstado()->addMsg(ms1);
						
						// create a particle system named explosions using the explosionTemplate
						/*Ogre::ParticleSystem* particleSystem = pEntidad->getPEstado()->getScnManager()->createParticleSystem(auxBala + "PFX", "Smoke");
						pEntidad->getPEstado()->getScnManager()->getSceneNode(auxBala)->attachObject(particleSystem);
						particleSystem->setEmitting(true);*/
					}
					restaPower();
				}
			}
		}
		else if (msg.getSubTipo() == SubTipo::GetPower) {
			if (msg.getMsg() == "item") {
				
			}
			else if (msg.getMsg() == "carga") {
				charge();
			}
			else
			{
				finCharge();
			}
		}
	}
	/*Mensaje ms(Tipo::Gui, "+0.01", SubTipo::CambiaMana);
	ms.setMsgInfo(pEntidad, pEntidad);
	pEntidad->getPEstado()->addMsg(ms);*/
}

void ManaComponent::restaPower()
{
	Mensaje ms(Tipo::Gui, "-25", SubTipo::CambiaMana);
	ms.setMsgInfo(pEntidad, pEntidad);
	pEntidad->getPEstado()->addMsg(ms, true);
}

void ManaComponent::charge()
{
	Mensaje ms(Tipo::Gui, "+0.5", SubTipo::CambiaMana);
	ms.setMsgInfo(pEntidad, pEntidad);
	pEntidad->getPEstado()->addMsg(ms, true);

	particleSystem->setEmitting(true);
	
}

void ManaComponent::finCharge()
{
	particleSystem->setEmitting(false);
}
void ManaComponent::restaurapS()
{
	Ogre::ParticleEmitter * em = particleSystem->getEmitter(0);
	em->setEnabled(false);
	em->setEnabled(true);

}