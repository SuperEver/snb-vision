# tools 说明
## libjpeg
在非安卓编译环境需要用到。仅需要编译一次，进行libjpeg目录，使用如下命令编译

`mkdir build && cd build && cmake -DCMAKE_INSTALL_PREFIX=../install .. && make install && cd ..`
运行完毕后可以将build目录删除，install目录为最终期望产出
