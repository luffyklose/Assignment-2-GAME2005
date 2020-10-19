#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

float PlayScene::m_pRamp[2];
float PlayScene::m_lootWeight;
float PlayScene::m_friCoefficient;

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
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
	updateDisplayList();	
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	/*if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}*/


	// handle player movement if no Game Controllers found
	/*if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}*/
	

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
	// Set GUI Title
	m_guiTitle = "Play Scene";

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel);

	m_pRamp[0] = 150.0f;
	m_pRamp[1] = 100.0f;
	m_lootWeight = 10.0f;
	m_friCoefficient = 0.5f;
	
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Preference", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if(ImGui::Button("Start"))
	{
		std::cout << "My Button Pressed" << std::endl;
	}

	ImGui::Separator();

	if(ImGui::SliderFloat2("Ramp size", m_pRamp, 1.0f, 600.0f))
	{
		std::cout << m_pRamp[0] << std::endl;
		std::cout << m_pRamp[1] << std::endl;
		std::cout << "---------------------------\n";
	}
	
	ImGui::Separator();
	
	if (ImGui::SliderFloat("Crate Mass", &m_lootWeight, 1.0f, 100.0f))
	{
		
	}

	if (ImGui::SliderFloat("Coefficient of Friction", &m_friCoefficient, 1.0f, 100.0f))
	{

	}	
	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
