#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

const float GRAVITY = 9.8f;

float PlayScene::m_pRamp[2];
float PlayScene::m_lootWeight;
float PlayScene::m_friCoefficient;
bool PlayScene::m_isMoving;
float PlayScene::m_rampSlope;
Crate* PlayScene::m_pCrate;

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	TextureManager::Instance()->draw("background", 400, 300,800,600,0,255,true);
	
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}

	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);
	Util::DrawLine(glm::vec2(100.0f, 550.0f), glm::vec2(100.0f, 550.0f - m_pRamp[1]), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	Util::DrawLine(glm::vec2(100.0f, 550.0f), glm::vec2(100.0f + m_pRamp[0], 550.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	Util::DrawLine(glm::vec2(100.0f + m_pRamp[0], 550.0f), glm::vec2(100.0f, 550.0f - m_pRamp[1]), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void PlayScene::update()
{
	if (!m_isMoving)
		m_pCrate->getTransform()->position = glm::vec2(150.0f, 580.0f - m_pRamp[1]);
	
	updateDisplayList();

	m_rampSlope = glm::degrees(atan(m_pRamp[1]/m_pRamp[0]));
	m_pCrate->SetAngle(m_rampSlope);
	//std::cout << "w:h = " << m_pRamp[1] << "/" << m_pRamp[0] << std::endl;
	//std::cout << "Crate Angle: " << m_rampSlope << std::endl;	

	if (m_pCrate->getTransform()->position.y >= 580.0f)
		m_pCrate->setIsMoving(false);
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();	

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	//Set Background Image
	TextureManager::Instance()->load("../Assets/textures/BG.png", "background");
	
	//Set Crate
	m_pCrate = new Crate();
	m_pCrate->getTransform()->position = glm::vec2(100.0f, 400.0f);
	addChild(m_pCrate);
	
	// Set GUI Title
	m_guiTitle = "Preference";

	/* Instructions Label */
	//m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	//m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);
	//addChild(m_pInstructionsLabel);

	m_pRamp[0] = 150.0f;
	m_pRamp[1] = 100.0f;
	m_lootWeight = 10.0f;
	m_friCoefficient = 0.5f;
	m_isMoving = false;
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Preference", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if(ImGui::Button("Reset"))
	{
		//std::cout << "My Button Pressed" << std::endl;
		m_isMoving = false;
		m_pCrate->setIsMoving(false);
		m_pCrate->getTransform()->position = glm::vec2(150.0f, 600.0f - m_pRamp[1]);
		m_pCrate->getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
		m_pCrate->getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
		//std::cout << "Crate at " << m_pCrate->getTransform()->position.x << "," << m_pCrate->getTransform()->position.y << std::endl;
	}

	if(!m_isMoving)
	{
		if (ImGui::Button("Start"))
		{
			//std::cout << "My Button Pressed" << std::endl;
			m_isMoving = true;
			m_pCrate->setIsMoving(true);
			SetData();
		}
	}
	

	ImGui::Separator();

	if(ImGui::SliderFloat2("Ramp size", m_pRamp, 1.0f, 600.0f))
	{
		/*std::cout << m_pRamp[0] << std::endl;
		std::cout << m_pRamp[1] << std::endl;
		std::cout << "---------------------------\n";*/
	}
	
	ImGui::Separator();
	
	if (ImGui::SliderFloat("Crate Mass", &m_lootWeight, 1.0f, 100.0f))
	{
		
	}

	if (ImGui::SliderFloat("Coefficient of Friction", &m_friCoefficient, 0.0f, 1.0f))
	{

	}
	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}

void PlayScene::SetData()
{
	//std::cout << "angle: " << m_rampSlope << "tan: "<<glm::tan(glm::radians(m_rampSlope))<<std::endl;
	//std::cout << "xia: " << m_lootWeight * GRAVITY * glm::sin(glm::radians(m_rampSlope)) << " Fk: " << m_lootWeight * GRAVITY * glm::cos(glm::radians(m_rampSlope)) * m_friCoefficient << std::endl;
	float tempAcc = GRAVITY * glm::sin(glm::radians(m_rampSlope)) - GRAVITY * m_friCoefficient * glm::cos(glm::radians(m_rampSlope));
	if(tempAcc<0)
	{
		tempAcc = 0;
		std::cout << "The slider cannot move" << std::endl;
	}
	m_pCrate->getRigidBody()->acceleration = glm::vec2(tempAcc * glm::cos(glm::radians(m_rampSlope)), tempAcc * glm::sin(glm::radians(m_rampSlope)));	
	//std::cout << "acc: " << tempAcc << " x:" << m_pCrate->getRigidBody()->acceleration.x << " y: " << m_pCrate->getRigidBody()->acceleration.y << std::endl;
	//std::cout << "ratio: " << m_pCrate->getRigidBody()->acceleration.y / m_pCrate->getRigidBody()->acceleration.x << std::endl;
	/*m_pCrate->getRigidBody()->acceleration.y = (m_lootWeight * GRAVITY - m_lootWeight * GRAVITY * glm::cos(glm::radians(m_rampSlope)) * glm::sin(glm::radians(90.0f - m_rampSlope)) 
		- m_lootWeight * GRAVITY * m_friCoefficient * glm::sin(glm::radians(m_rampSlope))) / m_lootWeight;
	m_pCrate->getRigidBody()->acceleration.x = (m_lootWeight * GRAVITY * glm::cos(glm::radians(m_rampSlope)) * glm::cos(glm::radians(90.0f - m_rampSlope)) 
		- m_lootWeight * GRAVITY * glm::cos(glm::radians(m_rampSlope)) * m_friCoefficient * glm::cos(glm::radians(m_rampSlope))) / m_lootWeight;
	std::cout << "acc: " << Util::magnitude(m_pCrate->getRigidBody()->acceleration) << " x:" << m_pCrate->getRigidBody()->acceleration.x << " y: " << m_pCrate->getRigidBody()->acceleration.y << std::endl;*/
}
