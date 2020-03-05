

## build opencv
```
mkdir build && cd build 
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../install -DWITH_CUDA=OFF -DWITH_PNG=OFF ..
make install -j 8
cd ..
```

## build ncnn
```
mkdir build && cd build 
cmake -DNCNN_VULKAN=OFF -DCMAKE_INSTALL_PREFIX=../install -DOpenCV_DIR=/Users/zhangyongchao/workspace/code/git/snb-vision/src/3rdparty/opencv-2.4/install/share/OpenCV ..
make install -j 8
cd ..
```

