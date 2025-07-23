cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("SebF2DEngine")

find_package(SFML 3 REQUIRED COMPONENTS Graphics)

add_executable (SebF2DEngine "SebF2DEngine.cpp" "SebF2DEngine.h" "ECS/Entity.h" "ECS/EntityManager.h" "Game/CTransform.h" "Maths/Vec2.h" "Game/CRigidbody.h" "Game/CShape.h" "ECS/Entity.cpp" "ECS/EntityManager.cpp" "Engine/GameEngine.h" "Engine/GameEngine.cpp" "Physics/SPhysics.h" "Physics/SPhysics.cpp" "Engine/Scene.h" "Scenes/SceneGravity.h" "Scenes/SceneGravity.cpp" "Game/CCollider.h" "Scenes/SceneCollision.h" "Scenes/SceneCollision.cpp" "Physics/PhysicsConstants.h" "Physics/Collision.h" "Physics/Collision.cpp" "Game/CCollisionState.h" "Scenes/SceneNoQuadTree.h" "Scenes/SceneQuadTree.h" "Scenes/SceneNoQuadTree.cpp" "Config/GameConfig.h" "Physics/QuadTree.h" "Physics/QuadTree.cpp" "Scenes/SceneQuadTree.cpp" "Utils/Logger.h" "Scenes/SceneFalling.h" "Scenes/SceneFalling.cpp" "Physics/Contact.h" "Physics/Contact.cpp" "Scenes/SceneFallingCircle.h" "Scenes/SceneFallingCircle.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SebF2DEngine PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(SebF2DEngine PRIVATE SFML::Graphics)