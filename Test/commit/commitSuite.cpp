#include <gtest/gtest.h>

#include <sstream>
#include <filesystem>
#include <vector>

#include "ObjLoader.hpp"
#include "Rotation.hpp"
#include "OpenGLRendererConfig.hpp"
#include "GameObject.hpp"

// Demonstrate some basic assertions.
TEST(HelloTest1, BasicAssertions) {

	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);

}

TEST(ObjLoaderTest, blob)
{
	
	std::vector<std::shared_ptr<GameObject>> gameObjects;

  glm::vec3 sponza_offset = glm::vec3(0.f, 0.0f, 0.0f);
  GLfloat sponza_scale = 10.f;
  Rotation sponza_rot;
  sponza_rot.degrees = 0.0f;
  sponza_rot.axis = glm::vec3(0.0f, 1.0f, 0.0f);

  std::stringstream modelFile;
  std::filesystem::path cwd = std::filesystem::current_path();
  modelFile << cwd.string();
  modelFile << RELATIVE_RESOURCE_PATH << "Models/dinosaurs.obj";

  ASSERT_EQ(static_cast<uint32_t>(gameObjects.size()), 0);
  std::shared_ptr<GameObject> sponza = std::make_shared<GameObject>(
      modelFile.str(), sponza_offset, sponza_scale, sponza_rot);
  gameObjects.push_back(sponza);
  ASSERT_EQ(static_cast<uint32_t>(gameObjects.size()), 1);
}