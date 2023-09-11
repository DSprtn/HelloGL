from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps


class HelloGLRecipe(ConanFile):
	name = "HelloGL"
	version = "1.0"
	package_type = "application"



	# Optional metadata
	license = "No license"
	author = "<Paul Górniak> <drtsprtn@gmail.com>"
	url = "<https://github.com/DSprtn/HelloGL>"
	description = "<Toy OpenGL renderer>"
	topics = ("<OpenGL>", "<IMGUI>", "<GLFW>")

	# Binary configuration
	settings = "os", "compiler", "build_type", "arch"

	# Sources are located in the same place as this recipe, copy them to the recipe
	exports_sources = "CMakeLists.txt", "src/*"


	def requirements(self):
		self.requires("assimp/5.2.2")
		self.requires("glfw/3.3.8")
		self.requires("glm/0.9.9.8")
		self.requires("glad/0.1.36")


		self.default_options = "glad/0.1.36:gl_profile=core", "glad/0.1.36:spec=gl", "glad/0.1.36:gl_version=4.6", "glad/0.1.36:spec=gl", "glad/0.1.36:no_loader=False", "assimp/5.2.2:shared=False"


	def build_requirements(self):
		self.tool_requires("cmake/3.26.4")

	#def layout(self):
		#cmake_layout(self)

	def generate(self):
		deps = CMakeDeps(self)
		deps.generate()
		tc = CMakeToolchain(self)
		tc.generate()

	def build(self):
		cmake = CMake(self)
		cmake.configure()
		cmake.build()

	def package(self):
		cmake = CMake(self)
		cmake.install()




