sudo apt-get install mesa-* libwayland-dev libxrandr-dev libxcb-keysyms1-dev
sudo apt-get install libvulkan1 mesa-vulkan-drivers vulkan-utils libxcb-keysyms1-dev
sudo apt-get install libxcb1-dev libx11-dev
wget https://sdk.lunarg.com/sdk/download/1.2.162.1/linux/vulkansdk-linux-x86_64-1.2.162.1.tar.gz
mkdir vulkan 
mv vulkansdk-linux-x86_64-1.2.162.1.tar.gz vulkan
cd vulkan
tar xf vulkansdk-linux-x86_64-1.2.162.1.tar.gz
# 下载github
cd 1.2.162.1/source/shaderc
python update_shaderc_sources.py 
# 编译
cd 1.2.162.1
#bash vulkansdk  # 编译vulkan
#source setup-env.sh # vulkan -> 系统环境变量
#./x86_64/bin/vulkaninfo

