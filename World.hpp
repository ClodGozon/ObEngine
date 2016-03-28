#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>

#include "Functions.hpp"
#include "Character.hpp"
#include "Spells.hpp"
#include "DataParser.hpp"
#include "TimeManager.hpp"
#include "LevelSprite.hpp"
#include "Light.hpp"
#include "Particle.hpp"

class World
{
private:
	std::string levelName = "";
	int sizeX = 0;
	int sizeY = 0;
	//vector<Object> objArray;
	std::vector<LevelSprite*> backSpriteArray;
	std::vector<LevelSprite*> frontSpriteArray;
	std::vector<Spells::Projectile*> spellArray;
	std::vector<Collision::PolygonalCollider*> collidersArray;
	std::map<std::string, Light::PointLight*> lightMap;
	std::vector<MathParticle*> particleArray;
	double camX = 0;
	double camY = 0;
	sf::Sprite backSprBlit;
	sf::RenderTexture renderTex;
	//LightSystem liSys;
	std::vector<Character*> charArray;
	double blurMul = 0.0003;
	sf::Shader blurShader;
	sf::Shader lightShader;
	int width = fn::Coord::width;
	int height = fn::Coord::height;
	int startX = 0;
	int startY = 0;
	anim::RessourceManager sprRsMan;
	double gameSpeed;
	std::map<std::string, bool> showCollisionModes;

public:
	void init();
	void addCharacter(Character* character);
	void addLevelSprite(LevelSprite* spr);
	void addCollider(Collision::PolygonalCollider* col);
	void addLight(Light::PointLight* lgt);
	Character* getCharacter(int index);
	void loadFromFile(std::string filename);
	DataParser* saveData();
	std::vector<Collision::PolygonalCollider*> getColliders();
	void update(double dt);
	void display(sf::RenderWindow* surf);
	void visualDisplayBack(sf::RenderWindow* surf);
	void visualDisplayFront(sf::RenderWindow* surf);
	int getSizeX();
	int getSizeY();
	void setCameraPosition(double tX, double tY, std::string setMode = "SET");
	double getCamX();
	double getCamY();
	void castSpell(Spells::Projectile* spellToCast);
	int getStartX();
	int getStartY();
	void addParticle(MathParticle* particle);
	void reorganizeLayers();
	void setBlurMul(double newBlur);
	//Map Editor
	LevelSprite* getSpriteByIndex(std::string backOrFront, int index);
	int getSpriteArraySize(std::string backOrFront);
	std::vector<LevelSprite*> getAllSprites();
	std::vector<LevelSprite*> getSpritesByLayer(int layer);
	LevelSprite* getSpriteByPos(int x, int y, int layer);
	LevelSprite* getSpriteByID(std::string ID);
	void deleteSprite(LevelSprite* sprToDelete);
	std::pair<Collision::PolygonalCollider*, int> getCollisionPointByPos(int x, int y);
	Collision::PolygonalCollider* getCollisionMasterByPos(int x, int y);
	Collision::PolygonalCollider* getCollisionByID(std::string id);
	std::vector<Collision::PolygonalCollider*> getAllCollidersByCollision(Collision::PolygonalCollider* col, int offx, int offy);
	void deleteCollisionByID(std::string id);
	void createCollisionAtPos(int x, int y);
	void enableShowCollision(bool drawLines = false, bool drawPoints = false, bool drawMasterPoint = false, bool drawSkel = false);
};