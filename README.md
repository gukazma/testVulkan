# kanjing vulkan-test

## how to use it
### Windows
download [vulkanSDK](https://vulkan.lunarg.com/sdk/home#windows)

install it

add bin dir to env path

* Step1: generate build project
```bash
# create build dir

mkdir build

cd build

# generate build project
cmake ..
```
* Step2: open build project

Open ``vulkan-begin.sln`` file in ``build`` dir by ``VS``.

**build  ``shaderCompile`` first!!!**

* Step3: set setup project

    Set one of ``test`` as start up project. pass cmd parameter `--gtest_filter=testVulkan.testVulkan`

* Step4: run test program

    Run without debug project.

### Ubuntu

```bash
sudo apt update

sudo apt-get install libsdl2-dev libx11-dev vulkan-sdk

clion .
```
./out/bin --gtest_filter=testVulkan.testVulkan