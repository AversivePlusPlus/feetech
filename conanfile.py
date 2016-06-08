from conans import ConanFile, CMake

class AversivePlusPlusModuleConan(ConanFile):
    name = "feetech"
    version = "0.1"
    exports = "*.hpp"
    requires = "stream/0.1@AversivePlusPlus/stable", "container/0.1@AversivePlusPlus/stable"

    def package(self):
        self.copy("*.hpp", src="include", dst="include")
